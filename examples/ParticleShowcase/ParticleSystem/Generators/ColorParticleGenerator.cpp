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
	_colors = particles->createAttribArray< RGBAColorf >( ParticleAttribType::COLOR );
	_startColors = particles->createAttribArray< RGBAColorf >( ParticleAttribType::START_COLOR );
	_endColors = particles->createAttribArray< RGBAColorf >( ParticleAttribType::END_COLOR );
}

void ColorParticleGenerator::generate( Node *node, crimild::Real64 dt, ParticleData *particles, ParticleId startId, ParticleId endId )
{
	auto cs = _colors->getData< RGBAColorf >();
	auto sc = _startColors->getData< RGBAColorf >();
	auto ec = _endColors->getData< RGBAColorf >();
	
	for ( ParticleId i = startId; i < endId; i++ ) {
		auto r = Random::generate< Real32 >( _minStartColor.r(), _maxStartColor.r() );
		auto g = Random::generate< Real32 >( _minStartColor.g(), _maxStartColor.g() );
		auto b = Random::generate< Real32 >( _minStartColor.b(), _maxStartColor.b() );
		auto a = Random::generate< Real32 >( _minStartColor.a(), _maxStartColor.a() );
		sc[ i ] = RGBAColorf( r, g, b, a );
	}

	for ( ParticleId i = startId; i < endId; i++ ) {
		auto r = Random::generate< Real32 >( _minEndColor.r(), _maxEndColor.r() );
		auto g = Random::generate< Real32 >( _minEndColor.g(), _maxEndColor.g() );
		auto b = Random::generate< Real32 >( _minEndColor.b(), _maxEndColor.b() );
		auto a = Random::generate< Real32 >( _minEndColor.a(), _maxEndColor.a() );
		ec[ i ] = RGBAColorf( r, g, b, a );
	}

    for ( ParticleId i = startId; i < endId; i++ ) {
        cs[ i ] = sc[ i ];
    }
}

