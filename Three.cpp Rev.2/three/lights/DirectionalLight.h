//
//  DirectionalLight.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 09/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__DirectionalLight__
#define __Three_cpp_Rev_2__DirectionalLight__

#include <iostream>
#include <vector>
#include "Light.h"

namespace three {
    class DirectionalLight : public Light {
    public:
        static ptr<DirectionalLight> create();
        static ptr<DirectionalLight> create( Color color, float intensity, glm::vec3 position );
        ~DirectionalLight();
        
        DirectionalLight();
        DirectionalLight( Color color, float intensity, glm::vec3 position );
    };
}

#endif /* defined(__Three_cpp_Rev_2__DirectionalLight__) */
