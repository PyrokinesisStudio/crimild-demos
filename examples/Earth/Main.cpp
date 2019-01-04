/*
 * Copyright (c) 2002-present, H. Hernán Saez
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <Crimild.hpp>
#include <Crimild_SDL.hpp>
#include <Crimild_OpenGL.hpp>

#include "Rendering/RenderGraph/RenderGraph.hpp"
#include "Rendering/RenderGraph/RenderGraphPass.hpp"
#include "Rendering/RenderGraph/RenderGraphAttachment.hpp"
#include "Rendering/RenderGraph/Passes/ForwardLightingPass.hpp"
#include "Rendering/RenderGraph/Passes/ScreenPass.hpp"
#include "Rendering/RenderGraph/Passes/DepthPass.hpp"
#include "Rendering/RenderGraph/Passes/BlendPass.hpp"
#include "Rendering/RenderGraph/Passes/DepthToRGBPass.hpp"
#include "Rendering/RenderGraph/Passes/FrameDebugPass.hpp"
#include "Rendering/RenderGraph/Passes/OpaquePass.hpp"
#include "Rendering/RenderGraph/Passes/LightAccumulationPass.hpp"
#include "Rendering/RenderGraph/Passes/TextureColorPass.hpp"
#include "Rendering/RenderGraph/Passes/DeferredLightingPass.hpp"

using namespace crimild;
using namespace crimild::rendergraph;
using namespace crimild::rendergraph::passes;

SharedPointer< RenderGraph > createRenderGraph( crimild::Bool useDebugPass )
{
	auto graph = crimild::alloc< RenderGraph >();

	auto depthPass = graph->createPass< passes::DepthPass >();
	auto lightingPass = graph->createPass< passes::LightAccumulationPass >();
	auto deferredLightingPass = graph->createPass< passes::DeferredLightingPass >();
	auto translucentTypes = containers::Array< RenderQueue::RenderableType > {
		RenderQueue::RenderableType::OPAQUE_CUSTOM,
		RenderQueue::RenderableType::TRANSLUCENT,
	};
	auto translucentPass = graph->createPass< passes::ForwardLightingPass >( translucentTypes );
	auto colorPass = graph->createPass< passes::BlendPass >();
    auto debugPass = graph->createPass< FrameDebugPass >();

	lightingPass->setDepthInput( depthPass->getDepthOutput() );
	lightingPass->setNormalInput( depthPass->getNormalOutput() );
	
	deferredLightingPass->setDepthInput( depthPass->getDepthOutput() );
	deferredLightingPass->setAmbientAccumInput( lightingPass->getAmbientOutput() );
	deferredLightingPass->setDiffuseAccumInput( lightingPass->getDiffuseOutput() );
	deferredLightingPass->setSpecularAccumInput( lightingPass->getSpecularOutput() );

	translucentPass->setDepthInput( depthPass->getDepthOutput() );

	colorPass->addInput( deferredLightingPass->getColorOutput() );
	colorPass->addInput( translucentPass->getColorOutput() );

	debugPass->addInput( colorPass->getOutput() );
    debugPass->addInput( depthPass->getNormalOutput() );
    debugPass->addInput( lightingPass->getAmbientOutput() );
    debugPass->addInput( lightingPass->getDiffuseOutput() );
    debugPass->addInput( lightingPass->getSpecularOutput() );
    debugPass->addInput( deferredLightingPass->getColorOutput() );
    debugPass->addInput( translucentPass->getColorOutput() );

	graph->setOutput( useDebugPass ? debugPass->getOutput() : colorPass->getOutput() );

	return graph;
}

SharedPointer< Node > buildAmbientLight( const RGBAColorf &color )
{
	auto light = crimild::alloc< Light >( Light::Type::AMBIENT );
	light->setAmbient( color );
	return light;
}

SharedPointer< Node > buildDirectionalLight( const RGBAColorf &color, const Vector3f &angles )
{
	auto light = crimild::alloc< Light >( Light::Type::DIRECTIONAL );
	light->setColor( color );
	light->local().rotate().fromEulerAngles( angles.x(), angles.y(), angles.z() );
	return light;
}

SharedPointer< Node > buildEarth( void )
{
	auto primitive = crimild::alloc< SpherePrimitive >( 1.0f, VertexFormat::VF_P3_N3_UV2 );
	auto geometry = crimild::alloc< Geometry >();
	geometry->attachPrimitive( primitive );

	auto material = crimild::alloc< Material >();
	material->setColorMap( AssetManager::getInstance()->get< Texture >( "assets/textures/earth-color.tga" ) );
	material->setSpecularMap( AssetManager::getInstance()->get< Texture >( "assets/textures/earth-specular.tga" ) );
	geometry->getComponent< MaterialComponent >()->attachMaterial( material );

	geometry->attachComponent( crimild::alloc< RotationComponent >( Vector3f( 0.0f, 1.0f, 0.0f ), 0.01 ) );

	return geometry;
}

int main( int argc, char **argv )
{
    auto settings = crimild::alloc< Settings >( argc, argv );
    settings->set( "video.show_frame_time", true );
    auto sim = crimild::alloc< sdl::SDLSimulation >( "Earth", settings );

    auto scene = crimild::alloc< Group >();

	scene->attachNode( buildEarth() );

	scene->attachNode( buildAmbientLight( RGBAColorf( 0.1f, 0.1f, 0.1f, 1.0f ) ) );
	scene->attachNode( buildDirectionalLight( RGBAColorf( 1.0f, 0.95f, 0.85f, 1.0f ), Vector3f( -0.05f * Numericf::PI, 0.25f * Numericf::PI, 0.0f ) ) );

	auto camera = crimild::alloc< Camera >( 45.0f, 4.0f / 3.0f, 0.1f, 1024.0f );
	camera->local().setTranslate( 0.0f, 0.0f, 3.0f );
    auto renderGraph = createRenderGraph( true );
    camera->setRenderPass( crimild::alloc< RenderGraphRenderPass >( renderGraph ) );
	scene->attachNode( camera );
    
    sim->setScene( scene );

	sim->registerMessageHandler< crimild::messaging::KeyReleased >( [ camera ]( crimild::messaging::KeyReleased const &msg ) {
		switch ( msg.key ) {
			case CRIMILD_INPUT_KEY_Q:
				crimild::concurrency::sync_frame( [ camera ]() {
					std::cout << "Normal" << std::endl;
					Renderer::getInstance()->setFrameBuffer( RenderPass::S_BUFFER_NAME, nullptr );
					auto renderGraph = createRenderGraph( false );
					camera->setRenderPass( crimild::alloc< RenderGraphRenderPass >( renderGraph ) );
				});
				break;

            case CRIMILD_INPUT_KEY_W:
                crimild::concurrency::sync_frame( [ camera ]() {
                    std::cout << "Debug" << std::endl;
                    Renderer::getInstance()->setFrameBuffer( RenderPass::S_BUFFER_NAME, nullptr );
                    auto renderGraph = createRenderGraph( true );
                    camera->setRenderPass( crimild::alloc< RenderGraphRenderPass >( renderGraph ) );
                });
                break;

			default:
				break;
		}
	});
	
	return sim->run();
}

