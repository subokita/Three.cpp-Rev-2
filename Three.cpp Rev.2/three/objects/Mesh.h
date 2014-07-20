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
#include "Constants.h"
#include "Object3D.h"
#include "Geometry.h"
#include "Material.h"
#include "Texture.h"
#include "Shader.h"

namespace three {
    class Mesh : public Object3D {
    public:
        static ptr<Mesh> create();
        static ptr<Mesh> create(ptr<Geometry> geometry, ptr<Material> material);
        
        void draw(ptr<Shader> shader, bool gammaInput);
        ~Mesh();

        Mesh();
        Mesh( ptr<Geometry> geometry, ptr<Material> material );
        
    public:
        ptr<Texture> texture;
        ptr<NormalTexture> normalMap;
        ptr<Geometry> geometry;
        ptr<Material> material;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Mesh__) */
