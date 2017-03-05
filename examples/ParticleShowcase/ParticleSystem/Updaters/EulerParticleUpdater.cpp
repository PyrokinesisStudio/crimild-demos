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

#include "EulerParticleUpdater.hpp"

using namespace crimild;

EulerParticleUpdater::EulerParticleUpdater( void )
{

}

EulerParticleUpdater::~EulerParticleUpdater( void )
{

}

void EulerParticleUpdater::configure( Node *node, ParticleData *particles )
{
	auto pAttribs = particles->getAttrib( ParticleAttribType::POSITION );
	assert( pAttribs != nullptr );

	_positions = pAttribs->getData< Vector3f >();
	assert( _positions != nullptr );

	auto vAttribs = particles->getAttrib( ParticleAttribType::VELOCITY );
	assert( vAttribs != nullptr );

	_velocities = vAttribs->getData< Vector3f >();
	assert( _velocities != nullptr );

	auto aAttribs = particles->getAttrib( ParticleAttribType::ACCELERATION );
	assert( aAttribs != nullptr );

	_accelerations = aAttribs->getData< Vector3f >();
	assert( _accelerations != nullptr );
}

void EulerParticleUpdater::update( Node *node, crimild::Real64 dt, ParticleData *particles )
{
	const auto g = dt * _globalAcceleration;
	const auto count = particles->getAliveCount();

	for ( int i = 0; i < count; i++ ) {
		_accelerations[ i ] += g;
	}

	for ( int i = 0; i < count; i++ ) {
		_velocities[ i ] += dt * _accelerations[ i ];
	}

	for ( int i = 0; i < count; i++ ) {
		auto v = dt * _velocities[ i ];
		_positions[ i ] += v;
	}
}

