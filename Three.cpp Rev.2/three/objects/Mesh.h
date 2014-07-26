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
#include "HasShaderUniforms.h"

#include "Constants.h"
#include "Object3D.h"
#include "Geometry.h"
#include "Material.h"
#include "Texture.h"
#include "Shader.h"

namespace three {
    class Mesh : public Object3D, public HasShaderUniforms {
    public:
        static ptr<Mesh> create();
        static ptr<Mesh> create(ptr<Geometry> geometry, ptr<Material> material);
        
        Mesh();
        Mesh( ptr<Geometry> geometry, ptr<Material> material );
        ~Mesh();
        
        void initGLBuffers();
        void draw();

        virtual void setUniforms(ptr<Shader> shader, bool gamma ) override;
        
    public:
        std::vector<GLuint> bufferIDs;
        bool glBuffersInitialized;
        
        ptr<Texture> texture;
        ptr<NormalMap> normalMap;
        ptr<SpecularMap> specularMap;
        ptr<EnvMap> envMap;
        ptr<Geometry> geometry;
        ptr<Material> material;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Mesh__) */
