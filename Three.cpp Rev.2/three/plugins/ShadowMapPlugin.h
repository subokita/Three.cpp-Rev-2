//
//  ShadowMapPlugin.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 24/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__ShadowMapPlugin__
#define __Three_cpp_Rev_2__ShadowMapPlugin__

#include <iostream>
#include "internal_headers.h"
#include "RenderPlugin.h"
#include "Frustum.h"

namespace three {
    class ShadowMapPlugin : public RenderPlugin {
    public:
        static ptr<ShadowMapPlugin> create();
        ShadowMapPlugin();
        ~ShadowMapPlugin();
        
        virtual void setState( ptr<Scene> scene, ptr<Camera> camera ) override;
        virtual void init    ( ptr<Scene> scene, ptr<Camera> camera ) override;
        virtual void render  ( ptr<Scene> scene, ptr<Camera> camera  ) override;
        
        void debugShadow();
        
    protected:
//        ptr<VirtualLight> createVirtualLight( ptr<Light> light, int cascade );
//        void updateVirtualLight( ptr<Light> light, int cascade );
//        void updateShadowCamera( ptr<Camera> camera, ptr<VirtualLight> light );
        
    protected:
        ptr<Texture> depthTexture;
        ptr<RenderTarget> renderTarget;
        ptr<ShaderLib> depthShader;
        std::vector<ptr<Light>> lights;
        
        
//        glm::vec3 min;
//        glm::vec3 max;
//        glm::vec3 position;
//        glm::mat4 projectionScreen;
        
        ptr<Frustum> frustum;
        
        #ifdef DEBUG_SHADOW
        GLuint quad_vertex_buffer;
        ptr<ShaderLib> passthruShader;
        #endif
    };
}

#endif /* defined(__Three_cpp_Rev_2__ShadowMapPlugin__) */
