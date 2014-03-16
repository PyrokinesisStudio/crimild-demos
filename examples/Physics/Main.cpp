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

using namespace crimild;

class RestartComponent : public RigidBodyComponent {
public:
    RestartComponent( void ) { }
    virtual ~RestartComponent( void ) { }
    
    virtual void onAttach( void ) override
    {
        _startPos = getNode()->getLocal().getTranslate();
        _startForce = getForce();
        _t = 0;
    }
    
    virtual void update( const Time &t ) override
    {
        RigidBodyComponent::update( t );
        if ( _t >= 3.0f ) {
            getNode()->local().setTranslate( _startPos );
            setForce( _startForce );
            _t = 0;
        }
        else {
            _t += t.getDeltaTime();
        }
    }
    
private:
    Vector3f _startPos;
    Vector3f _startForce;
    float _t;
};

Pointer< Node > makeBall( float x, float y, float z, float fx = 0.0f, float fy = 0.0f, float fz = 0.0f )
{
	Pointer< Primitive > sphere( new SpherePrimitive( 1.0 ) );
	Pointer< Geometry > geometry( new Geometry() );
	geometry->attachPrimitive( sphere.get() );
	geometry->local().setTranslate( x, y, z );

	Pointer< RigidBodyComponent > rigidBody( new RestartComponent() );
	rigidBody->setForce( Vector3f( fx, fy, fz ) );
	geometry->attachComponent( rigidBody.get() );

	Pointer< ColliderComponent > collider( new ColliderComponent() );
	geometry->attachComponent( collider.get() );
    
	return geometry;
}

Pointer< Node > makeGround( void )
{
	Pointer< Primitive > primitive( new QuadPrimitive( 10.0f, 10.0f ) );
	Pointer< Geometry > geometry( new Geometry() );
	geometry->attachPrimitive( primitive.get() );
	geometry->local().setRotate( Vector3f( 1.0f, 0.0f, 0.0f ), -Numericf::HALF_PI );
	
	Pointer< PlaneBoundingVolume > planeBoundingVolume( new PlaneBoundingVolume( Plane3f( Vector3f( 0.0f, 1.0f, 0.0f ), 0.0f ) ) );
	Pointer< ColliderComponent > collider( new ColliderComponent( planeBoundingVolume.get() ) );
	geometry->attachComponent( collider.get() );

	return geometry;
}

int main( int argc, char **argv )
{
	Pointer< Simulation > sim( new GLSimulation( "Physics", argc, argv ) );

	Pointer< Group > scene( new Group() );
	scene->attachNode( makeGround().get() );
	scene->attachNode( makeBall( -2.0f, 1.0f, 0.0f, 5.0f, 2.0f ).get() );
	scene->attachNode( makeBall( 2.0f, 5.0f, 0.0f ).get() );

	Pointer< Camera > camera( new Camera() );
    camera->setRenderPass( new ForwardRenderPass() );
	camera->local().setTranslate( 0.0f, 3.0f, 10.0f );
	camera->local().lookAt( Vector3f( 0.0f, 0.0f, 0.0f ), Vector3f( 0.0f, 1.0f, 0.0f ) );
	scene->attachNode( camera.get() );

	Pointer< Light > light( new Light() );
	light->local().setTranslate( -5.0f, 5.0f, 1.0f );
    light->local().lookAt( Vector3f( 0.0f, 0.0f, 0.0f ), Vector3f( 0.0f, 1.0f, 0.0f ) );
    light->setCastShadows( true );
    light->setShadowNearCoeff( 0.1f );
    light->setShadowFarCoeff( 10.0f );
	scene->attachNode( light.get() );

	sim->setScene( scene.get() );

	return sim->run();
}
