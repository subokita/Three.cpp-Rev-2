//
//  LambertMaterial.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 23/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__LambertMaterial__
#define __Three_cpp_Rev_2__LambertMaterial__

#include <iostream>

#include "internal_headers.h"
#include "Material.h"
#include "Color.h"

namespace three {
    class LambertMaterial : public Material {
    public:
        static ptr<LambertMaterial> create(
               Color diffuse      = 0xFFFFFF,
               Color ambient      = 0xFFFFFF,
               Color emissive     = 0x000000
        );
        
        virtual void setUniforms( ptr<ShaderLib> shader_lib, bool gamma ) override;
        
        LambertMaterial();
        LambertMaterial( Color diffuse, Color ambient, Color emissive );
        ~LambertMaterial();
        
    public:
        
        Color       color;
        Color       ambient;
        Color       emissive;
        bool        wrapAround;
        glm::vec3   wrapRGB;
        
        
        TEXTURE_OPERATION combine;
        
        bool        useRefraction;
        float       reflectivity;
        float       refractionRatio;
        bool        fog;
        COLOR_MODE  vertexColors;
        bool        skinning;
        bool        morphTargets;
        bool        morphNormals;
    };
}

#endif /* defined(__Three_cpp_Rev_2__LambertMaterial__) */
