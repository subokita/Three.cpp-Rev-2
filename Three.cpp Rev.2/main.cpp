//
//  main.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>
#include "three/three.h"
#include "examples/examples_header.h"

using namespace std;
using namespace three;

int main(int argc, const char * argv[])
{
//    Ex_001_SimplePrimitives::create()->run();
//    Ex_002_TextureSpecularNormalMappings::create()->run();
//    Ex_003_EnvironmentMapping::create()->run();
//    Ex_004_ShadowMapping::create()->run();
    
    const string path = "/Users/saburookita/Personal Projects/Three.cpp Rev.2/examples/assets/";
    
    Renderer renderer;
    renderer.init( "Ex 005: Loading PLY objects", 1600 * 2 / 4, 900 * 2 / 4 );
    
    /* Create scene */
    auto scene = Scene::create();
    scene->setFog(Fog::create( 0x72645b / 2, 2.0, 15.0 ));
    scene->setViewport( 0.0, 0.0, renderer.getWidth(), renderer.getHeight() );
    scene->setShadowMapType( SHADOW_MAP::PCF_SOFT );
    
    /* Create camera */
    auto camera = PerspectiveCamera::create( 50.0, renderer.getAspectRatio(), 0.001, 100.0 );
    camera->translate(0.0, 1.5, 5.5);
    camera->lookAt( 0.0, 0.0, 0.0 );
    
    
    
    auto statue = Loader::loadPLY(path + "/ply models/", "happy_vrip_res4.ply");
    statue->setMaterial(PhongMaterial::create(0x777777, 0x0, 0x0, 0x999999, 30, true));
    statue->scale = glm::vec3(10.0);
    statue->translate( glm::vec3(0.0, -1.6, 0.0) );
    statue->castShadow      = true;
    statue->receiveShadow   = true;
    scene->add( statue );
    
    
    /* Create our objects */
    auto sphere = Mesh::create( SphereGeometry::create(30, 20, 0.66f ),
                               PhongMaterial::create(0x777777, 0x0, 0x0, 0x999999, 30, true) );
    
    sphere->setNormalMap( TextureUtils::loadAsNormalMap  ( path, "tutorial_normals07.gif" ) );
    sphere->translate(+2.0, 0.0, 0.0);
    sphere->castShadow = true;
    sphere->receiveShadow = true;
    
    scene->add( sphere );
    
    /* And the ground plane */
    auto plane = Mesh::create( PlaneGeometry::create(20.0f),
                              PhongMaterial::create(0x777777, 0x777777, 0x0, 0x999999, 30) );
    plane->name = "plane";
    plane->rotateX(-90.0f);
    plane->translate(0.0, -1.0, 0.0);
    plane->receiveShadow = true;
    scene->add( plane );
    
    /* Cubemap */
    auto env = Mesh::create( CubeGeometry::create(20.0f), MeshCubeMapMaterial::create() );
    env->setTexture( TextureUtils::loadAsEnvMap( path + "cube/pisa",
                                                "nx.png", "ny.png", "nz.png",
                                                "px.png", "py.png", "pz.png"));
    
    statue->setEnvMap( downcast(env->getTexture(), EnvMap) );
    sphere->setEnvMap( downcast(env->getTexture(), EnvMap) );
    scene->add( env );
    
    
    /* Create a (rotating) directional light */
    auto dir_light = DirectionalLight::create(0x99CCFF, 1.35, glm::vec3( 3.0, 1.0, 3.0 ) );
    dir_light->castShadow       = true;
    dir_light->shadowBias       = -0.05;
    dir_light->shadowMapSize    = glm::vec2(1024);
    scene->add( dir_light );
    
    /* Create a spotlight, the shadow should be casted no the left hand side */
    auto spot_light = SpotLight::create(0x99CCFF, 1.0, 20.0, 50.0, 1.0 );
    spot_light->position   = glm::vec3(3.0, 2.0, 3.0);
    spot_light->castShadow = true;
    spot_light->shadowCameraFOV = 50.0;
    scene->add( spot_light );
    
    /* Create an ambient light */
    scene->add( AmbientLight::create(0x777777));
    
    /* Create a post render callback for objects rotation */
    bool rotate_objects = false;
    float light_rotation_1 = 0.0;
    renderer.setPostRenderCallbackHandler( [&](){
        dir_light->position.x = 3.0 * cosf( light_rotation_1 );
        dir_light->position.z = 3.0 * sinf( light_rotation_1 );
        
        light_rotation_1 += 0.01;
        
        if( rotate_objects ) {
            statue->rotateY(1.0f);
        }
    });
    
    /* Override key callback handler */
    renderer.setKeyCallbackHandler([&](GLFWwindow *window, int key, int scancode, int action, int mod) {
        if( action == GLFW_PRESS ) {
            switch ( key) {
                case GLFW_KEY_ESCAPE: case GLFW_KEY_Q:
                    glfwSetWindowShouldClose( window, GL_TRUE );
                    return;
                    
                case GLFW_KEY_W:
                    statue->getMaterial()->setWireframe( !statue->getMaterial()->isWireframe() );
                    sphere->getMaterial()->setWireframe( !sphere->getMaterial()->isWireframe() );
                    break;
                    
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
    
    return 0;
}

