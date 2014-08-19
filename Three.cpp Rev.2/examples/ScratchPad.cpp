//
//  ScratchPad.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 19/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "ScratchPad.h"

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
        
        /*Then write on screen*/
        renderer.addText( "Test Torus", 40, 40, "droid-bold", 0xFF0000, 20.0f );

        /* Create scene */
        auto scene = Scene::create();
        scene->setFog(Fog::create( 0x72645b, 2.0, 15.0 ));
        scene->setViewport( 0.0, 0.0, renderer.getWidth(), renderer.getHeight() );
        scene->setShadowMapType( SHADOW_MAP::PCF_SOFT );
        
        /* Create camera */
        auto camera = PerspectiveCamera::create( 50.0, renderer.getAspectRatio(), 0.001, 100.0 );
        camera->translate(0.0, 1.5, 5.5);
        camera->lookAt( 0.0, 1.0, 0.0 );
        
        auto torus = Mesh::create( TorusGeometry::create(0.5, 0.25, 40, 30), PhongMaterial::create( 0xFF00FF, 0x0, 0x0, 0x222222, 10.0, true ) );
        torus->translate( -4.0f, 0.75f, 0.0f );
        torus->receiveShadow = true;
        torus->castShadow    = true;
        scene->add(torus);
        
        auto octahedron = Mesh::create( OctahedronGeometry::create(1.0), PhongMaterial::create(0x00FFFF, 0x0, 0x0, 0222222, 1.0, true));
        octahedron->translate(-2.0f, 1.0f, 0.0f);
        octahedron->receiveShadow = true;
        octahedron->castShadow    = true;
        scene->add(octahedron);
        
        auto tetrahedron = Mesh::create( TetrahedronGeometry::create(1.0), PhongMaterial::create(0x0000FF, 0x0, 0x0, 0222222, 1.0, true));
        tetrahedron->translate(0.0f, 1.0f, 0.0f);
        tetrahedron->receiveShadow = true;
        tetrahedron->castShadow    = true;
        scene->add(tetrahedron);
        
        
        auto icosahedron = Mesh::create( IcosahedronGeometry::create(1.0), PhongMaterial::create(0xFF00FF, 0x0, 0x0, 0222222, 1.0, true));
        icosahedron->translate(2.0f, 1.0f, 0.0f);
        icosahedron->receiveShadow = true;
        icosahedron->castShadow    = true;
        scene->add(icosahedron);
        
        
        auto plane = Mesh::create( PlaneGeometry::create(20.0f),
                                  PhongMaterial::create(0x777777, 0x777777, 0x0, 0x999999, 30) );
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
                torus->rotateY(1.0f);
                octahedron->rotateY(-1.0f);
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