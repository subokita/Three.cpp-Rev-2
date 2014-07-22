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


ptr<Object3D> createCompositeObject();
ptr<Mesh> sphere, cube, cylinder;

const string path = "/Users/saburookita/Personal Projects/Three.cpp Rev.2/examples/assets/";

int main(int argc, const char * argv[])
{
//    Ex_001_SimplePrimitives::create()->run();
//    Ex_002_TextureSpecularNormalMappings::create()->run();
    Ex_003_EnvironmentMapping::create()->run();
    
    return 1;
    
    Renderer renderer;
    renderer.init( "", 1600 * 3 / 4, 900 * 3 / 4 );
    
    /* Create scene */
    auto scene = Scene::create();
    scene->fog = Fog::create( 0x72645b / 2, 2.0, 100.0 );
    
    /* Create camera */
    auto camera = PerspectiveCamera::create( 50.0, renderer.aspectRatio, 0.001, 100.0 );
    camera->position = glm::vec3(0.0, 1.5, 5.5);
    camera->lookAt( 0.0, 1.5, 0.0 );
    
    
    /* Add our composite object */
    auto composite = createCompositeObject();
    scene->add( composite );
    
    /* Create ground plane */
//    auto plane_mesh = Mesh::create( PlaneGeometry::create(20.0f, 1),
//                                    PhongMaterial::create(0x333333, 0x000000, 0x333333, 0x101010 ) );
//    plane_mesh->rotateX(-90.0);
//    scene->add( plane_mesh );
    
    auto env = Mesh::create( CubeGeometry::create(20.0f), MeshCubeMapMaterial::create() );
    env->texture = TextureUtils::loadAsEnvMap( path + "cube/pisa",
                                              "nx.png", "ny.png", "nz.png",
                                              "px.png", "py.png", "pz.png");
    
    scene->add( env );
    
    sphere->envMap      = downcast(env->texture, EnvMap);
    cube->envMap        = downcast(env->texture, EnvMap);
    cylinder->envMap    = downcast(env->texture, EnvMap);
    
    
    /* Create directional light */
    auto dir_light = DirectionalLight::create(0x99CCFF, 1.35, glm::vec3(3.0, 1.0, 3.0) );
    scene->add( dir_light );
    
    
    /* Create an ambient light */
    scene->add( AmbientLight::create(0x777777));
    
    /* Create a post render callback function that allow us to rotate the light and objects */
    float light_rotation_1 = 0.0;
    bool rotate_objects = false;
    
    renderer.setPostRenderCallbackHandler( [&](){
        /* rotate the directional light */
        dir_light->position.x = 3.0 * cosf( light_rotation_1 );
        dir_light->position.z = 3.0 * sinf( light_rotation_1 );
        
        light_rotation_1 += 0.01;
        
        if( rotate_objects )
            composite->rotateY(-1.0f);

    });
    
    /* Override key callback handler */
    renderer.setKeyCallbackHandler([&](GLFWwindow *window, int key, int scancode, int action, int mod) {
        if( action == GLFW_PRESS ) {
            switch ( key) {
                case GLFW_KEY_ESCAPE: case GLFW_KEY_Q:
                    glfwSetWindowShouldClose( window, GL_TRUE );
                    return;
                    
                case GLFW_KEY_W: { /* Toggle wireframe */
                    sphere->material->wireframe     = !sphere->material->wireframe;
                    cube->material->wireframe       = !cube->material->wireframe;
                    cylinder->material->wireframe   = !cylinder->material->wireframe;
                    break;
                }
                    
                case GLFW_KEY_R: /* Toggle rotation */
                    rotate_objects = !rotate_objects;
                    break;
                    
                default:
                    break;
            }
        }
    });
    
    
    renderer.gammaInput  = true;
    renderer.gammaOutput = true;
    renderer.clearColor = scene->fog->color;
    renderer.render(scene, camera );
    
    return 0;
}



ptr<Object3D> createCompositeObject() {
    
    /*Create a composite object*/
    auto composite = make_shared<Object3D>();
    composite->name = "composite";
    
    /* Main part is a sphere */
    sphere = Mesh::create( SphereGeometry::create(30, 20, 0.66f ),
                           PhongMaterial::create( 0x888888, 0xFFFFFF, 0x333333, 0x222222, 130.0, true ) );
    
    sphere->texture     = TextureUtils::loadAsTexture    ( path + "planets", "earth_atmos_2048.jpg");
    sphere->normalMap   = TextureUtils::loadAsNormalMap  ( path + "planets", "earth_normal_2048.jpg" );
    sphere->specularMap = TextureUtils::loadAsSpecularMap( path + "planets", "earth_specular_2048.jpg" );
    composite->add( sphere );
    
    /* But a cube is attached to the sphere (not to composite directly), thus transformation is relative to sphere */
    cube = Mesh::create( CubeGeometry::create( 1.0f ),
                         PhongMaterial::create( 0xFFFFFF, 0xFFFFFF, 0x000000, 0x111111, 50.0, false ) );
    cube->translate( 2.0, 0.0, 0.0 );
    sphere->add( cube );

    /* Try to add a texture on to it */
    cube->normalMap = TextureUtils::loadAsNormalMap ( path, "four_shapes_normal.tga" );
    
    
    cylinder = Mesh::create( CylinderGeometry::create(0.5, 0.3, 1.0, 30, 5, true),
                             PhongMaterial::create( 0xFFFFFF, 0x101010, 0x000000, 0x111111, 50.0, false ) );
    
    cylinder->translate( -2.0, 0.0, 0.0 );
    cylinder->material->side = three::DoubleSide;
    cylinder->texture   = TextureUtils::loadAsTexture   ( path, "rock_color.tga" );
    cylinder->normalMap = TextureUtils::loadAsNormalMap ( path, "rock_normal.tga" );
    sphere->add( cylinder );
    
    composite->translate(0.0f, 1.5f, 0.0f);
    
    
    return composite;
}
