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

#include "CameraSortParticleUpdater.hpp"

using namespace crimild;

CameraSortParticleUpdater::CameraSortParticleUpdater( void )
{

}

CameraSortParticleUpdater::~CameraSortParticleUpdater( void )
{

}

void CameraSortParticleUpdater::configure( Node *node, ParticleData *particles )
{
    auto pAttribs = particles->getAttrib( ParticleAttribType::POSITION );
    assert( pAttribs != nullptr );
    
    _positions = pAttribs->getData< Vector3f >();
    assert( _positions != nullptr );
}

void CameraSortParticleUpdater::update( Node *node, double dt, ParticleData *particles )
{
	// TODO: compute in world space

	const auto camera = Camera::getMainCamera();
	auto cameraPos = camera->getWorld().getTranslate();
	node->getWorld().applyInverseToPoint( cameraPos, cameraPos );

	const auto pCount = particles->getAliveCount();

	// TODO: I know, bubble sort is slow...
	// TODO: Also, precompute distances
	for ( int i = 0; i < pCount; i++ ) {
		auto pos = _positions[ i ];
		for ( int j = i + 1; j < pCount; j++ ) {
			if ( Distance::computeSquared( _positions[ j ], cameraPos ) > Distance::computeSquared( _positions[ i ], cameraPos ) ) {
				particles->swap( i, j );
			}
		}
	}
}

