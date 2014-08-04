//
//  SpecularMap.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 21/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__SpecularMap__
#define __Three_cpp_Rev_2__SpecularMap__

#include <iostream>
#include "internal_headers.h"
#include "Texture.h"

namespace three {
    class SpecularMap: public Texture {
    public:
        static ptr<SpecularMap> create();
        
        virtual void setUniforms(ptr<ShaderLib> shader_lib, bool gamma) override;
        
        SpecularMap();
        ~SpecularMap();
        
    protected:
    };
}

#endif /* defined(__Three_cpp_Rev_2__SpecularMap__) */
