//
//  Ex_009_ParticleSystem.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 19/09/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Ex_009_ParticleSystem.h"
#include <sstream>

using namespace std;

namespace three  {
    
    ptr<Ex_009_ParticleSystem> Ex_009_ParticleSystem::create(){
        return make_shared<Ex_009_ParticleSystem>();
    }
    
    /**
     * Example on how to use particle system
     * It's still very much a prototype, please refrain from using it
     */
    void Ex_009_ParticleSystem::run() {
        const string path = "/Users/saburookita/Personal Projects/Three.cpp Rev.2/examples/assets/";
        
        ForwardRenderer renderer;
        renderer.init( "Ex 009: Particle System", 1600 * 2 / 4, 900 * 2 / 4 );
        renderer.setCameraControl(Arcball::create(2.0f));
        
        /* Create scene */
        auto scene = Scene::create();
        scene->setFog(Fog::create( 0x0, 2.0, 1000.0 ));
        scene->setViewport( 0.0, 0.0, renderer.getWidth(), renderer.getHeight() );
        scene->setShadowMapType( SHADOW_MAP::PCF_SOFT );
        
        /* Create camera */
        auto camera = PerspectiveCamera::create( 90.0, renderer.getAspectRatio(), 0.001, 1000.0 );
        camera->translate(0.0, 1.5, 5.5);
        camera->lookAt( 0.0, 1.0, 0.0 );
        
        auto geometry = Geometry::create();
        for( int i = 0; i < 1000; i++ ) {
            geometry->addVertex( glm::vec3(MathUtils::randomFloat(-500.0, 500.0),
                                           MathUtils::randomFloat(-500.0, 500.0),
                                           MathUtils::randomFloat(-500.0, 500.0)) );
        }
        
        auto material = ParticleSystemMaterial::create();
        material->setColor( 0x99CCFF );
        material->setSizeAttenuation( true );
        material->setPointSize(35.0f);
        
        ptr<ParticleSystem> particles = ParticleSystem::create(geometry, material);
        particles->setTexture( TextureUtils::loadAsTexture(path, "disc.png") );
        particles->setSortParticles(true);
        
        scene->add( static_cast<ptr<Mesh>>(particles) );
        
        
        /* Override key callback handler */
        renderer.setKeyCallbackHandler([&](GLFWwindow *window, int key, int scancode, int action, int mod) {
            if( action == GLFW_PRESS ) {
                switch ( key) {
                    case GLFW_KEY_R: /* Toggle rotation */
                        break;
                        
                    default:
                        break;
                }
            }
        });
        
        
        renderer.setGamma( true, true );
        
        renderer.setClearColor( scene->getFog()->getColor() );
        renderer.render(scene, camera );

    }
}