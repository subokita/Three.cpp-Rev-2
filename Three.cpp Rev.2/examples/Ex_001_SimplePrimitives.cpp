//
//  Ex_001_SimplePrimitives.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 22/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//
    
#include "Ex_001_SimplePrimitives.h"

using namespace std;

namespace three  {
    
    ptr<Ex_001_SimplePrimitives> Ex_001_SimplePrimitives::create(){
        return make_shared<Ex_001_SimplePrimitives>();
    }
    
    void Ex_001_SimplePrimitives::run() {
        ForwardRenderer renderer;
        renderer.init( "Ex 001: Simple Primitives", 1600 * 2 / 4, 900 * 2 / 4 );
        renderer.setCameraControl(Arcball::create(2.0f));
        
        /* Create scene */
        auto scene = Scene::create();
        scene->setFog(Fog::create( 0x72645b / 2, 2.0, 10.0 ));
        scene->setViewport(0, 0, 1600 * 2 / 4, 900 * 2 / 4);
        
        /* Create camera */
        auto camera = PerspectiveCamera::create( 50.0, renderer.getAspectRatio(), 0.001, 100.0 );
        camera->position = glm::vec3(0.0, 0.0, 5.5);
        camera->lookAt( 0.0, 0.0, 0.0 );
        
        /* A sphere, cube, and cylinder walk into a pub */
        auto sphere = Mesh::create( SphereGeometry::create(30, 20, 0.66f ),
                                    PhongMaterial::create( 0xCC00CC, 0x0, 0x0, 0x222222, 130.0, true ) );
        
        auto cube = Mesh::create( CubeGeometry::create( 1.0f ),
                                  PhongMaterial::create( 0x00CCCC, 0x0, 0x0, 0x111111, 150.0, false ) );
        
        cube->translate(2.0f, 0.0f, 0.0f);
        
        auto cylinder = Mesh::create( CylinderGeometry::create(0.5, 0.3, 1.0, 30, 5, true),
                                      PhongMaterial::create( 0xCCCC00, 0x0, 0x0, 0x111111, 150.0, false ) );
        cylinder->getMaterial()->setSide( SIDE::DOUBLE_SIDE );
        cylinder->translate(-2.0f, 0.0f, 0.0f);

        scene->add( sphere );
        scene->add( cube );
        scene->add( cylinder );
        
        auto plane = Mesh::create( PlaneGeometry::create(20.0f, 1),
                                   PhongMaterial::create(0x333333, 0x333333, 0x333333, 0x101010 ) );
        plane->rotateX(-90.0);
        plane->translate( 0.0, -1.5, 0.0 );
        scene->add( plane );
        
        /* Create directional light */
        auto dir_light = DirectionalLight::create(0x99CCFF, 1.35, glm::vec3(3.0, 1.0, 3.0) );
        scene->add( dir_light );
        
        scene->add(HemisphereLight::create(0xFFFFFF, 0x333333, 0.8));
        
        /* Create an ambient light */
        scene->add( AmbientLight::create(0x777777));
        
        /* Create a post render callback for objects rotation */
        bool rotate_objects = false;
        float light_rotation_1 = 0.0;
        renderer.setPostRenderCallbackHandler( [&](){
            dir_light->position.x = ( 2.0 * cosf( light_rotation_1 ) );
            dir_light->position.z = ( 2.0 * sinf( light_rotation_1 ) );
            
            light_rotation_1 += 0.01;
            
            if( rotate_objects ) {
                sphere->rotateY  (-1.0f);
                cube->rotateY    (-1.0f);
                cylinder->rotateX(-1.0f);
            }
        });
        
        /* Override key callback handler */
        renderer.setKeyCallbackHandler([&](GLFWwindow *window, int key, int scancode, int action, int mod) {
            if( action == GLFW_PRESS ) {
                switch ( key) {
                    case GLFW_KEY_R: /* Toggle rotation */
                        rotate_objects = !rotate_objects;
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