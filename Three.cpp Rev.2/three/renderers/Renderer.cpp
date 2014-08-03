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
        clearAlpha   ( 1.0 ),
        renderTarget ( RenderTarget::create( GL_FRAMEBUFFER, 0) )
    {}
    
    Renderer::~Renderer() {
        if( vertexArrayId != -1 )
            glDeleteVertexArrays(1, &vertexArrayId);
        
        glfwDestroyWindow( window );
        glfwTerminate();
        
        window   = nullptr;
        instance = nullptr;
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
        
        arcball = Arcball::create(width, height, 2.0f);
        
        
        initCallbacks();
        instance = this;
        
    }
    
    
    GLuint Renderer::getWidth(){
        return this->width;
    }
    
    GLuint Renderer::getHeight() {
        return this->height;
    }
    
    void Renderer::setDefaultGLState() {
        glClearColor( clearColor.rep.x, clearColor.rep.y, clearColor.rep.z, clearAlpha );
        glClearDepth( 1.0 );
        glClearStencil( 0 );
        
        glEnable( GL_DEPTH_TEST );
        glDepthFunc( GL_LEQUAL );
        
        glFrontFace( GL_CCW );
        
        glEnable( GL_BLEND );
        glBlendEquation( GL_FUNC_ADD );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        
        glCullFace( GL_BACK );
        
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
            auto cam = instance->camera;
            glm::vec3 dir = glm::normalize(cam->target->position - cam->position) * static_cast<float>(-y);
            if( glm::length((cam->position - dir)) == 0.0 )
                return;

            cam->matrix = glm::translate(cam->matrix, dir);
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
    
    
    void Renderer::render(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera) {
        if( instance == nullptr )
            throw runtime_error( "Unable to start Renderer, please invoke init() first" );
        
        if( scene == nullptr || camera == nullptr )
            throw runtime_error( "Unable to render scene with a null scene or camera" );
        
        this->scene = scene;
        this->camera = camera;
        
        this->scene->update();
        
        vector<ptr<Object3D>> descendants = scene->getDescendants();
        
        for( auto object: descendants ) {
            if( instance_of(object, Mesh)) {
                ptr<Mesh> mesh = downcast(object, Mesh);
                
                if( mesh->geometry != nullptr && !mesh->glBuffersInitialized )
                    mesh->initGLBuffers();
                
                ptr<ShaderLib> shader_lib = ShaderLib::create(mesh);
                shader_lib->addDefinitions( scene, mesh, gammaInput, gammaOutput );
                mesh->shaderID = shader_lib->getID();
                
                if( shaderLibs.count( mesh->shaderID ) == 0) {
                    shaderLibs[mesh->shaderID] = shader_lib;
                    shader_lib->compileShader();
                }
            }
        }
        
        auto shadow_map = ShadowMapPlugin::create();
        this->preRenderPlugins.push_back( shadow_map );
        
        for( ptr<RenderPlugin> pre_render_plugin: preRenderPlugins )
            pre_render_plugin->init(scene, camera);

        const Rect viewport = scene->getViewportSize();
        
        while( !glfwWindowShouldClose( window )) {
            renderPlugins( preRenderPlugins, scene, camera );

            renderTarget->bind();

            setDefaultGLState();
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            glViewport(viewport.x, viewport.y, viewport.width, viewport.height);

            for( auto object: descendants ){
                object->updateMatrixWorld(false);
                
                if( instance_of(object, Mesh) == false )
                    continue;
                
                ptr<ShaderLib> shader_lib = shaderLibs[object->shaderID];
                shader_lib->bind();
                {
                    scene->setUniforms( shader_lib, gammaInput );
                    shader_lib->draw(camera, arcball, object, gammaInput );
                }
                shader_lib->unbind();
            }

            #ifdef DEBUG_SHADOW
            if( shadow_map != nullptr )
                shadow_map->debugShadow();
            #endif
            
            renderPlugins( postRenderPlugins, scene, camera );

            scene->update();
            
            glfwSwapBuffers( window );
            glfwPollEvents();
            
            postRenderCallback();
        }
        
    }
    
    
    void Renderer::renderPlugins( std::vector<ptr<RenderPlugin>>& plugins, ptr<Scene> scene, ptr<Camera> camera ) {
        if( plugins.empty() )
            return;
        
        for( ptr<RenderPlugin> plugin: plugins ) {
            plugin->setState( scene, camera );
            plugin->render( scene, arcball, camera );
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