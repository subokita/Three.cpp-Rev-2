//
//  CameraControl.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "CameraControl.h"
#include "three.h"

using namespace std;
namespace three {
    
    
    CameraControl::CameraControl():
        mouseEvent  ( 0 ),
        xAxis       ( true ),
        yAxis       ( true )
    {
    }
    
    CameraControl::~CameraControl() {
    }
    
    
    void CameraControl::init( ptr<Camera> camera, int window_width, int window_height ) {
        this->camera        = camera;
        this->windowWidth   = window_width;
        this->windowHeight  = window_height;
    }
    
    glm::mat4 CameraControl::createTransformationMatrix() {
        return glm::mat4(1.0);
    }
    
    
    /**
     * Convert the mouse cursor coordinate on the window (i.e. from (0,0) to (windowWidth, windowHeight))
     * into normalized screen coordinate (i.e. (-1, -1) to (1, 1)
     */
    glm::vec3 CameraControl::toScreenCoord( double x, double y ) {
        glm::vec3 coord(0.0f);
        
        if( xAxis )
            coord.x =  (2 * x - windowWidth ) / windowWidth;
        
        if( yAxis )
            coord.y = -(2 * y - windowHeight) / windowHeight;
        
        /* Clamp it to border of the windows, comment these codes to allow rotation when cursor is not over window */
        coord.x = glm::clamp( coord.x, -1.0f, 1.0f );
        coord.y = glm::clamp( coord.y, -1.0f, 1.0f );
        
        float length_squared = coord.x * coord.x + coord.y * coord.y;
        if( length_squared <= 1.0 )
            coord.z = sqrt( 1.0 - length_squared );
        else
            coord = glm::normalize( coord );
        
        return coord;
    }
    
    
    void CameraControl::scrollCallback( GLFWwindow *window, double x, double y ) {
    }
    
    void CameraControl::mouseButtonCallback( GLFWwindow * window, int button, int action, int mods ) {
    }
    
    void CameraControl::cursorCallback( GLFWwindow *window, double x, double y ) {
    }
}