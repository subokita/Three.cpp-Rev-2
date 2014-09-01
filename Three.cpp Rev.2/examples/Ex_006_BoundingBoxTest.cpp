//
//  Ex_006_BoundingBoxTest.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 06/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Ex_006_BoundingBoxTest.h"

using namespace std;

namespace three  {
    
    ptr<Ex_006_BoundingBoxTest> Ex_006_BoundingBoxTest::create(){
        return make_shared<Ex_006_BoundingBoxTest>();
    }
    
    void Ex_006_BoundingBoxTest::run() {
        const string path = "/Users/saburookita/Personal Projects/Three.cpp Rev.2/examples/assets/";
        
        ForwardRenderer renderer;
        renderer.init( "Ex 006: Bounding Box Tests", 1600 * 2 / 4, 900 * 2 / 4 );
        renderer.setCameraControl(Arcball::create(2.0f));
        
        /* Create scene */
        auto scene = Scene::create();
        scene->setFog(Fog::create( 0x72645b / 2, 2.0, 15.0 ));
        scene->setViewport( 0.0, 0.0, renderer.getWidth(), renderer.getHeight() );
        scene->setShadowMapType( SHADOW_MAP::PCF );
        
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
                                          aiProcess_Triangulate |
                                          aiProcess_OptimizeMeshes |
                                          aiProcess_JoinIdenticalVertices |
                                          aiProcess_GenSmoothNormals | aiProcess_FlipWindingOrder );
            
            statue->setMaterial(PhongMaterial::create(0xcccccc, 0x0, 0x000000, 0x999999, 10, true));
            statue->getGeometry()->setScale(10.0f);
            statue->castShadow      = true;
            statue->receiveShadow   = true;
            
            auto bbox = statue->computeBoundingBox();
            glm::vec3 center = bbox->center();
            glm::vec3 size   = bbox->size();
            statue->translate(x_offset, -(center.y - size.y * 0.5), 0.0);
            
            cout << *bbox << endl;
            
            x_offset += 2.0f;
            scene->add( statue );
            statues.push_back( statue );
        }
        
        auto box = Mesh::create( CubeGeometry::create(1.0f, 3),
                                PhongMaterial::create(0x777777, 0x777777, 0x0, 0x0, 0.0, true) );
        box->setTexture( TextureUtils::loadAsTexture( path, "crate.tga") );
        box->getGeometry()->rotateX(45.0f);
        box->getGeometry()->setScale(1.5f);
        box->translate(3.0f, 0.5, 0.0);
        box->castShadow = true;
        box->receiveShadow = true;
        scene->add( box );
        
        auto cylinder = Mesh::create( CylinderGeometry::create(0.5, 0.5, 1.0, 30, 5, false),
                                      PhongMaterial::create( 0xDDDDDD, 0x0, 0x0, 0x111111, 150.0, true )
                                     );
        cylinder->translate(-3.0f, 0.5f, 0.0f);
        cylinder->castShadow = true;
        cylinder->receiveShadow = true;
        scene->add( cylinder );
        
        
        /* And the ground plane */
        auto plane = Mesh::create( PlaneGeometry::create(50.0f), PhongMaterial::create() );
        plane->rotateX(-90.0f);
        plane->receiveShadow = true;
        scene->add( plane );

        /* Cubemap */
//        auto env = Mesh::create( CubeGeometry::create(50.0f), MeshCubeMapMaterial::create() );
//        env->setTexture( TextureUtils::loadAsEnvMap( path + "cube/pisa",
//                                                    "nx.png", "ny.png", "nz.png",
//                                                    "px.png", "py.png", "pz.png"));
//        
//        cylinder->setEnvMap( env->getTexture() );
//        scene->add( env );
        
        
        /* Create a (rotating) directional light */
        auto dir_light = DirectionalLight::create(0x99CCFF, 1.35, glm::vec3( 3.0, 1.0, 3.0 ) );
        dir_light->castShadow       = true;
        dir_light->shadowBias       = -0.0001;
        dir_light->shadowCameraNear = -10.0;
        dir_light->shadowCameraFar  =  10.0;
        dir_light->shadowMapSize    = glm::vec2(512);
        scene->add( dir_light );
        
        /* Create a spotlight, the shadow should be casted no the left hand side */
        auto spot_light = SpotLight::create(0x99CCFF, 1.0, 20.0, 50.0, 1.0 );
        spot_light->position = glm::vec3(3.0, 2.0, 3.0);
        spot_light->castShadow = true;
        scene->add( spot_light );
        
        
        /* Create an ambient light */
        scene->add( AmbientLight::create(0xCCCCCC));
        
        /* Create a post render callback for objects rotation */
        bool rotate_objects = false;
        float light_rotation_1 = 0.0;
        renderer.setPostRenderCallbackHandler( [&](){
            dir_light->position.x = ( 3.0 * cosf( light_rotation_1 ) );
            dir_light->position.z = ( 3.0 * sinf( light_rotation_1 ) );
            
            light_rotation_1 += 0.01;
            
            if( rotate_objects ) {
                box->rotateY(-1.0f);
                statues[0]->rotateY(-0.5f);
            }
        });
        
        
        renderer.setMouseButtonCallbackHandler([&] (GLFWwindow *, int button, int action, int mod){
            auto descendants = scene->getDescendants();
            /* Reset the color first */
            for( auto obj: descendants ) {
                if( instance_of(obj, Mesh)) {
                    auto mesh = downcast( obj, Mesh );
                    if( instance_of(mesh->getMaterial(), PhongMaterial)){
                        auto phong = downcast(mesh->getMaterial(), PhongMaterial);
                        phong->setDiffuseColor( 0xDDDDDD );
                    }
                }
            }
            
            if( action == GLFW_PRESS ) {
                auto raycaster = Projector::pickingRay(renderer.getCursorPosition(), camera);
                
                /* Upon selected / ray picked, change the diffuse color to red */
                for( auto obj: descendants ) {
                    if( instance_of(obj, Geometry))
                        continue;
                    
                    auto bound = obj->getBoundingBox();
                    
                    if(raycaster->ray->intersects(bound)) {
                        if( instance_of(obj, Mesh)) {
                            auto mesh = downcast( obj, Mesh );
                            
                            if( instance_of(mesh->getMaterial(), PhongMaterial)){
                                auto phong = downcast(mesh->getMaterial(), PhongMaterial);
                                phong->setDiffuseColor( 0xDD0000 );
                            }
                        }
                    }
                }
            }
        });
        
        /* Override key callback handler */
        renderer.setKeyCallbackHandler([&](GLFWwindow *, int key, int scancode, int action, int mod) {
            if( action == GLFW_PRESS ) {
                switch ( key) {
                    case GLFW_KEY_R: /* Toggle rotation */
                        rotate_objects = !rotate_objects;
                        break;
                        
                    default: break;
                }
            }
        });
        
        renderer.setGamma( true, true );
        
        renderer.setClearColor( scene->getFog()->getColor() );
        renderer.render(scene, camera );
    }
}