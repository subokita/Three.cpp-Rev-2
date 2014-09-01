//
//  Ex_007_PointCloudTest.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 13/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Ex_007_PointCloudTest.h"

using namespace std;

namespace three  {
    
    ptr<Ex_007_PointCloudTest> Ex_007_PointCloudTest::create(){
        return make_shared<Ex_007_PointCloudTest>();
    }
    
    void Ex_007_PointCloudTest::run() {
        const string path = "/Users/saburookita/Personal Projects/Three.cpp Rev.2/examples/assets/";
        
        ForwardRenderer renderer;
        renderer.init( "Ex 007: Point Cloud test", 1600 * 2 / 4, 900 * 2 / 4 );
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
        const string ply_path = "/Users/saburookita/Sandbox/VoxelCarving/";
        const vector<string> filenames = {
            "soldier/soldier.ply",
            "morpheus/morpheus.ply",
            "predator/predator.ply",
        };
        
        const vector<float> scales      = {1.0f, 1.0f, 1.0f};
        const vector<float> x_rotations = {-90.0f, -90.0f, -90.0f};
        const vector<float> y_rotations = {0.0f, -30.0f, -180.0f};
        
        vector<ptr<Mesh>> statues;
        
        float x_offset = -2.5;
        for( int i = 0; i < filenames.size(); i++ ) {
            auto statue = Loader::loadPLY( ply_path, filenames[i], 0
                                          | aiProcess_JoinIdenticalVertices
                                          | aiProcess_GenSmoothNormals );
            
            statue->setMaterial(PhongMaterial::create(0x777777, 0x0, 0x777777, 0x0, 0, true));
            statue->getGeometry()->setScale(scales[i]);
            statue->getGeometry()->rotateX(x_rotations[i]);
            statue->getGeometry()->rotateZ(y_rotations[i]);
            statue->castShadow      = true;
            statue->receiveShadow   = true;
            statue->setPointMode(true);
            
            auto bounding_box = statue->computeBoundingBox();
            
            glm::vec3 size    = bounding_box->size();
            glm::vec3 center  = bounding_box->center();
            statue->translate(x_offset, -(center.y - size.y * 0.5), 0.0);
            
            x_offset += 2.5f;
            scene->add( statue );
            statues.push_back( statue );
        }
        
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
                for( auto statue: statues )
                    statue->rotateY(1.0f);
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