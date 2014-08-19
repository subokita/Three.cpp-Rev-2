//
//  Camera.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Camera__
#define __Three_cpp_Rev_2__Camera__

#include <iostream>
#include <glm/glm.hpp>

#include "internal_headers.h"
#include "Object3D.h"
#include "Utils.h"

namespace three {
    class Camera : public Object3D {
    public:
        const glm::mat4& getProjectionMatrix();
        virtual ~Camera();
        
        /* Methods */
        void lookAt( const glm::vec3 vec );
        void lookAt( const float x, const float y, const float z );
        
        void updatePosition( glm::mat4& mat );
        void updateTargetPosition(glm::mat4& mat);
        void updateUpDirection( glm::mat4& mat );
        
        virtual void updateMatrix() override;
        
        
        /** Output stream overloading */
        friend std::ostream &operator <<( std::ostream& os, const Camera& obj ) {
            os << "matrixWorldInverse: " << Utils::toString(obj.matrixWorldInverse) << std::endl;
            os << "projectionMatrix  : " << Utils::toString(obj.projection)         << std::endl;
            return os;
        }
        
    protected:
        Camera();
        Camera(const glm::vec3 position, const glm::vec3 up);
        
        virtual void updateProjectionMatrix() = 0;
        
        
    public:
        glm::mat4 matrixWorldInverse;
        
        
    protected:
        glm::mat4 projection;
        
    };
}

#endif /* defined(__Three_cpp_Rev_2__Camera__) */
