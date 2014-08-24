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

    public:
        static ptr<PointLight> create( const glm::vec3 position, const Color color, const float intensity, const float distance );
        ~PointLight();
        
        const float getDistance();
        
        /** Output stream overloading */
        friend std::ostream &operator <<( std::ostream& os, const PointLight& obj ) {
            os << "PointLight {" << std::endl;
            os << "\tintensity: " << obj.intensity << std::endl;
            os << "\tdistance : " << obj.distance  << std::endl;
            os << "}";
            return os;
        }
        
    protected:
        PointLight();
        PointLight( const glm::vec3 position, const Color color, const float intensity, const float distance );

    protected:
        float distance;
    };
}

#endif /* defined(__Three_cpp_Rev_2__PointLight__) */
