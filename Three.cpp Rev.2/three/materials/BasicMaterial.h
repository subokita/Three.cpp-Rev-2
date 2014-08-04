//
//  BasicMaterial.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 23/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__BasicMaterial__
#define __Three_cpp_Rev_2__BasicMaterial__

#include <iostream>

#include "internal_headers.h"
#include "Material.h"
#include "Color.h"

namespace three {
    class BasicMaterial : public Material {
    public:
        static ptr<BasicMaterial> create( Color color = 0xFFFFFF );
        virtual void setUniforms( ptr<ShaderLib> shader_lib, bool gamma ) override;
        ~BasicMaterial();
        
    protected:
        BasicMaterial();
        BasicMaterial(Color color);

    protected:
        Color color;
        
    };
}

#endif /* defined(__Three_cpp_Rev_2__BasicMaterial__) */
