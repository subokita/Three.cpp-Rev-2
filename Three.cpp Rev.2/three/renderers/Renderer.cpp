//
//  Renderer.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Renderer.h"
#include <exception>
#include <sstream>

#include <glm/gtc/matrix_inverse.hpp>

#include "three.h"

using namespace std;

namespace three {
    Renderer* Renderer::instance = nullptr;
    
    Renderer::Renderer() :
        vertexArrayId( -1 ),
        gammaInput   ( false ),
        gammaOutput  ( false ),
        clearColor   ( 0x000000 ),
        clearAlpha   ( 1.0 )
    {}
    
    Renderer::~Renderer() {
        if( vertexArrayId != -1 )
            glDeleteVertexArrays(1, &vertexArrayId);
        
        glfwDestroyWindow( window );
        window   = nullptr;
        instance = nullptr;
        glfwTerminate();
    }
    
    
    void Renderer::init( std::string window_title, GLuint window_width, GLuint window_height ) {
        width   = window_width;
        height  = window_height;
        aspectRatio = width * 1.0 / height;
        
        if( !glfwInit() )
            throw std::runtime_error( "Unable to init GLFW" );
        
        
        /* Tell GLFW to use OpenGL 4.1 */
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
        glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
        
        window = glfwCreateWindow( window_width, window_height, window_title.c_str(), nullptr, nullptr );
        if(!window) {
            throw std::runtime_error( "Unable to create GLFW window" );
            glfwTerminate();
        }
        
        glfwSetInputMode( window, GLFW_STICKY_KEYS, GL_TRUE );
        glfwMakeContextCurrent( window );
     
        glGenVertexArrays(1, &vertexArrayId );
        glBindVertexArray(vertexArrayId);
        
        initCallbacks();
        
        instance = this;
        arcball.init(width, height, 2.0f);
    }
    
    
    void Renderer::setDefaultGLState() {
        glClearColor( clearColor.rep.x, clearColor.rep.y, clearColor.rep.z, clearAlpha );
        glClearDepth( 1.0 );
        glClearStencil( 0 );
        
        glEnable( GL_DEPTH_TEST );
        glDepthFunc( GL_LEQUAL );
        
        glFrontFace( GL_CCW );
        glCullFace( GL_BACK );
        glEnable( GL_CULL_FACE );
        
        glEnable( GL_BLEND );
        glBlendEquation( GL_FUNC_ADD );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        
        glViewport(0, 0, width, height);
    }
    
    /**
     * Initialize all the necessary callbacks for GLFW
     */
    void Renderer::initCallbacks() {
        errorCallbackHandler = [](int error, const char * desc) {
            cerr << desc << endl;
        };
        
        frameBufferSizeHandler = [](GLFWwindow *window, int width, int height) {
            glViewport(0, 0, width, height);
        };
        
        keyCallbackHandler = [](GLFWwindow *window, int key, int scancode, int action, int mod) {
            if( action == GLFW_PRESS ) {
                switch ( key) {
                    case GLFW_KEY_ESCAPE: case GLFW_KEY_Q:
                        glfwSetWindowShouldClose( window, GL_TRUE );
                        return;
                        
                    default:
                        break;
                }
            }
        };
        
        scrollCallbackHandler = []( GLFWwindow *window, double x, double y ) {

        };
        
        cursorCallbackHandler = []( GLFWwindow *window, double x, double y ) {
            instance->arcball.cursorCallback( window, x, y );
        };
        
        mouseButtonCallbackHandler = []( GLFWwindow * window, int button, int action, int mods ) {
            instance->arcball.mouseButtonCallback( window, button, action, mods );
        };
        
        glfwSetErrorCallback          ( Renderer::errorCallback );
        glfwSetFramebufferSizeCallback( window, Renderer::frameBufferSizeCallback );
        glfwSetKeyCallback            ( window, Renderer::keyCallback );
        glfwSetScrollCallback         ( window, Renderer::scrollCallback );
        glfwSetCursorPosCallback      ( window, Renderer::cursorCallback );
        glfwSetMouseButtonCallback    ( window, Renderer::mouseButtonCallback );
    }
    
    /**
     * This function injects necessary #define preprocessor directives
     * to the the vertex and fragment shaders, depending on the 
     * how the user defines the scene
     */
    void Renderer::addDefinitionsToShader( ptr<Scene> scene ) {
        if( scene->fog != nullptr ) {
            shaderLib->defines.push_back("#define USE_FOG");
            
            if( instance_of(scene->fog, FogExp2))
                shaderLib->defines.push_back("#define FOG_EXP2");
        }
        
        if( gammaInput )
            shaderLib->defines.push_back("#define GAMMA_INPUT");
        
        if( gammaOutput )
            shaderLib->defines.push_back("#define GAMMA_OUTPUT");
        
        stringstream ss;

        ss.str("");
        ss << "#define MAX_DIR_LIGHTS " << static_cast<int>(count_if( scene->directionalLights.begin(), scene->directionalLights.end(), [](ptr<DirectionalLight> light){
            return light->visible;
        }));
        shaderLib->defines.push_back( ss.str() );

        ss.str("");
        ss << "#define MAX_POINT_LIGHTS " << static_cast<int>(count_if( scene->pointLights.begin(), scene->pointLights.end(), [](ptr<PointLight> light){
            return light->visible;
        }));
        shaderLib->defines.push_back( ss.str() );
        
        ss.str("");
        ss << "#define MAX_HEMI_LIGHTS " << static_cast<int>(count_if( scene->hemisphereLights.begin(), scene->hemisphereLights.end(), [](ptr<HemisphereLight> light){
            return light->visible;
        }));
        shaderLib->defines.push_back( ss.str() );
        
        ss.str("");
        ss << "#define MAX_SPOT_LIGHTS " << static_cast<int>(count_if( scene->spotLights.begin(), scene->spotLights.end(), [](ptr<SpotLight> light){
            return light->visible;
        }));
        shaderLib->defines.push_back( ss.str() );
        
        
        shaderLib->defines.push_back( "#define USE_MAP" );
        shaderLib->defines.push_back( "#define USE_NORMALMAP" );
    }
    
    
    void Renderer::setAmbientLights(){
        /* Setting ambient light */
        if( scene->ambientLight == nullptr )
            shader->setUniform( "ambient_light_color", Color(0x000000), 1.0, gammaInput );
        else {
            if( scene->ambientLight->visible )
                shader->setUniform( "ambient_light_color", scene->ambientLight->color, 1.0, gammaInput );
        }
    }
    
    void Renderer::setDirectionalLights(){
        size_t size = count_if( scene->directionalLights.begin(), scene->directionalLights.end(),
        [](ptr<DirectionalLight> light){
            return light->visible;
        });
        
        if( size == 0 )
            return;
        
        vector<glm::vec3> directions;
        vector<Color> colors;
        vector<float> intensities;
        
        for( ptr<DirectionalLight> light : scene->directionalLights ) {
            if( !light->visible )
                continue;
            
            directions.push_back ( light->position );
            colors.push_back     ( light->color );
            intensities.push_back( light->intensity );
        }
        
        shader->setUniform( "directional_light_direction", directions );
        shader->setUniform( "directional_light_color", colors, intensities, gammaInput );
    }
    
    void Renderer::setSpotLights(){
        size_t size = count_if( scene->spotLights.begin(), scene->spotLights.end(),
        [](ptr<SpotLight> light){
           return light->visible;
        });

        if( size == 0 )
            return;
        
        vector<Color> colors;
        vector<float> intensities;
        vector<glm::vec3> positions;
        vector<glm::vec3> directions;
        vector<float> cos_angles;
        vector<float> exponents;
        vector<float> distances;
        
        for( ptr<SpotLight> light: scene->spotLights ){
            colors.push_back     (light->color);
            intensities.push_back(light->intensity);
            positions.push_back  (light->position);
            directions.push_back (glm::normalize(light->position - light->target));
            cos_angles.push_back (cosf(Math::degToRad(light->angle)));
            exponents.push_back  (light->exponent);
            distances.push_back  (light->distance);
        }
        
        
        shader->setUniform( "spot_light_color", colors, intensities, gammaInput );
        shader->setUniform( "spot_light_position", positions );
        shader->setUniform( "spot_light_direction", directions );
        
        shader->setUniform( "spot_light_angle_cos", cos_angles );
        shader->setUniform( "spot_light_exponent", exponents );
        shader->setUniform( "spot_light_distance", distances );
    }
    
    void Renderer::setHemisphereLights(){
        size_t size = count_if( scene->hemisphereLights.begin(), scene->hemisphereLights.end(),
        [](ptr<HemisphereLight> light){
           return light->visible;
        });
        if( size == 0 )
            return;
        
        vector<Color> sky_colors;
        vector<Color> ground_colors;
        vector<float> intensities;
        vector<glm::vec3> directions;
        
        for( ptr<HemisphereLight> light: scene->hemisphereLights ){
            sky_colors.push_back   (light->color);
            ground_colors.push_back(light->groundColor);
            intensities.push_back  (light->intensity);
            directions.push_back   (light->position);
        }
        
        shader->setUniform( "hemisphere_light_sky_color", sky_colors, intensities, gammaInput );
        shader->setUniform( "hemisphere_light_ground_color", ground_colors, intensities, gammaInput );
        shader->setUniform( "hemisphere_light_direction", directions );
    }
    
    void Renderer::setPointLights() {
        size_t size = count_if( scene->pointLights.begin(), scene->pointLights.end(),
        [](ptr<PointLight> light){
           return light->visible;
        });
        if( size == 0 )
            return;
        
        vector<Color> colors;
        vector<glm::vec3> positions;
        vector<float> intensities;
        vector<float> distances;
        
        for( ptr<PointLight> light: scene->pointLights ) {
            colors.push_back     (light->color);
            intensities.push_back(light->intensity);
            positions.push_back  (light->position);
            distances.push_back  (light->distance);
        }
        
        shader->setUniform( "point_light_color", colors, intensities, gammaInput );
        shader->setUniform( "point_light_position", positions );
        shader->setUniform( "point_light_distance", distances );
    }
    
    void Renderer::setFog() {
        /* Setting fog */
        if( scene->fog != nullptr ) {
            if( instance_of(scene->fog, Fog)) {
                ptr<Fog> fog = downcast(scene->fog, Fog);
                shader->setUniform( "fog_color", fog->color, 1.0, gammaInput );
                shader->setUniform( "fog_near", fog->near );
                shader->setUniform( "fog_far",  fog->far );
            }
            else if( instance_of(scene->fog, FogExp2)) {
                ptr<FogExp2> fog = downcast(scene->fog, FogExp2);
                shader->setUniform( "fog_color", fog->color, 1.0, gammaInput );
                shader->setUniform( "fog_density", fog->distance );
            }
        }

    }
    
    void Renderer::drawObjects(ptr<Object3D> object) {
        for( auto entry : object->children ) {
            ptr<Object3D> child = entry.second;
            
            if( !child->visible )
                continue;
            
            child->updateMatrixWorld(false);
            glm::mat4 mv = camera->matrix * arcball.createViewRotationMatrix() * child->matrixWorld;
            shader->setUniform( "normal_mat", glm::inverseTranspose( glm::mat3(mv) ) );
            shader->setUniform( "model_view_mat", mv );
            
            if( instance_of(child, Mesh ) ) {
                ptr<Mesh> mesh = downcast(child, Mesh) ;
                mesh->draw(shader, gammaInput);
            }
            
            drawObjects( child );
        }
    }
    

    void Renderer::drawObjects() {
        
        shader->setUniform( "projection_mat", camera->projection );
        shader->setUniform( "view_mat", camera->matrix * arcball.createViewRotationMatrix() );
        shader->setUniform( "camera_pos_w", camera->position );
        
        drawObjects( scene );
    }
    
    void Renderer::initObjects( ptr<Object3D> object ) {
        for( auto entry : object->children ) {
            ptr<Object3D> child = entry.second;
            
            if( instance_of(child, Mesh ) ) {
                ptr<Mesh> mesh = downcast(child, Mesh) ;
                if( mesh->geometry != nullptr && !mesh->geometry->glBuffersInitialized )
                    mesh->geometry->initGLBuffers();
            }
            
            initObjects( child );
        }
    }
    
    void Renderer::render(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera) {
        if( scene == nullptr || camera == nullptr )
            throw runtime_error( "Unable to render scene without a valid scene and camera" );
        
        this->scene = scene;
        this->camera = camera;
        
        setDefaultGLState();
        
        initObjects( scene );
        
        if( shaderLib != nullptr ) {
            addDefinitionsToShader( scene );
            
            if( shader == nullptr )
                shader = Shader::create(*shaderLib);
            else
                shader->initFromSource( *shaderLib );
        }
        
        
        while( !glfwWindowShouldClose( window )) {
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
            
            camera->updateMatrix();
            
            renderPlugins( preRenderPlugins, scene, camera );
            
            shader->bind();
            
                setAmbientLights();
                setHemisphereLights();
                setFog();
                setDirectionalLights();
                setPointLights();
                setSpotLights();
                
                drawObjects();
                
                glfwSwapBuffers( window );
                glfwPollEvents();
            
            shader->unbind();
            
            renderPlugins( postRenderPlugins, scene, camera );
            
            postRenderCallback();
        }
        
    }
    
    
    void Renderer::renderPlugins( std::vector<ptr<RenderPlugin>>& plugins, ptr<Scene> scene, ptr<Camera> camera ) {
        if( plugins.empty() )
            return;
        
        for( ptr<RenderPlugin> plugin: plugins ) {
            plugin->render( scene, camera );
        }
    }
    
    void Renderer::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mod) {
        instance->keyCallbackHandler( window, key, scancode, action, mod );
    }

    void Renderer::frameBufferSizeCallback( GLFWwindow *window, int width, int height ) {
        instance->frameBufferSizeHandler(window, width, height );
    }
    
    void Renderer::errorCallback( int error, const char * desc ) {
        instance->errorCallbackHandler(error, desc);
    }
    
    void Renderer::scrollCallback( GLFWwindow *window, double x, double y ) {
        instance->scrollCallbackHandler(window, x, y);
    }
    
    void Renderer::mouseButtonCallback( GLFWwindow * window, int button, int action, int mods ){
        instance->mouseButtonCallbackHandler(window, button, action, mods );
    }
    
    void Renderer::cursorCallback( GLFWwindow *window, double x, double y ) {
        instance->cursorCallbackHandler(window, x, y);
    }
    
    
    void Renderer::setPostRenderCallbackHandler( function<void()> handler ) {
        postRenderCallback = handler;
    }
    
    void Renderer::setFrameBufferSizeHandler( function<void(GLFWwindow*, int, int)> handler ) {
        frameBufferSizeHandler = handler;
    }
    
    void Renderer::setKeyCallbackHandler( function<void(GLFWwindow*, int, int, int, int)> handler ) {
        keyCallbackHandler = handler;
    }
    
    void Renderer::setErrorCallbackHandler( function<void(int, const char*)> handler) {
        errorCallbackHandler = handler;
    }
    
    void Renderer::setScrollCallbackHandler( function<void(GLFWwindow*, double, double)> handler ){
        scrollCallbackHandler = handler;
    }
    
    void Renderer::setMouseButtonCallbackHandler( function<void(GLFWwindow*, int, int, int)> handler ){
        mouseButtonCallbackHandler = handler;
    }
    
    void Renderer::setCursorCallbackHandler( function<void(GLFWwindow*, double, double)> handler ) {
        cursorCallbackHandler = handler;
    }
}