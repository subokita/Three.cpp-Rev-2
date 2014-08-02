//
//  AmbientLight.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 08/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__AmbientLight__
#define __Three_cpp_Rev_2__AmbientLight__

#include <iostream>
#include "Light.h"

namespace three {
    class AmbientLight: public Light, public HasShaderUniforms {
    public:
        static ptr<AmbientLight> create(Color color);
        virtual void setUniforms( ptr<ShaderLib> shader_lib, bool gamma ) override;
        
        ~AmbientLight();
        
    protected:
        /** Constructors **/
        AmbientLight();
        AmbientLight(Color& color);
    };
}

#endif /* defined(__Three_cpp_Rev_2__AmbientLight__) */
