//
//  Ex_005_PLYLoading.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 04/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Ex_005_PLYLoading.h"


using namespace std;

namespace three  {
    
    ptr<Ex_005_PLYLoading> Ex_005_PLYLoading::create(){
        return make_shared<Ex_005_PLYLoading>();
    }
    
    void Ex_005_PLYLoading::run() {
        const string path = "/Users/saburookita/Personal Projects/Three.cpp Rev.2/examples/assets/";
        
        ForwardRenderer renderer;
        renderer.init( "Ex 005: Loading PLY objects", 1600 * 2 / 4, 900 * 2 / 4 );
        renderer.setCameraControl(Arcball::create(2.0f));
        
        /* Create scene */
        auto scene = Scene::create();
        scene->setFog(Fog::create( 0x72645b / 2, 2.0, 15.0 ));
        scene->setViewport( 0.0, 0.0, renderer.getWidth(), renderer.getHeight() );
        scene->setShadowMapType( SHADOW_MAP::PCF_SOFT );
        
        /* Create camera */
        auto camera = PerspectiveCamera::create( 50.0, renderer.getAspectRatio(), 0.001, 100.0 );
        camera->translate(0.0, 1.5, 5.5);
        camera->lookAt( 0.0, 1.0, 0.0 );
        
        /* Load our ply models */
        vector<string> filenames = {
            "dragon_vrip_res3.ply",
            "happy_vrip_res3.ply",
        };
        
        vector<ptr<Mesh>> statues;
        
        float x_offset = -1.0;
        for( string filename: filenames ) {
            auto statue = Loader::loadPLY(path + "/ply models/", filename,
                                          aiProcess_JoinIdenticalVertices |
                                          aiProcess_GenSmoothNormals | aiProcess_FlipWindingOrder );
            
            auto material = PhongMaterial::create(0xF2BA5A, 0x0, 0x777777, 0x0, 20, true);
            material->setReflectivity(0.7);
            material->setMetal(false);
            
            
            statue->setMaterial(material);
            statue->getGeometry()->setScale(10.f);
            statue->castShadow      = true;
            statue->receiveShadow   = true;
            
            auto bounding_box = statue->computeBoundingBox();
            glm::vec3 size    = bounding_box->size();
            glm::vec3 center  = bounding_box->center();
            statue->translate(x_offset, -(center.y - size.y * 0.5), 0.0);
            
            
            x_offset += 2.0f;
            scene->add( statue );
            statues.push_back( statue );
        }
        
        /* Create our objects */
        auto sphere = Mesh::create( SphereGeometry::create(30, 20, 0.66f ),
                                   PhongMaterial::create(0x777777, 0x0, 0x0, 0x999999, 30, true) );
        
        sphere->setNormalMap( TextureUtils::loadAsNormalMap  ( path, "tutorial_normals07.gif" ) );
        sphere->translate( 2.0, 0.66f, 1.0);
        sphere->castShadow = true;
        sphere->receiveShadow = true;
        scene->add( sphere );
        
        
        auto box = Mesh::create( CubeGeometry::create(3.0f, 3.0f, 3.0f),
                                PhongMaterial::create(0x777777, 0x777777, 0x0, 0x0, 0.0, true) );
        box->setTexture( TextureUtils::loadAsTexture( path, "crate.tga") );
        box->translate( -1.0f, 1.5f, -4.0f );
        box->castShadow = true;
        box->receiveShadow = true;
        scene->add( box );
        
        
        /* And the ground plane */
        auto plane = Mesh::create( PlaneGeometry::create(50.0f), PhongMaterial::create() );
        plane->rotateX(-90.0f);
        plane->receiveShadow = true;
        scene->add( plane );
        
        /* Cubemap */
        auto env = Mesh::create( CubeGeometry::create(50.0f), MeshCubeMapMaterial::create() );
        env->setTexture( TextureUtils::loadAsEnvMap( path + "cube/pisa",
                                                    "nx.png", "ny.png", "nz.png",
                                                    "px.png", "py.png", "pz.png"));
        
        statues[0]->setEnvMap( env->getTexture() );
        statues[1]->setEnvMap( env->getTexture() );
        sphere->setEnvMap( env->getTexture() );
        scene->add( env );
        
        
        /* Create a (rotating) directional light */
        auto dir_light = DirectionalLight::create(0x99CCFF, 1.35, glm::vec3( 3.0, 1.0, 3.0 ) );
        dir_light->castShadow       = true;
        dir_light->shadowBias       = -0.01;
        dir_light->shadowCameraNear = -10.0;
        dir_light->shadowCameraFar  =  10.0;
        dir_light->shadowMapSize    = glm::vec2(1024, 1024);
        scene->add( dir_light );
        
        /* Create a spotlight, the shadow should be casted no the left hand side */
        auto spot_light = SpotLight::create(0x99CCFF, 1.0, 20.0, 50.0, 1.0 );
        spot_light->position = glm::vec3(3.0, 2.0, 3.0);
        spot_light->castShadow = true;
        scene->add( spot_light );
        
        /* Create an ambient light */
        scene->add( AmbientLight::create(0x777777));
        
        /* Create a post render callback for objects rotation */
        bool rotate_objects = false;
        float light_rotation_1 = 0.0;
        renderer.setPostRenderCallbackHandler( [&](){
            dir_light->position.x = ( 3.0 * cosf( light_rotation_1 ) );
            dir_light->position.z = ( 3.0 * sinf( light_rotation_1 ) );
            
            light_rotation_1 += 0.01;
            
            if( rotate_objects ) {
                for( auto statue: statues )
                    statue->rotateY(1.0f);
            }
        });
        
        /* Override key callback handler */
        renderer.setKeyCallbackHandler([&](GLFWwindow *window, int key, int scancode, int action, int mod) {
            if( action == GLFW_PRESS ) {
                switch ( key) {
                    case GLFW_KEY_F:
                        for( auto statue: statues )
                            statue->getMaterial()->setPolygonMode( POLYGON_MODE::POLYGON );
                        sphere->getMaterial()->setPolygonMode( POLYGON_MODE::POLYGON );
                        break;
                        
                    case GLFW_KEY_P:
                        for( auto statue: statues ) {
                            statue->getMaterial()->setPolygonMode( POLYGON_MODE::POINT );
                        }
                        sphere->getMaterial()->setPolygonMode( POLYGON_MODE::POINT );
                        break;
                        
                    case GLFW_KEY_W:
                        for( auto statue: statues ) {
                            statue->getMaterial()->setPolygonMode( POLYGON_MODE::WIREFRAME );
                        }
                        sphere->getMaterial()->setPolygonMode( POLYGON_MODE::WIREFRAME );
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
    }
}