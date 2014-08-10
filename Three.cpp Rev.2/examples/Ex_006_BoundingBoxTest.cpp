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
        
        Renderer renderer;
        renderer.init( "Ex 006: Bounding Box Tests", 1600 * 2 / 4, 900 * 2 / 4 );
        renderer.setCameraControl(Arcball::create(2.0f));
        
        /* Create scene */
        auto scene = Scene::create();
        scene->setFog(Fog::create( 0x72645b / 2, 2.0, 15.0 ));
        scene->setViewport( 0.0, 0.0, renderer.getWidth(), renderer.getHeight() );
        scene->setShadowMapType( SHADOW_MAP::PCF );
        
        /* Create camera */
        auto camera = PerspectiveCamera::create( 50.0, renderer.getAspectRatio(), 0.001, 100.0 );
        camera->translate(0.0, 0.0, 5.5);
        camera->lookAt( 0.0, 0.0, 0.0 );
        
        /* Load our ply models */
        vector<string> filenames = {
            "dragon_vrip_res3.ply",
            "happy_vrip_res3.ply",
        };
        
        vector<ptr<Mesh>> statues;
        
//        float x_offset = -1.0;
//        for( string filename: filenames ) {
//            auto statue = Loader::loadPLY(path + "/ply models/", filename,
//                                          aiProcess_JoinIdenticalVertices |
//                                          aiProcess_GenSmoothNormals | aiProcess_FlipWindingOrder );
//            
//            statue->setMaterial(PhongMaterial::create(0xcccccc, 0x0, 0x000000, 0x999999, 10, true));
//            statue->getGeometry()->setScale(10.0f);
//            statue->castShadow      = true;
//            statue->receiveShadow   = true;
//            
//            auto bounding_box = statue->computeBoundingBox();
//            glm::vec3 size    = bounding_box->size();
//            glm::vec3 center  = bounding_box->center();
//            statue->translate(x_offset, -(center.y - size.y * 0.5), 0.0);
//            
//            x_offset += 2.0f;
//            scene->add( statue );
//            statues.push_back( statue );
//            
//            auto s = statue->computeBoundingSphere();
//            auto bound = Mesh::create(SphereGeometry::create(30, 15, s->radius), PhongMaterial::create());
//            bound->translate(s->center);
//            bound->setWireframeMode(true);
//            scene->add( bound );
//        }
//        
//        
//        auto origin = Mesh::create( SphereGeometry::create(15, 10, 0.1f), PhongMaterial::create() );
//        origin->setWireframeMode(true);
//        origin->translate(0.0, 1.0, 0.0);
//        scene->add(origin);
//        
//        /* Create our objects */
        auto sphere = Mesh::create( SphereGeometry::create(30, 20, 0.11f ),
                                   PhongMaterial::create(0x777777, 0x0, 0x0, 0x999999, 30, true) );
        sphere->setWireframeMode(true);
        sphere->castShadow = true;
        sphere->receiveShadow = true;
        scene->add( sphere );
        
        
        auto box = Mesh::create( CubeGeometry::create(1.0f, 3),
                                PhongMaterial::create(0x777777, 0x777777, 0x0, 0x0, 0.0, true) );
        box->setTexture( TextureUtils::loadAsTexture( path, "crate.tga") );
        box->name = "box";
        box->getGeometry()->rotateX(45.0f);
        box->getGeometry()->setScale(1.0f);
        box->translate(2.0f, 0.0, 0.0);
        box->castShadow = true;
        box->receiveShadow = true;
        scene->add( box );
        
        auto cylinder = Mesh::create( CylinderGeometry::create(0.5, 0.5, 1.0, 30, 5, false),
                                      PhongMaterial::create( 0xCCCC00, 0x0, 0x0, 0x111111, 150.0, true ) );
        cylinder->name = "cylinder";
        cylinder->translate(-2.0f, 0.0f, 0.0f);
        cylinder->castShadow = true;
        cylinder->receiveShadow = true;
        scene->add( cylinder );
        
        
        /* And the ground plane */
        auto plane = Mesh::create( PlaneGeometry::create(50.0f), PhongMaterial::create() );
        plane->name = "plane";
        plane->rotateX(-90.0f);
        plane->receiveShadow = true;
//        scene->add( plane );
//
//        /* Cubemap */
//        auto env = Mesh::create( CubeGeometry::create(50.0f), MeshCubeMapMaterial::create() );
//        env->setTexture( TextureUtils::loadAsEnvMap( path + "cube/pisa",
//                                                    "nx.png", "ny.png", "nz.png",
//                                                    "px.png", "py.png", "pz.png"));
//        
//        sphere->setEnvMap( env->getTexture() );
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
//        scene->add( spot_light );
        
        
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
            }
        });
        
        renderer.setCursorCallbackHandler([&](GLFWwindow *, double x, double y){
        });
        
        renderer.setMouseButtonCallbackHandler([&] (GLFWwindow *, int button, int action, int mod){
            if( action == GLFW_PRESS ) {
                glm::vec3 vec = renderer.getCursorPosition();
                
                glm::vec4 ray_start(vec.x, vec.y, -1.0, 1.0);
                glm::vec4 ray_end  (vec.x, vec.y,  0.0, 1.0);
                glm::mat4 inverse_vp = glm::inverse(camera->getProjectionMatrix() * camera->matrix);
                
                ray_start = inverse_vp * ray_start;
                ray_end   = inverse_vp * ray_end;
                ray_start /= ray_start.w;
                ray_end   /= ray_end.w;

                glm::vec3 origin    = glm::vec3(ray_start);
                glm::vec3 direction = glm::normalize( glm::vec3( ray_end - ray_start ) );
                ptr<Ray> ray = Ray::create(origin, direction);
                
                
                auto descendants = scene->getDescendants();
                for( auto obj: descendants ) {
                    if( instance_of(obj, Geometry))
                        continue;
                    
                    if(ray->intersects(obj->computeBoundingSphere()))
                        cout << obj->name << endl;
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