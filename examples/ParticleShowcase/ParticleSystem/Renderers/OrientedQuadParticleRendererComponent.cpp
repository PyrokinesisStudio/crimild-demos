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

#include "OrientedQuadParticleRendererComponent.hpp"

#include "../ParticleSystemComponent.hpp"

using namespace crimild;

OrientedQuadParticleRendererComponent::OrientedQuadParticleRendererComponent( void )
{
	// create the material here so it can be modified later
	_material = crimild::alloc< Material >();

    auto program = crimild::retain( AssetManager::getInstance()->get< ShaderProgram >( Renderer::SHADER_PROGRAM_UNLIT_TEXTURE ) );
    _material->setProgram( program );
}

OrientedQuadParticleRendererComponent::~OrientedQuadParticleRendererComponent( void )
{

}

void OrientedQuadParticleRendererComponent::onAttach( void )
{
	_geometry = crimild::alloc< Geometry >();
	if ( _material != nullptr ) {
		_geometry->getComponent< MaterialComponent >()->attachMaterial( _material );
	}

	getNode< Group >()->attachNode( _geometry );
}

void OrientedQuadParticleRendererComponent::onDetach( void )
{

}

void OrientedQuadParticleRendererComponent::start( void )
{
	const auto ps = getComponent< ParticleSystemComponent >();
	_particles = ps->getParticles();
	_positions = _particles->getAttrib( ParticleAttribType::POSITION )->getData< Vector3f >();
	_colors = _particles->getAttrib( ParticleAttribType::COLOR )->getData< RGBAColorf >();
	_sizes = _particles->getAttrib( ParticleAttribType::UNIFORM_SCALE )->getData< crimild::Real32 >();

    _primitive = crimild::alloc< Primitive >( Primitive::Type::TRIANGLES );

	_geometry->attachPrimitive( _primitive );
}

void OrientedQuadParticleRendererComponent::update( const Clock &c )
{
    const auto pCount = _particles->getAliveCount();
    if ( pCount == 0 ) {
        return;
    }
    
    auto vbo = crimild::alloc< VertexBufferObject >( VertexFormat::VF_P3_UV2, 4 * pCount );
	auto ibo = crimild::alloc< IndexBufferObject >( 6 * pCount );

	const auto camera = Camera::getMainCamera();
	auto cameraUp = camera->getWorld().computeUp();
	auto cameraRight = camera->getWorld().computeRight();

	const auto node = getNode();
	node->getWorld().applyInverseToVector( cameraUp, cameraUp );
	node->getWorld().applyInverseToVector( cameraRight, cameraRight );

	const auto offset0 = cameraUp - cameraRight;
	const auto offset1 = -cameraUp - cameraRight;
	const auto offset2 = -cameraUp + cameraRight;
	const auto offset3 = cameraUp + cameraRight;

	const auto uv0 = Vector2f( 0.0f, 0.0f );
	const auto uv1 = Vector2f( 0.0f, 1.0f );
	const auto uv2 = Vector2f( 1.0f, 1.0f );
	const auto uv3 = Vector2f( 1.0f, 0.0f );

	// TODO: compute in world space

	for ( auto i = 0; i < pCount; i++ ) {
		auto idx = i * 4;
		auto pos = _positions[ i ];
		auto s = _sizes[ i ];
		
		vbo->setPositionAt( idx + 0, pos + s * offset0 );
		vbo->setTextureCoordAt( idx + 0, uv0 );
		
		vbo->setPositionAt( idx + 1, pos + s * offset1 );
		vbo->setTextureCoordAt( idx + 1, uv1 );
		
		vbo->setPositionAt( idx + 2, pos + s * offset2 );
		vbo->setTextureCoordAt( idx + 2, uv2 );

		vbo->setPositionAt( idx + 3, pos + s * offset3 );
		vbo->setTextureCoordAt( idx + 3, uv3 );
	}

	for ( auto i = 0; i < pCount; i++ ) {
		const auto idx = i * 6;
		const auto vdx = i * 4;
		ibo->setIndexAt( idx + 0, vdx + 0 );
		ibo->setIndexAt( idx + 1, vdx + 1 );
		ibo->setIndexAt( idx + 2, vdx + 2 );
		ibo->setIndexAt( idx + 3, vdx + 0 );
		ibo->setIndexAt( idx + 4, vdx + 2 );
		ibo->setIndexAt( idx + 5, vdx + 3 );
	}

    crimild::concurrency::sync_frame( [this, vbo, ibo] {
        _primitive->setVertexBuffer( vbo );
        _primitive->setIndexBuffer( ibo );
    });
}


