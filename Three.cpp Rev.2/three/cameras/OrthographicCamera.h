//
//  OrthographicCamera.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__OrthographicCamera__
#define __Three_cpp_Rev_2__OrthographicCamera__

#include <iostream>
#include "Camera.h"

namespace three {
    class OrthographicCamera : public Camera {
        
    public:
        static ptr<OrthographicCamera> create(float left, float right, float top, float bottom, float near, float far );

        ~OrthographicCamera();
        
        virtual void updateProjectionMatrix() override;
        
        
        /** Output stream overloading */
        friend std::ostream &operator <<( std::ostream& os, const OrthographicCamera& obj ) {
            os << "OrthographicCamera {" << std::endl;
            os << "\tleft  : " << obj.left   << std::endl;
            os << "\tright : " << obj.right  << std::endl;
            os << "\ttop   : " << obj.top    << std::endl;
            os << "\tbottom: " << obj.bottom << std::endl;
            os << "\tnear  : " << obj.near   << std::endl;
            os << "\tfar   : " << obj.far    << std::endl;
            os << "}";
            return os;
        }
        
    protected:
        OrthographicCamera();
        OrthographicCamera( float left, float right, float top, float bottom, float near, float far );
        
    public:
        /* Data members */
        float left;
        float right;
        float top;
        float bottom;
        float near;
        float far;
    };
}

#endif /* defined(__Three_cpp_Rev_2__OrthographicCamera__) */
