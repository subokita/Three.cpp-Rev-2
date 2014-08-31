//
//  MathUtils.h
//  Three.cpp
//
//  Created by Saburo Okita on 27/06/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp__MathUtils__
#define __Three_cpp__MathUtils__

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace three {
    
    class MathUtils {
    public:
        static bool equals( glm::vec3 a, glm::vec3 b, int precision );
        static std::string generateUUID();
        static float getMaxScaleOnAxis( glm::mat4x4& mat );
        static float clamp( float val, float min, float max );
        static float clampBottom( float val, float min );
        static float mapLinear( float val, float a1, float a2, float b1, float b2 );
        static float smoothStep( float val, float min, float max );
        static float smootherStep( float val, float min, float max );
        static int random16();
        static int randomInt(float low, float high);
        static float randomFloat(float low, float high);
        static float randomFloatSpread( float range );
        static int sign( float x );
        static float degToRad( float degree );
        static float radToDeg( float radians );
        static bool isPowerOfTwo( int value );
        static glm::vec3 lerp( glm::vec3 a, glm::vec3 b, float alpha );
        
        static glm::mat4x4 rodriguesToMatrix( glm::vec3 rodrigues_vector );
        
        static glm::mat4x4 composeMatrix( glm::vec3 position, glm::quat quaternion, glm::vec3 scale );
        static void decomposeMatrix( glm::mat4& mat, glm::vec3& position, glm::quat& quaternion, glm::vec3& scale );
        
        static glm::vec3 applyQuaternion( glm::vec3& vec, glm::quat& q );
        static glm::mat4 lookAt( glm::vec3 eye, glm::vec3 target, glm::vec3 up );
    };
}

#endif /* defined(__Three_cpp__MathUtils__) */
