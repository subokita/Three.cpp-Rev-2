//
//  Arcball.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 14/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Arcball.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_inverse.hpp>

namespace three {
    
    ptr<Arcball> Arcball::create( GLfloat roll_speed, bool x_axis, bool y_axis  ) {
        return make_shared<Arcball>(Arcball(roll_speed, x_axis, y_axis ));
    }
    
    
    Arcball::Arcball(GLfloat roll_speed, bool x_axis, bool y_axis) :
        CameraControl( x_axis, y_axis ),
        rollSpeed   (roll_speed),
        angle       (0.0f),
        camAxis     (glm::vec3(0.0, 1.0, 0.0))
    {
        
    }
    
    Arcball::~Arcball() {    
    }
    
    void Arcball::scrollCallback( GLFWwindow *window, double x, double y ) {
        /* Handle zooming using mouse scroll */
        
        glm::vec3 dir = glm::normalize(camera->position - camera->target->position);
        if( glm::length(dir) <= 0.01 )
            return;
        
        dir *= static_cast<float>(-y);
        
        camera->position += dir;
        camera->target->position += dir;
        
        camera->lookAt(camera->target->position);
    }
    
    
    /**
     * Check whether we should start the mouse event
     * Event 0: when no tracking occured
     * Event 1: at the start of tracking, recording the first cursor pos
     * Event 2: tracking of subsequent cursor movement
     */
    void Arcball::mouseButtonCallback( GLFWwindow * window, int button, int action, int mods ){
        this->mouseEvent = action == GLFW_PRESS ;
        this->button     = button;
        
        if( action == GLFW_RELEASE ) {
            glm::mat4 rot_mat = createTransformationMatrix();
            camera->matrix *= rot_mat;
            camera->updatePosition      ( rot_mat );
            camera->updateUpDirection   ( rot_mat );
            camera->updateTargetPosition( rot_mat );
            
            angle               = 0.0;
            camAxis             = glm::vec3(0.0, 1.0, 0.0);
            translationVector   = glm::vec3(0.0);
        }
    }
    
    void Arcball::cursorCallback( GLFWwindow *window, double x, double y ){
        if( mouseEvent == 0 ) {
            return;
        }
        else if( mouseEvent == 1 ) {
            /* Start of trackball, remember the first position */
            prevPos     = toScreenCoord( x, y );
            mouseEvent  = 2;
            return;
        }
        
        /* Tracking the subsequent */
        currPos  = toScreenCoord( x, y );
        
        if( mouseEvent == 2 && button == GLFW_MOUSE_BUTTON_LEFT ) {
            /* Calculate the angle in radians, and clamp it between 0 and 90 degrees */
            angle    = acos( std::min(1.0f, glm::dot(prevPos, currPos) ));
            
            /* Cross product to get the rotation axis, but it's still in camera coordinate */
            camAxis  = glm::cross( prevPos, currPos );
        }
        else if(mouseEvent == 2 && button == GLFW_MOUSE_BUTTON_RIGHT) {
            translationVector = (currPos - prevPos);
            translationVector *= 2.0;
        }
    }
    
    /**
     * Create rotation matrix within the camera coordinate,
     * multiply this matrix with view matrix to rotate the camera
     */
    glm::mat4 Arcball::createTransformationMatrix() {
        return glm::translate(glm::rotate( glm::degrees(angle) * rollSpeed, camAxis ), translationVector);
    }
    
    /**
     * Create rotation matrix within the world coordinate,
     * multiply this matrix with model matrix to rotate the object
     */
    glm::mat4 Arcball::createModelRotationMatrix( glm::mat4& view_matrix ){
        glm::vec3 axis = glm::inverse(glm::mat3(view_matrix)) * camAxis;
        return glm::rotate( glm::degrees(angle) * rollSpeed, axis );
    }
}