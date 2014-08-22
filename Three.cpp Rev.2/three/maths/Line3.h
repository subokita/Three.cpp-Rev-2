//
//  Line3.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 14/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Line3__
#define __Three_cpp_Rev_2__Line3__

#include <iostream>
#include "Utils.h"
#include "internal_headers.h"


namespace three {
    class Line3 {
    public:
        Line3();
        Line3( glm::vec3 param_start, glm::vec3 param_end );
        ~Line3();
        
        Line3& set( glm::vec3 param_start, glm::vec3 param_end );

        glm::vec3 center();
        glm::vec3 delta();
        
        float distanceSquared();
        float distance();
        
        float closestPointToPointParameter( glm::vec3 point, bool clamp_to_line );
        glm::vec3 at( float t );
        glm::vec3 closestPointToPoint( glm::vec3 point, bool clamp_to_line );
        
        Line3& applyMatrix( glm::mat4x4& mat );
        bool equals( Line3& other );

        
    public:
        glm::vec3 start;
        glm::vec3 end;
        
        friend std::ostream &operator <<( std::ostream& os, const Line3& line ) {
            os << Utils::toString( line.start ) << " to " << Utils::toString( line.end );
            return os;
        }
    };
}


#endif /* defined(__Three_cpp_Rev_2__Line3__) */
