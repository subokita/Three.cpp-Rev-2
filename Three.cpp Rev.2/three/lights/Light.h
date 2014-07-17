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
#include "Constants.h"
#include "Color.h"
#include "Object3D.h"

namespace three {
    class Light : public Object3D{
    public:
        /** Constructors **/
        Light();
        Light( const Color color, const float intensity );
        virtual ~Light();
        
    public:
        /* Data members */
        Color color;
        float intensity;
        PTR(Object3D) target;
        
        // FIXME: Is it Light or Object3D?
        std::vector<PTR(Light)> shadowCascadeArray;
        
        /** Output stream overloading */
        friend std::ostream &operator <<( std::ostream& os, const Light& obj ) {
            os << "Light {" << std::endl;
            os << "\tcolor: " << obj.color << std::endl;
            os << "}";
            return os;
        }
    };
}

#endif /* defined(__Three_cpp_Rev_2__Light__) */
