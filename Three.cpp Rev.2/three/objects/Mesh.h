//
//  Mesh.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 09/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Mesh__
#define __Three_cpp_Rev_2__Mesh__

#include <iostream>
#include "internal_headers.h"
#include "HasShaderUniforms.h"

#include "Constants.h"
#include "Object3D.h"

namespace three {
    class Mesh : public Object3D, public HasShaderUniforms {
    public:
        static ptr<Mesh> create(ptr<Geometry> geometry, ptr<Material> material);

        Mesh();
        virtual ~Mesh();
        
        virtual void initGLBuffers();
        virtual void draw();
        virtual void setGLState();
        virtual void setUniforms(ptr<ShaderLib> shader_lib, bool gamma ) override;
        
        bool hasTexture();
        bool hasNormalMap();
        bool hasSpecularMap();
        bool hasEnvMap();
        bool hasGeometry();
        bool hasMaterial();
        bool areGLBuffersInitialized();
        
        const ptr<Texture>      getTexture();
        const ptr<NormalMap>    getNormalMap();
        const ptr<SpecularMap>  getSpecularMap();
        const ptr<EnvMap>       getEnvMap();
        const ptr<Geometry>     getGeometry();
        const ptr<Material>     getMaterial();
        
        void setPointMode    ( bool flag );
        void setWireframeMode( bool flag );
        
        void setTexture     ( const ptr<Texture> texture );
        void setNormalMap   ( const ptr<NormalMap> normal_map );
        void setSpecularMap ( const ptr<SpecularMap> specular_map );
        void setEnvMap      ( const ptr<Texture> env_map );
        void setEnvMap      ( const ptr<EnvMap> env_map );
        void setGeometry    ( const ptr<Geometry> geometry );
        void setMaterial    ( const ptr<Material> material );
        
    protected:
        GLuint bufferIDs[5];
        bool glBuffersInitialized;
        
        ptr<Geometry> geometry;
        ptr<Texture> texture;
        ptr<NormalMap> normalMap;
        ptr<SpecularMap> specularMap;
        ptr<EnvMap> envMap;
        ptr<Material> material;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Mesh__) */
