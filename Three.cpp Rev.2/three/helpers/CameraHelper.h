//
//  CameraHelper.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 25/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__CameraHelper__
#define __Three_cpp_Rev_2__CameraHelper__

#include <iostream>
#include "Mesh.h"
#include "Color.h"


namespace three {
    class CameraHelper : public Mesh {
    public:
        static ptr<CameraHelper> create(Color line_color = 0x00FF00);
        static ptr<CameraHelper> create(glm::vec3 eye, glm::vec3 center, Color line_color = 0x00FF00);
        ~CameraHelper();
        
        void align(glm::vec3 eye, glm::vec3 center);
        
    protected:
        CameraHelper(Color color);
        CameraHelper(glm::vec3 eye, glm::vec3 center, Color line_color);
        
        void init(Color line_color);
    };
}

#endif /* defined(__Three_cpp_Rev_2__CameraHelper__) */
