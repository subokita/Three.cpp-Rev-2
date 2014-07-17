//
//  MeshPhongMaterial.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 09/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__MeshPhongMaterial__
#define __Three_cpp_Rev_2__MeshPhongMaterial__

#include <iostream>
#include "Material.h"
#include "Color.h"
#include "Constants.h"

namespace three {
    class MeshPhongMaterial: public Material {
    public:
        static PTR(MeshPhongMaterial) create(
             Color color        = 0xFFFFFF,
             Color ambient      = 0xFFFFFF,
             Color emissive     = 0x000000,
             Color specular     = 0x111111,
             float shininess    = 30.0,
             bool metal = false
         );
        
        
        MeshPhongMaterial();
        MeshPhongMaterial( Color color, Color ambient, Color emissive, Color specular, float shininess, bool metal );
        ~MeshPhongMaterial();
        
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
//        Texture     normalMap;
//        glm::vec2   normalScale;
//        Texture     specularMap;
//        Texture     envMap;
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


#endif /* defined(__Three_cpp_Rev_2__MeshPhongMaterial__) */
