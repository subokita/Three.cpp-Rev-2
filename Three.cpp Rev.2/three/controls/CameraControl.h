//
//  CameraControl.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__CameraControl__
#define __Three_cpp_Rev_2__CameraControl__

#include <iostream>
#include "internal_headers.h"

namespace three {
    class CameraControl {
    public:
        virtual void scrollCallback( GLFWwindow *window, double x, double y );
        virtual void mouseButtonCallback( GLFWwindow * window, int button, int action, int mods );
        virtual void cursorCallback( GLFWwindow *window, double x, double y );

        virtual glm::mat4 createTransformationMatrix();
        virtual glm::vec3 toScreenCoord( double x, double y );
        
        void init( ptr<Camera> camera, int window_width, int window_height );
        
        ~CameraControl();
        
    protected:
        CameraControl(bool x_axis = true, bool y_axis = true);
        ptr<Camera> camera;
        
        int windowWidth;
        int windowHeight;
        int mouseEvent;
        int button;
        
        bool xAxis;
        bool yAxis;
    };
}

#endif /* defined(__Three_cpp_Rev_2__CameraControl__) */
