//
//  PointLight.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 10/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__PointLight__
#define __Three_cpp_Rev_2__PointLight__

#include <iostream>
#include "Light.h"

namespace three {
    class PointLight: public Light {
    friend class ShaderLib;

    public:
        static ptr<PointLight> create( const glm::vec3 position, const Color color, const float intensity, const float distance );
        ~PointLight();
        
    protected:
        /** Constructors **/
        PointLight();
        PointLight( const glm::vec3 position, const Color color, const float intensity, const float distance );

        
        /* Data members */
        float distance;
        
        
        /** Output stream overloading */
        friend std::ostream &operator <<( std::ostream& os, const PointLight& obj ) {
            os << "PointLight {" << std::endl;
            os << "\tintensity: " << obj.intensity << std::endl;
            os << "\tdistance : " << obj.distance  << std::endl;
            os << "}";
            return os;
        }
    };
}

#endif /* defined(__Three_cpp_Rev_2__PointLight__) */
