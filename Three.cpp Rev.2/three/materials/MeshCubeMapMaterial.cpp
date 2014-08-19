//
//  MeshCubeMapMaterial.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 22/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "MeshCubeMapMaterial.h"

namespace three {
    
    ptr<MeshCubeMapMaterial> MeshCubeMapMaterial::create() {
        return make_shared<MeshCubeMapMaterial>();
    }
    
    MeshCubeMapMaterial::MeshCubeMapMaterial(){}
    MeshCubeMapMaterial::~MeshCubeMapMaterial(){}
    
    void MeshCubeMapMaterial::setUniforms( ptr<ShaderLib> shader, bool gamma ){
        glFrontFace( GL_CW );
    }
}