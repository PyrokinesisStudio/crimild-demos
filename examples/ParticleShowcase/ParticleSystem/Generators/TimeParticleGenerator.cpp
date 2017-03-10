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

#include "TimeParticleGenerator.hpp"

using namespace crimild;

TimeParticleGenerator::TimeParticleGenerator( void )
{

}

TimeParticleGenerator::~TimeParticleGenerator( void )
{

}

void TimeParticleGenerator::configure( Node *node, ParticleData *particles )
{
    auto tAttribs = particles->getAttrib( ParticleAttribType::TIME );
	if ( tAttribs == nullptr ) {
		particles->setAttribs( ParticleAttribType::TIME, crimild::alloc< Real32ParticleAttribArray >() );
		tAttribs = particles->getAttrib( ParticleAttribType::TIME );
	}
    _times = tAttribs->getData< crimild::Real32 >();

    auto lAttribs = particles->getAttrib( ParticleAttribType::LIFE_TIME );
	if ( lAttribs == nullptr ) {
		particles->setAttribs( ParticleAttribType::LIFE_TIME, crimild::alloc< Real32ParticleAttribArray >() );
		lAttribs = particles->getAttrib( ParticleAttribType::LIFE_TIME );
	}
    _lifeTimes = lAttribs->getData< crimild::Real32 >();
}

void TimeParticleGenerator::generate( Node *node, crimild::Real64 dt, ParticleData *particles, ParticleId startId, ParticleId endId )
{
    for ( ParticleId i = startId; i < endId; i++ ) {
		auto t = Random::generate< crimild::Real32 >( _minTime, _maxTime );
		_lifeTimes[ i ] = t;
		_times[ i ] = t;
    }
}
