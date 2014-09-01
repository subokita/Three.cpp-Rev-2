//
//  Ex_003_EnvironmentMapping.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 22/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Ex_003_EnvironmentMapping.h"


using namespace std;

namespace three  {
    
    ptr<Ex_003_EnvironmentMapping> Ex_003_EnvironmentMapping::create(){
        return make_shared<Ex_003_EnvironmentMapping>();
    }
    
    void Ex_003_EnvironmentMapping::run() {
        const string path = "/Users/saburookita/Personal Projects/Three.cpp Rev.2/examples/assets/";
        
        ForwardRenderer renderer;
        renderer.init( "Ex 003: Environment Mappings, Cube Maps", 1600 * 2 / 4, 900 * 2 / 4 );
        renderer.setCameraControl(Arcball::create(2.0f));
        
        /* Create scene */
        auto scene = Scene::create();
        scene->setFog(Fog::create( 0x72645b / 2, 2.0, 15.0 ));
        scene->setViewport(0, 0, 1600 * 2 / 4, 900 * 2 / 4);
        
        /* Create camera */
        auto camera = PerspectiveCamera::create( 50.0, renderer.getAspectRatio(), 0.001, 100.0 );
        camera->position = glm::vec3(0.0, 1.0, 5.5);
        camera->lookAt( 0.0, 1.0, 0.0 );
        
        /* A sphere, cube, and cylinder walk into a pub */
        auto sphere = Mesh::create( SphereGeometry::create(30, 20, 0.66f ),
                                   PhongMaterial::create( 0xCCCCCC, 0x0, 0x0, 0x222222, 130.0, true ) );
        
        sphere->setTexture      ( TextureUtils::loadAsTexture    ( path + "planets", "earth_atmos_2048.jpg") );
        sphere->setNormalMap    ( TextureUtils::loadAsNormalMap  ( path + "planets", "earth_normal_2048.jpg" ) );
        sphere->setSpecularMap  ( TextureUtils::loadAsSpecularMap( path + "planets", "earth_specular_2048.jpg" ) );
        
        
        auto sphere_2 = Mesh::create( SphereGeometry::create(30, 20, 0.66f ),
                                     PhongMaterial::create( 0xDD00DD, 0x0, 0x0, 0x222222, 130.0, true ) );
        sphere_2->setNormalMap( TextureUtils::loadAsNormalMap  ( path, "tutorial_normals07.gif" ) );
        sphere_2->translate( 0.0f, 1.66f, 0.0f );
        
        /* Cube with texture and normal map */
        auto cube = Mesh::create( CubeGeometry::create( 1.0f, 4 ),
                                 PhongMaterial::create( 0xCCCCCC, 0x0, 0x0, 0x111111, 150.0, false ) );
        
        cube->setNormalMap( TextureUtils::loadAsNormalMap( path, "four_shapes_normal.tga" ) );
        cube->translate(2.0f, 0.0f, 0.0f);
        
        
        auto cube_2_material = PhongMaterial::create( 0xCCCCCC, 0x0, 0x0, 0x111111, 150.0, false );
        cube_2_material->setRefraction( true );
        cube_2_material->setRefractionRatio( 0.5 );
        
        auto cube_2 = Mesh::create( CubeGeometry::create( 1.0f ), cube_2_material);
        cube_2->translate(2.0f, 1.66f, 0.0f);
        cube_2->getGeometry()->rotateZ(45.0);
        cube_2->getGeometry()->rotateX(-45.0);
        
        /* An open cylinder with double sided rendering */
        auto cylinder = Mesh::create( CylinderGeometry::create(0.5, 0.5, 1.0, 30, 5, true),
                                     PhongMaterial::create( 0xCCCCCC, 0x0, 0x0, 0x111111, 150.0, false ) );
        cylinder->getMaterial()->setSide( SIDE::DOUBLE_SIDE );
        
        cylinder->setTexture  ( TextureUtils::loadAsTexture   ( path, "rock_color.tga" ) );
        cylinder->setNormalMap( TextureUtils::loadAsNormalMap ( path, "rock_normal.tga" ) );
        cylinder->translate(-2.0f, 0.0f, 0.0f);
        
        auto cylinder_geometry = CylinderGeometry::create(0.5, 0.1, 1.0, 30, 5, true);
        cylinder_geometry->rotateX(-90.0);
        
        auto cylinder_2 = Mesh::create( cylinder_geometry,
                                        PhongMaterial::create( 0xCCCCCC, 0x0, 0x0, 0x111111, 150.0, false ) );
        cylinder_2->getMaterial()->setSide( SIDE::DOUBLE_SIDE );
        cylinder_2->setNormalMap( TextureUtils::loadAsNormalMap ( path, "rock_normal.tga" ) );
        cylinder_2->translate(-2.0f, 1.66f, 0.0f);
        
        scene->add( sphere );
        scene->add( sphere_2 );
        scene->add( cube );
        scene->add( cube_2 );
        scene->add( cylinder );
        scene->add( cylinder_2 );
        
        
        auto env = Mesh::create( CubeGeometry::create(20.0f), MeshCubeMapMaterial::create() );
        env->setTexture( TextureUtils::loadAsEnvMap( path + "cube/pisa",
                                                  "nx.png", "ny.png", "nz.png",
                                                  "px.png", "py.png", "pz.png") );
        
        scene->add( env );
        
        sphere->setEnvMap    ( env->getTexture() );
        sphere_2->setEnvMap  ( env->getTexture() );
        cube->setEnvMap      ( env->getTexture() );
        cube_2->setEnvMap    ( env->getTexture() );
        cylinder->setEnvMap  ( env->getTexture() );
        cylinder_2->setEnvMap( env->getTexture() );
        
        
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
                sphere_2->rotateZ(-1.0f);
                cube->rotateY    (-1.0f);
                cube_2->rotateY  ( 1.0f);
                cylinder->rotateX(-1.0f);
                cylinder_2->rotateZ(-1.0f);
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