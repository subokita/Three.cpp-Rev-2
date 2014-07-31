//
//  HemisphereLight.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 11/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__HemisphereLight__
#define __Three_cpp_Rev_2__HemisphereLight__

#include <iostream>
#include "Light.h"

namespace three {
    class HemisphereLight : public Light {
    friend class ShaderLib;

    public:
        static ptr<HemisphereLight> create(const Color sky_color, const Color ground_color, const float intensity);
        ~HemisphereLight();
        
        const Color& getGroundColor();
        
    protected:
        HemisphereLight();
        HemisphereLight(const Color sky_color, const Color ground_color, const float intensity);
        
        
        /* Data members */
        Color groundColor;
        
        /** Output stream overloading */
        friend std::ostream &operator <<( std::ostream& os, const HemisphereLight& obj ) {
            os << "HemisphereLight {" << std::endl;
            os << "\tgroundColor: " << obj.groundColor << std::endl;
            os << "\tintensity  : " << obj.intensity   << std::endl;
            os << "}";
            return os;
        }
    };
}

#endif /* defined(__Three_cpp_Rev_2__HemisphereLight__) */
