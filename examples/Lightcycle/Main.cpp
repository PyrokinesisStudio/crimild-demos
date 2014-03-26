/*
 * Copyright (c) 2013, Hernan Saez
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
#include <Crimild_GLFW.hpp>

#include <fstream>
#include <string>
#include <vector>

using namespace crimild;

Pointer< Node > makeGround( void )
{
	Pointer< Primitive > primitive( new QuadPrimitive( 100.0f, 100.0f ) );
	Pointer< Geometry > geometry( new Geometry() );
	geometry->attachPrimitive( primitive.get() );
	geometry->local().setRotate( Vector3f( 1.0f, 0.0f, 0.0f ), -Numericf::HALF_PI );
	
	return geometry;
}

int main( int argc, char **argv )
{
	Pointer< Simulation > sim( new GLSimulation( "Lightcycle", argc, argv ) );

	Pointer< Group > scene( new Group() );

	OBJLoader loader( FileSystem::getInstance().pathForResource( "assets/HQ_Movie cycle.obj" ) );
	Pointer< Node > model = loader.load();
	if ( model != nullptr ) {
		Pointer< Group > group( new Group() );
		Quaternion4f q0, q1;
		q0.fromAxisAngle( Vector3f( 1.0f, 0.0f, 0.0f ), -Numericf::HALF_PI );
		q1.fromAxisAngle( Vector3f( 0.0f, 0.0f, 1.0f ), -Numericf::HALF_PI );
		model->local().setRotate( q0 * q1 );
		group->attachNode( model.get() );
		Pointer< RotationComponent > rotationComponent( new RotationComponent( Vector3f( 0, 1, 0 ), -0.01 ) );
		group->attachComponent( rotationComponent.get() );
		scene->attachNode( group.get() );
	}
    
    scene->attachNode( makeGround().get() );

	Pointer< Light > light( new Light() );
	light->local().setTranslate( 5.0f, 5.0f, 2.0f );
    light->local().lookAt( Vector3f( -1.0f, 0.0f, 0.0f ), Vector3f( 0.0f, 1.0f, 0.0f ) );
    light->setCastShadows( true );
    light->setShadowNearCoeff( 1.0f );
    light->setShadowFarCoeff( 50.0f );
	scene->attachNode( light.get() );

	Pointer< Camera > camera( new Camera() );
	camera->local().setTranslate( 1.5f, 2.15f, 3.25f );
    camera->local().lookAt( Vector3f( -1.0f, 0.0f, 0.5f ), Vector3f( 0.0f, 1.0f, 0.0f ) );
	scene->attachNode( camera.get() );
    
#if 1
    camera->setRenderPass( new ForwardRenderPass() );
#else
    camera->setRenderPass( new DeferredRenderPass() );
    camera->getRenderPass()->getImageEffects().add( new gl3::GlowImageEffect() );
#endif

	sim->setScene( scene.get() );
	return sim->run();
}

