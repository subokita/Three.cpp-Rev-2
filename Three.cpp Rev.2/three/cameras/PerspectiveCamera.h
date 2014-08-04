//
//  PerspectiveCamera.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__PerspectiveCamera__
#define __Three_cpp_Rev_2__PerspectiveCamera__

#include <iostream>
#include "Camera.h"

namespace three {
    class PerspectiveCamera : public Camera {
        
    public:
        static ptr<PerspectiveCamera> create(const float fov, const float aspect, const float near, const float far );

        ~PerspectiveCamera();
        virtual void updateProjectionMatrix() override;
        
        //FIXME: implement this
        void setLens( const float focal_length, const float frame_height );
        
        //FIXME: implement this
        void setViewOffset( const float full_width, const float full_height,
                            const float x, const float y, const float width, const float height );
        
        
    protected:
        PerspectiveCamera();
        PerspectiveCamera(const float fov, const float aspect, const float near, const float far );

    protected:
        float fov;
        float aspect;
        float near;
        float far;
        
        /* FIXME: Do these belong here? */
        float fullWidth;
        float fullHeight;
        float x;
        float y;
        float width;
        float height;
        
        /** Output stream overloading */
        friend std::ostream &operator <<( std::ostream& os, const PerspectiveCamera& obj ) {
            os << "PerspectiveCamera {" << std::endl;
            os << "\tfov   : " << obj.fov    << std::endl;
            os << "\taspect: " << obj.aspect << std::endl;
            os << "\tnear  : " << obj.near   << std::endl;
            os << "\tfar   : " << obj.far    << std::endl;
            os << "}";
            return os;
        }
    };
}

#endif /* defined(__Three_cpp_Rev_2__PerspectiveCamera__) */
