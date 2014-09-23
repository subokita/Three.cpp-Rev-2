//
//  Renderer.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Renderer.h"
#include "three.h"

#include <exception>
#include <sstream>
#include <glm/gtc/matrix_inverse.hpp>

namespace three {
    Renderer* Renderer::instance = nullptr;
    
    Renderer::Renderer() :
        vertexArrayId( -1 ),
        gammaInput   ( false ),
        gammaOutput  ( false ),
        clearColor   ( 0x0 ),
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
    
    /**
     * Return the GLFW window's width
     * @return width of the GLFW window
     */
    GLuint Renderer::getWidth(){
        return this->width;
    }
    
    /**
     * Return the GLFW window's height
     * @return width of the GLFW height
     */
    GLuint Renderer::getHeight() {
        return this->height;
    }
    
    /**
     * Initialize renderer, it does several things including:
     * - setting up GLFW
     * - init callbacks for the user input
     *
     * @param window_title  title of the GLFW window
     * @param window_width  width of the GLFW window
     * @param window_height height of the GLFW window
     */
    void Renderer::init( std::string window_title, GLuint window_width, GLuint window_height ) {
        width       = window_width;
        height      = window_height;
        aspectRatio = width * 1.0 / height;
        
        if( !glfwInit() )
            throw std::runtime_error( "Unable to init GLFW" );
        
        /* Tell GLFW to use OpenGL 4.5 */
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
        glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
        
        window = glfwCreateWindow( window_width, window_height, window_title.c_str(), nullptr, nullptr );
        if(!window) {
            glfwTerminate();
            throw std::runtime_error( "Unable to create GLFW window" );
        }
        
        glfwSetInputMode( window, GLFW_STICKY_KEYS, GL_TRUE );
        glfwMakeContextCurrent( window );
    }
    
#pragma mark FONT_RELATED
    /**
     * Load font using fontstash, it will also compile the font shader internally
     *
     * @param font_name the name used to identify the font to be loaded
     * @param filename  the filename (and path) of the font to be loaded
     */
    void Renderer::addFont( const std::string font_name, const std::string filename ) {
        if( !fontStash.isInitialized() ) {
            Rect viewport = scene == nullptr ? Rect(0, 0, width, height) : scene->getViewportSize();
            fontStash.init(512, 512, FONS_ZERO_TOPLEFT, viewport );
        }
        
        fontStash.addFont(font_name, filename);
    }
    
    unsigned int Renderer::addText( std::string text, int x, int y,
                                    std::string font_name, Color color, float font_size, float spacing, float blur ) {
        return fontStash.addText( text, x, y, font_name, color, font_size, spacing, blur );
    }
    
    void Renderer::setText( unsigned int text_entry_ID, std::string text ) {
        fontStash.setText(text_entry_ID, text );
    }
    
    void Renderer::setText( unsigned int text_entry_ID, std::string text, int x, int y ) {
        fontStash.setText(text_entry_ID, text, x, y );
    }
    void Renderer::setText( unsigned int text_entry_ID, std::string text, int x, int y,
                            std::string font_name, Color color, float font_size, float spacing, float blur ) {
        fontStash.setText(text_entry_ID, text, x, y, font_name, color, font_size, spacing, blur );
    }
    
    void Renderer::clearText() {
        fontStash.clearText();
    }
    
    
    void Renderer::setGamma( const bool input, const bool output ) {
        this->gammaInput  = input;
        this->gammaOutput = output;
    }
    
    void Renderer::setClearColor( Color clear_color ) {
        this->clearColor = clear_color;
    }
    
    float Renderer::getAspectRatio() {
        return this->aspectRatio;
    }
    
    /**
     * Set the default OpenGL state
     */
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
    }
    
    
    void Renderer::setCameraControl( ptr<CameraControl> cam_control ) {
        this->camControl = cam_control;
    }
    
    glm::vec3 Renderer::getCursorPosition() {
        return cursorPosition;
    }
    
    void Renderer::renderPlugins( std::vector<ptr<RenderPlugin>>& plugins, ptr<Scene> scene, ptr<Camera> camera ) {
        if( plugins.empty() )
            return;
        
        for( ptr<RenderPlugin> plugin: plugins ) {
            plugin->setState( scene, camera );
            plugin->render( scene, camera );
        }
    }
    
#pragma mark CALLBACKS
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
        
        defaultKeyCallbackHandler = [](GLFWwindow *window, int key, int scancode, int action, int mod) {
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
        
        keyCallbackHandler = [](GLFWwindow *window, int key, int scancode, int action, int mod) {};
        
        scrollCallbackHandler = []( GLFWwindow *window, double x, double y ) {
            if( instance->camControl != nullptr ) {
                instance->camControl->scrollCallback(window, x, y);
            }
        };
        
        defaultCursorCallbackHandler = []( GLFWwindow *window, double x, double y ) {
            if( instance->camControl != nullptr ) {
                instance->cursorPosition = instance->camControl->toScreenCoord(x, y);
                instance->camControl->cursorCallback( window, x, y );
            }
        };
        
        cursorCallbackHandler = []( GLFWwindow *window, double x, double y ) {};
        
        defaultMouseButtonCallbackHandler = []( GLFWwindow * window, int button, int action, int mods ) {
            if( instance->camControl != nullptr )
                instance->camControl->mouseButtonCallback( window, button, action, mods );
        };
        
        mouseButtonCallbackHandler = []( GLFWwindow * window, int button, int action, int mods ) {};
        
        glfwSetErrorCallback          ( Renderer::errorCallback );
        glfwSetFramebufferSizeCallback( window, Renderer::frameBufferSizeCallback );
        glfwSetKeyCallback            ( window, Renderer::keyCallback );
        glfwSetScrollCallback         ( window, Renderer::scrollCallback );
        glfwSetCursorPosCallback      ( window, Renderer::cursorCallback );
        glfwSetMouseButtonCallback    ( window, Renderer::mouseButtonCallback );
    }
    
    void Renderer::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mod) {
        instance->defaultKeyCallbackHandler( window, key, scancode, action, mod );
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
        instance->defaultMouseButtonCallbackHandler(window, button, action, mods );
        instance->mouseButtonCallbackHandler(window, button, action, mods );
    }
    
    void Renderer::cursorCallback( GLFWwindow *window, double x, double y ) {
        instance->defaultCursorCallbackHandler(window, x, y);
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