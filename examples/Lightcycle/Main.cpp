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
		group->attachNode( model );
		Pointer< RotationComponent > rotationComponent( new RotationComponent( Vector3f( 0, 1, 0 ), -0.01 ) );
		group->attachComponent( rotationComponent );
		scene->attachNode( group );
	}

	Pointer< Light > light( new Light() );
	light->local().setTranslate( 0.0f, 0.0f, 10.0f );
	scene->attachNode( light );

	Pointer< Camera > camera( new Camera() );
	camera->local().setTranslate( -0.5f, 0.75f, 3.0f );
	scene->attachNode( camera );

	Pointer< OffscreenRenderPass > renderPass( new OffscreenRenderPass() );
	camera->setRenderPass( renderPass );
	Pointer< ImageEffect > glowEffect( new ImageEffect() );
	Pointer< ShaderProgram > glowProgram( new gl3::GlowShaderProgram() );
	glowEffect->setProgram( glowProgram );
	renderPass->attachImageEffect( glowEffect );

	sim->setScene( scene );
	return sim->run();
}

