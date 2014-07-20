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
#include "internal_headers.h"
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace three {
    //FIXME: I just shoved my old arcball class, might not work properly
    /**
     * An arcball / trackball class
     */
    class Arcball {
    private:
        int windowWidth;
        int windowHeight;
        int mouseEvent;
        GLfloat rollSpeed;
        GLfloat angle ;
        glm::vec3 prevPos;
        glm::vec3 currPos;
        glm::vec3 camAxis;
        
        bool xAxis;
        bool yAxis;
        
    public:
        static ptr<Arcball> create(int window_width, int window_height, GLfloat roll_speed = 1.0f, bool x_axis = true, bool y_axis = true);
        
        void init( int window_width, int window_height, GLfloat roll_speed, bool x_axis, bool y_axis );
        glm::vec3 toScreenCoord( double x, double y );
        
        void mouseButtonCallback( GLFWwindow * window, int button, int action, int mods );
        void cursorCallback( GLFWwindow *window, double x, double y );
        
        glm::mat4 createViewRotationMatrix();
        glm::mat4 createModelRotationMatrix( glm::mat4& view_matrix );
        
    };
}

#endif /* defined(__Three_cpp_Rev_2__Arcball__) */
