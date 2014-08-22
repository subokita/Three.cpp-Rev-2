//
//  HasShaderUniforms.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 22/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__HasShaderUniforms__
#define __Three_cpp_Rev_2__HasShaderUniforms__

#include <iostream>
#include "internal_headers.h"

namespace three  {
    
    /**
     * Interface for all the objects that pass uniform values to the shaders
     * Currently this class has a terrible sounding name
     */
    class HasShaderUniforms {
    public:
        virtual void setUniforms( ptr<ShaderLib> shader, bool gamma ) = 0;
    };
}

#endif /* defined(__Three_cpp_Rev_2__HasShaderUniforms__) */
