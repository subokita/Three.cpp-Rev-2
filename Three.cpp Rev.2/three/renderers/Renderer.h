//
//  Renderer.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Renderer__
#define __Three_cpp_Rev_2__Renderer__

#include <iostream>
#include <map>
#include <vector>

#include "internal_headers.h"
#include "Color.h"

namespace three {
    class Renderer : public std::enable_shared_from_this<Renderer> {
    public:
        Renderer();
        virtual ~Renderer();
        
        void init( std::string window_title, GLuint window_width, GLuint window_height );
        void render( ptr<Scene> scene, ptr<Camera> camera );
        void setDefaultGLState();
        
        void setPostRenderCallbackHandler ( std::function<void()> handler );
        void setFrameBufferSizeHandler    ( std::function<void(GLFWwindow*, int, int)> handler );
        void setKeyCallbackHandler        ( std::function<void(GLFWwindow*, int, int, int, int)> handler );
        void setErrorCallbackHandler      ( std::function<void(int, const char*)> handler);
        void setScrollCallbackHandler     ( std::function<void(GLFWwindow*, double, double)> handler );
        void setMouseButtonCallbackHandler( std::function<void(GLFWwindow*, int, int, int)> handler );
        void setCursorCallbackHandler     ( std::function<void(GLFWwindow*, double, double)> handler );
        
        GLuint getWidth();
        GLuint getHeight();
        
        void setGamma( const bool input, const bool output );
        void setClearColor( Color clear_color = 0xFFFFFF );
        const float getAspectRatio();
        
    protected:
        static void errorCallback          ( int error, const char * desc );
        static void frameBufferSizeCallback( GLFWwindow *window, int width, int height );
        static void keyCallback            ( GLFWwindow *window, int key, int scancode, int action, int mod);
        static void scrollCallback         ( GLFWwindow *window, double x, double y );
        static void mouseButtonCallback    ( GLFWwindow * window, int button, int action, int mods );
        static void cursorCallback         ( GLFWwindow *window, double x, double y );
        
        void initCallbacks();
        void renderPlugins( std::vector<ptr<RenderPlugin>>& plugins, ptr<Scene> scene, ptr<Camera> camera );
        
    protected:
        ptr<RenderTarget> renderTarget;
        ptr<Arcball> arcball;
        ptr<Scene> scene;
        ptr<Camera> camera;
        ptr<ShadowMapPlugin> shadowMapPlugin;
        
        static Renderer* instance;
        
        bool gammaInput;
        bool gammaOutput;
        Color clearColor;
        float clearAlpha;
        float aspectRatio;
        
        GLuint width;
        GLuint height;
        GLuint vertexArrayId;
        
        GLFWwindow* window;
        
        std::map<std::string, ptr<ShaderLib>> shaderLibs;
        std::vector<ptr<RenderPlugin>> preRenderPlugins;
        std::vector<ptr<RenderPlugin>> postRenderPlugins;
        
        /* All the function callback pointers */
        std::function<void()>                                postRenderCallback = [](){};
        std::function<void(int, const char*)>                errorCallbackHandler;
        std::function<void(GLFWwindow*, int, int)>           frameBufferSizeHandler;
        std::function<void(GLFWwindow*, int, int, int, int)> keyCallbackHandler;
        std::function<void(GLFWwindow*, double, double)>     scrollCallbackHandler;
        std::function<void(GLFWwindow*, int, int, int)>      mouseButtonCallbackHandler;
        std::function<void(GLFWwindow*, double, double)>     cursorCallbackHandler;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Renderer__) */
