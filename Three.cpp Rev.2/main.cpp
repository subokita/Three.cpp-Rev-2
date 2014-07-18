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

using namespace std;
using namespace three;

unsigned int sphere_id;
unsigned int cube_id;
unsigned int bounding_box_id;

ptr<Object3D> createCompositeObject();

int main(int argc, const char * argv[])
{
    Renderer renderer;
    renderer.init( "", 1600 * 3 / 4, 900 * 3 / 4 );
    renderer.shaderLib = make_shared<ShaderLib::Shader>(ShaderLib::phong);
    
    /* Create scene */
    auto scene = Scene::create();
    scene->fog = Fog::create( Color(0x72645b / 2), 2.0, 15.0 );
    
    /* Create camera */
    auto camera = PerspectiveCamera::create( 50.0, renderer.aspectRatio, 0.001, 15.0 );
    camera->position = glm::vec3(0.0, 1.5, 5.5);
    camera->lookAt( 0.0, 1.5, 0.0 );
    
    
    /* Add our composite object */
    auto composite = createCompositeObject();
    scene->add( composite );
    
    
    /* Create ground plane */
    auto plane_mesh = Mesh::create( PlaneGeometry::create(20.0f, 1),
                                    MeshPhongMaterial::create(0x101010, 0x666666, 0x000000, 0x101010 ) );
    plane_mesh->rotateX(-90.0);
    scene->add( plane_mesh );
    
    
    /* Create directional light */
    auto dir_light = DirectionalLight::create(Color(0x99CCFF), 1.35, glm::vec3(3.0, 1.0, 3.0) );
    scene->add( dir_light );
    
    
    /* Create an ambient light */
    scene->add( AmbientLight::create(Color(0x101010) ));
    
    /* Create a spot light */
    auto spot_light = SpotLight::create( Color(0xFFFFFF), 3.0, 10.0, 20.0, 1.0 );
    spot_light->translate(0.0f, 5.0f, 0.0f);
    scene->add( spot_light );
    
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
        
        /* Widen the spot light */
        spot_light->angle = Math::clamp(spot_light->angle + 0.01f, 15.0, 300.0);
    });
    
    /* Override key callback handler */
    renderer.setKeyCallbackHandler([&](GLFWwindow *window, int key, int scancode, int action, int mod) {
        if( action == GLFW_PRESS ) {
            switch ( key) {
                case GLFW_KEY_ESCAPE: case GLFW_KEY_Q:
                    glfwSetWindowShouldClose( window, GL_TRUE );
                    return;
                    
                case GLFW_KEY_W: { /* Toggle wireframe */
                    ptr<Mesh> sphere = downcast(composite->getObjectByID(sphere_id, true), Mesh);
                    sphere->material->wireframe = !sphere->material->wireframe;
                    
                    ptr<Mesh> cube = downcast(composite->getObjectByID(cube_id, true), Mesh);
                    cube->material->wireframe = !cube->material->wireframe;
                    
                    break;
                }
                    
                case GLFW_KEY_B: { /* Toggle bounding box visibility */
                    ptr<Mesh> bounding = downcast(composite->getObjectByID(bounding_box_id, true), Mesh);
                    bounding->visible = !bounding->visible;
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
    
    string path = "/Users/saburookita/Desktop/Dropbox/OpenGL-master/Binaries/";
    string filename = "four_shapes_color.tga";
    
    /*Create a composite object*/
    auto composite = make_shared<Object3D>();
    
    /* Main part is a sphere */
    auto sphere = Mesh::create( SphereGeometry::create(30, 15, 0.66f ),
                                MeshPhongMaterial::create( 0xFFFFFF, 0xFFFFFF, 0x000000, 0x111111, 30.0, false ) );
    composite->add( sphere );
    sphere->texture = TextureUtils::loadImageAsTexture( path, filename );
    
    
    /* But a cube is attached to the sphere (not to composite directly), thus transformation is relative to sphere */
    auto cube = Mesh::create( CubeGeometry::create( 1.0f ),
                              MeshPhongMaterial::create( 0xFFFFFF, 0xFFFFFF, 0x000000, 0x111111, 50.0, true ) );
    
    cube->translate( 2.0, 0.0, 0.0 );
    sphere->add( cube );

    /* Try to add a texture on to it */
//    /Users/saburookita/Downloads/mrdoob-three.js-d3cb4e7/examples/textures/UV_Grid_Sm.jpg
//    /Users/saburookita/Desktop/Dropbox/OpenGL-master/Binaries/four_shapes_normal.tga
    cube->texture = TextureUtils::loadImageAsTexture( path, filename );
    
    /* Just for testing, add the bounding box to the composite object, it should cover both sphere and cube */
    auto bound_geom = composite->computeBoundingBox();
    auto bound_mat  = MeshPhongMaterial::create();
    bound_mat->wireframe = true;
    
    auto bound_mesh = Mesh::create( CubeGeometry::create(1.0f), bound_mat );
    bound_mesh->translate( bound_geom->center() - composite->position ) ;
    bound_mesh->scale     = bound_geom->size();
    bound_mesh->visible   = true;
    
    composite->add( bound_mesh );
    composite->translate(0.0f, 1.5f, 0.0f);
    
    sphere_id       = sphere->id;
    cube_id         = cube->id;
    bounding_box_id = bound_mesh->id;
    
    return composite;
}
