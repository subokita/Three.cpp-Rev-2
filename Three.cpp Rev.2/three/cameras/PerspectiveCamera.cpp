//
//  PerspectiveCamera.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "PerspectiveCamera.h"
#include "../maths/MathUtils.h"

#include <glm/gtc/matrix_transform.hpp>

namespace three {
    PerspectiveCamera::PerspectiveCamera() :
        fov   ( 50.0 ),
        aspect( 1.0 ),
        near  ( 0.1 ),
        far   ( 2000.0 )
    {
        updateProjectionMatrix();
    }
    
    PerspectiveCamera::PerspectiveCamera( const float fov, const float aspect, const float near, const float far ):
        fov   ( fov ),
        aspect( aspect ),
        near  ( near ),
        far   ( far )
    {
        updateProjectionMatrix();
    }
    
    PerspectiveCamera::~PerspectiveCamera(){}
    
    ptr<PerspectiveCamera> PerspectiveCamera::create( const float fov, const float aspect, const float near,
                                                      const float far ){
        return make_shared<PerspectiveCamera>( PerspectiveCamera(fov, aspect, near, far) );
    }
    
    
    //FIXME:
    void PerspectiveCamera::setLens( float focal_length, float frame_height ){
        if ( frame_height < 1.0 )
            frame_height = 24.0;
        
        this->fov = 2 * MathUtils::radToDeg( atan( frame_height / (focal_length * 2.0) ) );
        updateProjectionMatrix();
    }
    
    //FIXME:
    void PerspectiveCamera::setViewOffset( float full_width, float full_height, float x, float y,
                                           float width, float height ) {
        
        this->fullWidth  = full_width;
        this->fullHeight = full_height;
        this->x          = x;
        this->y          = y;
        this->width      = width;
        this->height     = height;
        
        this->updateProjectionMatrix();
    }
    
    void PerspectiveCamera::updateProjectionMatrix() {
        if( this->fullWidth > 0.0 ) {
            
            float aspect = fullWidth / fullHeight;
            float top    = tan( MathUtils::degToRad( fov * 0.5 ) ) * near;
            float bottom = -top;
            float left   = aspect * bottom;
            float right  = aspect * top;
            float width  = fabsf( right - left );
            float height = fabsf( top - bottom );
            
            projection = glm::frustum( left + this->x * width / this->fullWidth,
                                       left + (this->x + this->width) * width / this->fullWidth,
                                       top - (this->x + this->height) * height / this->fullHeight,
                                       top - this->y * height / this->fullHeight,
                                       this->near, this->far);
        }
        else
            projection = glm::perspective( fov, aspect, near, far );
    }
}