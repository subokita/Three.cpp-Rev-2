//
//  MeshCubeMapMaterial.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 22/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__MeshCubeMapMaterial__
#define __Three_cpp_Rev_2__MeshCubeMapMaterial__

#include <iostream>
#include "internal_headers.h"
#include "Material.h"

namespace three {
    //FIXME: This shouldn't be a material, but it's not quite a mesh too.
    class MeshCubeMapMaterial: public Material {
    public:
        static ptr<MeshCubeMapMaterial> create();
        
        MeshCubeMapMaterial();
        ~MeshCubeMapMaterial();
        
        virtual void setUniforms( ptr<ShaderLib> shader, bool gamma ) override;
    };
}

#endif /* defined(__Three_cpp_Rev_2__MeshCubeMapMaterial__) */
