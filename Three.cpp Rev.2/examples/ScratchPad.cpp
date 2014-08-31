//
//  ScratchPad.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 19/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "ScratchPad.h"
#include <glm/gtx/quaternion.hpp>

using namespace std;

namespace three  {
    
    ptr<ScratchPad> ScratchPad::create(){
        return make_shared<ScratchPad>();
    }
    
    void ScratchPad::run() {
        const string path = "/Users/saburookita/Personal Projects/Three.cpp Rev.2/examples/assets/";
        
        Renderer renderer;
        renderer.init( "Scratch Pad", 1600 * 2 / 4, 900 * 2 / 4 );
        renderer.setCameraControl(Arcball::create(2.0f));
        
        /* First load the fonts */
        renderer.addFont("droid-bold",      path + "fonts/DroidSerif-Bold.ttf");
        

        /* Create scene */
        auto scene = Scene::create();
        scene->setFog(Fog::create( 0x72645b, 2.0, 100000.0 ));
        scene->setViewport( 0.0, 0.0, renderer.getWidth(), renderer.getHeight() );
        scene->setShadowMapType( SHADOW_MAP::PCF_SOFT );
        
        /* Create camera */
        auto camera = PerspectiveCamera::create( 50.0, renderer.getAspectRatio(), 0.001, 100000.0 );
        camera->translate(0.0, 1.5, 5.5);
        camera->lookAt( 0.0, 1.0, 0.0 );
        
        /* Load our ply models */
        const string ply_path = "/Users/saburookita/Sandbox/VoxelCarving/";
        const vector<string> filenames = {
            "morpheus/morpheus.ply",
        };
        
        vector<ptr<Mesh>> statues;
        
        for( int i = 0; i < filenames.size(); i++ ) {
            auto statue = Loader::loadPLY( ply_path, filenames[i], 0
                                          | aiProcess_JoinIdenticalVertices
                                          | aiProcess_GenSmoothNormals );
            
            statue->setMaterial(PhongMaterial::create(0x777777, 0x0, 0x777777, 0x0, 0, true));
            statue->getGeometry()->rotateX(-90.0f);
            statue->castShadow      = true;
            statue->receiveShadow   = true;
            statue->setPointMode(true);
            
            auto bounding_box = statue->computeBoundingBox();
            
            glm::vec3 size    = bounding_box->size();
            glm::vec3 center  = bounding_box->center();
            cout << center << endl;
            cout << size << endl;
            
            statue->translate( 0.0, -(center.y - size.y * 0.5), 0.0);
            
            scene->add( statue );
            statues.push_back( statue );
            
            camera->translate(center.x, center.y, size.x );
            camera->lookAt( center );
        }
        
        ptr<Mesh> cam_helper = CameraHelper::create(glm::vec3(0.0, 1.5, -5.5), glm::vec3(0.0));
        scene->add( cam_helper );
        
        /* Add the ground plane */
        auto plane = Mesh::create( PlaneGeometry::create(50.0f), PhongMaterial::create() );
        plane->rotateX(-90.0f);
        plane->receiveShadow = true;
        scene->add( plane );
        
        /* Cubemap */
        auto env = Mesh::create( CubeGeometry::create(50.0f), MeshCubeMapMaterial::create() );
        env->setTexture( TextureUtils::loadAsEnvMap( path + "cube/pisa",
                                                    "nx.png", "ny.png", "nz.png",
                                                    "px.png", "py.png", "pz.png"));
        scene->add( env );
        
        
        /* Create a (rotating) directional light */
        auto dir_light = DirectionalLight::create(0x99CCFF, 1.35, glm::vec3( 3.0, 1.0, 3.0 ) );
        dir_light->castShadow       = true;
        dir_light->shadowBias       = -0.001;
        dir_light->shadowCameraNear = -10.0;
        dir_light->shadowCameraFar  =  10.0;
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
                statues[0]->rotateY(1.0f);
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