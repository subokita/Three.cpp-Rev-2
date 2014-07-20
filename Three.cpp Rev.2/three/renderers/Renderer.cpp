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
        arcball = Arcball::create(width, height, 2.0f);
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
            glm::vec3 pos = instance->camera->position * instance->camera->quaternion;
            glm::vec3 dir = glm::normalize(instance->camera->target + pos) * static_cast<float>(y);
            instance->camera->position = (pos - dir);
        };
        
        cursorCallbackHandler = []( GLFWwindow *window, double x, double y ) {
            instance->arcball->cursorCallback( window, x, y );
        };
        
        mouseButtonCallbackHandler = []( GLFWwindow * window, int button, int action, int mods ) {
            instance->arcball->mouseButtonCallback( window, button, action, mods );
        };
        
        glfwSetErrorCallback          ( Renderer::errorCallback );
        glfwSetFramebufferSizeCallback( window, Renderer::frameBufferSizeCallback );
        glfwSetKeyCallback            ( window, Renderer::keyCallback );
        glfwSetScrollCallback         ( window, Renderer::scrollCallback );
        glfwSetCursorPosCallback      ( window, Renderer::cursorCallback );
        glfwSetMouseButtonCallback    ( window, Renderer::mouseButtonCallback );
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
        
        vector<ptr<Object3D>> descendants = scene->getDescendants();
        for( auto object: descendants ) {
            if( instance_of(object, Mesh)) {
                ptr<Mesh> mesh = downcast(object, Mesh);
                ptr<ShaderLib> shader = ShaderLib::create(mesh);
                
                shader->addDefinitions( scene, mesh, gammaInput, gammaOutput );
                
                mesh->shaderID = shader->getID();
                
                if( shaders.count( mesh->shaderID ) == 0) {
                    shaders[mesh->shaderID] = shader;
                    shader->compileShader();
                }
            }
        }
        
        while( !glfwWindowShouldClose( window )) {
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
            
            camera->updateMatrix();
            
            renderPlugins( preRenderPlugins, scene, camera );
            
            for( auto object: descendants ){
                object->updateMatrixWorld(false);
                
                if( instance_of(object, Mesh) == false )
                    continue;
                
                ptr<ShaderLib> shader = shaders[object->shaderID];
                shader->bind();
                    shader->setFog              ( scene->fog, gammaInput );
                    shader->setAmbientLights    ( scene->ambientLight, gammaInput );
                    shader->setHemisphereLights ( scene->hemisphereLights, gammaInput );
                    shader->setDirectionalLights( scene->directionalLights, gammaInput );
                    shader->setPointLights      ( scene->pointLights, gammaInput );
                    shader->setSpotLights       ( scene->spotLights, gammaInput);
                
                    shader->draw(camera, arcball, object, gammaInput );
                shader->unbind();
            }
            
            renderPlugins( postRenderPlugins, scene, camera );
            
            glfwSwapBuffers( window );
            glfwPollEvents();
            
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