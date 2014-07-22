//
//  PhongMaterial.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 09/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__PhongMaterial__
#define __Three_cpp_Rev_2__PhongMaterial__

#include <iostream>
#include "internal_headers.h"
#include "Material.h"
#include "Color.h"
#include "Constants.h"

namespace three {
    class PhongMaterial: public Material {
    public:
        static ptr<PhongMaterial> create(
             Color color        = 0xFFFFFF,
             Color ambient      = 0xFFFFFF,
             Color emissive     = 0x000000,
             Color specular     = 0x111111,
             float shininess    = 30.0,
             bool metal = false
         );
        
        
        virtual void setUniforms( ptr<Shader> shader, bool gamma ) override;
        
        PhongMaterial();
        PhongMaterial( Color color, Color ambient, Color emissive, Color specular, float shininess, bool metal );
        ~PhongMaterial();
        
    public:
        
        /* Data members */
        Color       color;
        Color       ambient;
        Color       emissive;
        Color       specular;
        float       shininess;
        bool        metal;
        bool        wrapAround;
        glm::vec3   wrapRGB;
//        Texture     map;
//        Texture     lightMap;
//        Texture     bumpMap;
//        float       bumpScale;
//        Texture     NormalMap;
//        glm::vec2   normalScale;
//        Texture     specularMap;
//        Texture     envMap;
        bool        useRefraction;
        int         combine;
        float       reflectivity;
        float       refractionRatio;
        bool        fog;
        int         shading;
        int         vertexColors;
        bool        skinning;
        bool        morphTargets;
        bool        morphNormals;
    };
}


#endif /* defined(__Three_cpp_Rev_2__PhongMaterial__) */
