//
//  Gyroscope.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 26/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Gyroscope__
#define __Three_cpp_Rev_2__Gyroscope__

#include <iostream>
#include "internal_headers.h"
#include "Object3D.h"

namespace three {
    class Gyroscope : public Object3D {
    public:
        static ptr<Gyroscope> create();
        Gyroscope();
        ~Gyroscope();
        
        void updateMatrixWorld(bool force) override;
        
        
    public:
        glm::vec3 translationWorld;
        glm::vec3 translationObject;
        glm::quat quaternionWorld;
        glm::quat quaternionObject;
        glm::vec3 scaleWorld;
        glm::vec3 scaleObject;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Gyroscope__) */
