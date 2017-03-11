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

#ifndef CRIMILD_PARTICLE_GENERATOR_COLOR_
#define CRIMILD_PARTICLE_GENERATOR_COLOR_

#include "../ParticleSystemComponent.hpp"

namespace crimild {

    class ColorParticleGenerator : public ParticleSystemComponent::ParticleGenerator {
    public:
        ColorParticleGenerator( void );
        virtual ~ColorParticleGenerator( void );

		inline void setMinStartColor( const RGBAColorf &color ) { _minStartColor = color; }
		inline const RGBAColorf getMinStartColor( void ) const { return _minStartColor; }

		inline void setMaxStartColor( const RGBAColorf &color ) { _maxStartColor = color; }
		inline const RGBAColorf getMaxStartColor( void ) const { return _maxStartColor; }

		inline void setMinEndColor( const RGBAColorf &color ) { _minEndColor = color; }
		inline const RGBAColorf getMinEndColor( void ) const { return _minEndColor; }

		inline void setMaxEndColor( const RGBAColorf &color ) { _maxEndColor = color; }
		inline const RGBAColorf getMaxEndColor( void ) const { return _maxEndColor; }

		virtual void configure( Node *node, ParticleData *particles ) override;
        virtual void generate( Node *node, crimild::Real64 dt, ParticleData *particles, ParticleId startId, ParticleId endId ) override;

    private:
		RGBAColorf _minStartColor;
		RGBAColorf _maxStartColor;
		RGBAColorf _minEndColor;
		RGBAColorf _maxEndColor;

        ParticleAttribArray *_colors = nullptr;
		ParticleAttribArray *_startColors = nullptr;
		ParticleAttribArray *_endColors = nullptr;
    };

}

#endif

