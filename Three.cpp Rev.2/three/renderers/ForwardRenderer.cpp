//
//  ForwardRenderer.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 01/09/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "ForwardRenderer.h"
#include "three.h"

namespace three {
    
    /**
     * Initialize forward renderer, it does several things including:
     * - calling Renderer::init()
     * - generate the vertex array
     *
     * @param window_title  title of the GLFW window
     * @param window_width  width of the GLFW window
     * @param window_height height of the GLFW window
     */
    void ForwardRenderer::init( string window_title, GLuint window_width, GLuint window_height ) {
        Renderer::init(window_title, window_width, window_height);
        
        glGenVertexArrays(1, &vertexArrayId );
        glBindVertexArray(vertexArrayId);
        
        glEnable( GL_POINT_SPRITE );
        glEnable( GL_VERTEX_PROGRAM_POINT_SIZE );
        
        initCallbacks();
        instance = this;
    }
    
    
    /**
     * Perform forward render
     * @param scene     the scene graph to be rendered
     * @param camera    the camera viewing the scene
     */
    void ForwardRenderer::render(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera) {
        if( instance == nullptr )
            throw runtime_error( "Unable to start Renderer, please invoke init() first" );
        
        if( scene == nullptr || camera == nullptr )
            throw runtime_error( "Unable to render scene with a null scene or camera" );
        
        if( this->camControl != nullptr )
            camControl->init(camera, width, height );
        
        this->scene     = scene;
        this->camera    = camera;
        this->scene->update();
        
        vector<ptr<Object3D>> descendants = scene->getDescendants();
        
        /* Try to get a set of unique shaders for each objects in the scene */
        for( auto object: descendants ) {
            if( instance_of(object, Mesh)) {
                ptr<Mesh> mesh = downcast(object, Mesh);
                
                /* Init Mesh's buffer objects when necessary */
                if( mesh->hasGeometry() && !mesh->areGLBuffersInitialized() )
                    mesh->initGLBuffers();
                
                ptr<ShaderLib> shader_lib = ShaderLib::create(mesh);
                shader_lib->addDefinitions( scene, mesh, gammaInput, gammaOutput );
                mesh->shaderID = shader_lib->getID();
                
                /* Store the unique shader only, share if they are the same */
                if( shaderLibs.count( mesh->shaderID ) == 0) {
                    shaderLibs[mesh->shaderID] = shader_lib;
                    shader_lib->compileShader();
                }
            }
        }
        
        /* Instantiate shadow plugin when necessary */
        if( this->scene->getShadowCasterCount() > 0 ) {
            shadowMapPlugin = ShadowMapPlugin::create();
            this->preRenderPlugins.push_back( shadowMapPlugin );
        }
        
        for( ptr<RenderPlugin> pre_render_plugin: preRenderPlugins )
            pre_render_plugin->init(scene, camera);
        
        const Rect viewport = scene->getViewportSize();
        
        while( !glfwWindowShouldClose( window )) {
            /* Update objects' world matrices */
            for( auto object: descendants )
                object->updateMatrixWorld( false );
            
            renderPlugins( preRenderPlugins, scene, camera );
            
            /* Render the scene */
            renderTarget->bind();
            setDefaultGLState();
            glViewport( viewport.x, viewport.y, viewport.width, viewport.height );
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            
            for( auto object: descendants ){
                if( !instance_of(object, Mesh) )
                    continue;
                
                /* Set the GL state here */
                downcast(object, Mesh)->setGLState();
                
                ptr<ShaderLib> shader_lib = shaderLibs[object->shaderID];
                shader_lib->bind();
                scene->setUniforms( shader_lib, gammaInput );
                shader_lib->draw(camera, camControl, object, gammaInput );
                shader_lib->unbind();
            }
            
            /* Draw font if it's initialized */
            if( fontStash.isInitialized() )
                fontStash.render();
            
            renderTarget->unbind();
            
            #ifdef DEBUG_SHADOW
            if( shadowMapPlugin != nullptr ) {
                setDefaultGLState();
                shadowMapPlugin->debugShadow();
            }
            #endif
            
            renderPlugins( postRenderPlugins, scene, camera );
            
            scene->update();
            
            glfwSwapBuffers( window );
            glfwPollEvents();
            
            postRenderCallback();
        }
    }
}