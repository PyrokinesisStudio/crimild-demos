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

#include "ColorParticleGenerator.hpp"

using namespace crimild;

ColorParticleGenerator::ColorParticleGenerator( void )
{

}

ColorParticleGenerator::~ColorParticleGenerator( void )
{

}

void ColorParticleGenerator::configure( Node *node, ParticleData *particles )
{
    auto cAttribs = particles->getAttrib( ParticleAttribType::COLOR );
	if ( cAttribs == nullptr ) {
		particles->setAttribs( ParticleAttribType::COLOR, crimild::alloc< RGBAColorfParticleAttribArray >() );
		cAttribs = particles->getAttrib( ParticleAttribType::COLOR );
	}
    _colors = cAttribs->getData< RGBAColorf >();
    
	auto sColorAttribs = particles->getAttrib( ParticleAttribType::START_COLOR );
	if ( sColorAttribs == nullptr ) {
		particles->setAttribs( ParticleAttribType::START_COLOR, crimild::alloc< RGBAColorfParticleAttribArray >() );
		sColorAttribs = particles->getAttrib( ParticleAttribType::START_COLOR );
	}
	_startColors = sColorAttribs->getData< RGBAColorf >();

	auto eColorAttribs = particles->getAttrib( ParticleAttribType::END_COLOR );
	if ( eColorAttribs == nullptr ) {
		particles->setAttribs( ParticleAttribType::END_COLOR, crimild::alloc< RGBAColorfParticleAttribArray >() );
		eColorAttribs = particles->getAttrib( ParticleAttribType::END_COLOR );
	}
	_endColors = eColorAttribs->getData< RGBAColorf >();
}

void ColorParticleGenerator::generate( Node *node, crimild::Real64 dt, ParticleData *particles, ParticleId startId, ParticleId endId )
{
	for ( ParticleId i = startId; i < endId; i++ ) {
		auto r = Random::generate< Real32 >( _minStartColor.r(), _maxStartColor.r() );
		auto g = Random::generate< Real32 >( _minStartColor.g(), _maxStartColor.g() );
		auto b = Random::generate< Real32 >( _minStartColor.b(), _maxStartColor.b() );
		auto a = Random::generate< Real32 >( _minStartColor.a(), _maxStartColor.a() );
		_startColors[ i ] = RGBAColorf( r, g, b, a );
	}

	for ( ParticleId i = startId; i < endId; i++ ) {
		auto r = Random::generate< Real32 >( _minEndColor.r(), _maxEndColor.r() );
		auto g = Random::generate< Real32 >( _minEndColor.g(), _maxEndColor.g() );
		auto b = Random::generate< Real32 >( _minEndColor.b(), _maxEndColor.b() );
		auto a = Random::generate< Real32 >( _minEndColor.a(), _maxEndColor.a() );
		_endColors[ i ] = RGBAColorf( r, g, b, a );
	}

    for ( ParticleId i = startId; i < endId; i++ ) {
        _colors[ i ] = _startColors[ i ];
    }
}
