//
//  Arcball.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 14/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Arcball__
#define __Three_cpp_Rev_2__Arcball__

#include <iostream>

#include "CameraControl.h"
#include "../internal_headers.h"


#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace three {
    //FIXME: I just shoved my old arcball class, might not work properly
    /**
     * An arcball / trackball class
     */
    class Arcball : public CameraControl {
        
    public:
        static ptr<Arcball> create(GLfloat roll_speed = 1.0f, bool x_axis = true, bool y_axis = true);
        
        ~Arcball();
        
        virtual void scrollCallback( GLFWwindow *window, double x, double y ) override;
        virtual void mouseButtonCallback( GLFWwindow * window, int button, int action, int mods ) override;
        virtual void cursorCallback( GLFWwindow *window, double x, double y ) override;
        virtual glm::mat4 createTransformationMatrix() override;
        
        glm::mat4 createModelRotationMatrix( glm::mat4& view_matrix );
        
    protected:
        Arcball(GLfloat roll_speed, bool x_axis, bool y_axis);

    protected:
        GLfloat rollSpeed;
        GLfloat angle ;
        glm::vec3 prevPos;
        glm::vec3 currPos;
        glm::vec3 camAxis;
        glm::vec3 translationVector;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Arcball__) */
