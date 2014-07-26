//
//  VirtualLight.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 26/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__VirtualLight__
#define __Three_cpp_Rev_2__VirtualLight__

#include <iostream>
#include "internal_headers.h"
#include "DirectionalLight.h"

namespace three {
    class VirtualLight: public DirectionalLight {
    public:
        static ptr<VirtualLight> create();
        VirtualLight();
        ~VirtualLight();
        
        void lookAt( const float x, const float y, const float z );
        void lookAt(const glm::vec3 vec );
        
    public:
        ptr<Camera> originalCamera;
        bool isVirtual;
        bool onlyShadow;
        std::vector<glm::vec3> pointsWorld;
        std::vector<glm::vec3> pointsFrustum;
    };
}

#endif /* defined(__Three_cpp_Rev_2__VirtualLight__) */
