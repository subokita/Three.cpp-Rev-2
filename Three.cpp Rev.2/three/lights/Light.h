//
//  Light.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 08/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Light__
#define __Three_cpp_Rev_2__Light__

#include <iostream>
#include "internal_headers.h"

#include "Color.h"
#include "Object3D.h"
#include "RenderTarget.h"
#include "CastsShadow.h"
#include "HasShaderUniforms.h"

namespace three {
    class Light : public Object3D, public CastsShadow {
    public:
        /** Constructors **/
        Light();
        Light( const Color color, const float intensity );
        virtual ~Light();
        
        const Color& getColor();
        const float getIntensity();
        
        /** Output stream overloading */
        friend std::ostream &operator <<( std::ostream& os, const Light& obj ) {
            os << "Light {" << std::endl;
            os << "\tcolor: " << obj.color << std::endl;
            os << "}";
            return os;
        }
        
        
    protected:
        Color color;
        float intensity;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Light__) */
