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

#include "ParticleEmitterComponent.hpp"
#include "ParticleSystemComponent.hpp"

using namespace crimild;

ParticleEmitterComponent::ParticleEmitterComponent( void )
{

}

ParticleEmitterComponent::~ParticleEmitterComponent( void )
{

}

void ParticleEmitterComponent::start( void )
{
    auto ps = getComponent< ParticleSystemComponent >();
	assert( ps != nullptr );
	
    _particles = ps->getParticles();
	assert( _particles != nullptr );

	auto node = getNode();

	auto gCount = _generators.getCount();
    for ( int i = 0; i < gCount; i++ ) {
		_generators[ i ]->configure( node, _particles );
	}
}

void ParticleEmitterComponent::update( const Clock &c )
{
    const auto dt = c.getDeltaTime();

    const ParticleId maxNewParticles = _burst ? _emitRate : Numeric< ParticleId >::max( 1, dt * _emitRate );
    const ParticleId startId = _particles->getAliveCount();            
    const ParticleId endId = Numeric< ParticleId >::min( startId + maxNewParticles, _particles->getParticleCount() - 1 );

	auto node = getNode();

	auto gCount = _generators.getCount();
	for ( int i = 0; i < gCount; i++ ) { // TODO: optimize this loop	   
		_generators[ i ]->generate( node, dt, _particles, startId, endId );
	}

    for ( ParticleId i = startId; i < endId; i++ ) {
        _particles->wake( i );
    }
}

