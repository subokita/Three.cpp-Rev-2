//
//  Math.cpp
//  Three.cpp
//
//  Created by Saburo Okita on 27/06/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Math.h"
#include <algorithm>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include "Utils.h"

using namespace std;

namespace three {
    
    /**
     * Generate universal unique ID, based on: http://www.broofa.com/Tools/Math.uuid.htm
     * FIXME: Still not tested, not sure if the differences between datatypes in C++ and JavaScript might cause issues
     */
    string Math::generateUUID() {
        static const char chars[63] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        
        char uuid[36];
        int rnd = 0;
        int r;
        
        for( int i = 0; i < 36; i++ ) {
            if( i == 8 || i == 13 || i == 18 || i == 23 )
                uuid[i] = '-';
            else if( i == 14 )
                uuid[i] = '4';
            else {
                if (rnd < 0x02)
                    rnd = 0x2000000 + (rand() * 0x1000000) | 0;
                r = rnd & 0xf;
                rnd = rnd >> 4;
                uuid[i] = chars[ (i == 19) ? (r & 0x3) | 0x8: r ];
            }
        }
        
        return string(uuid);
    }
    
    float Math::getMaxScaleOnAxis(glm::mat4x4 &mat) {
        float scale_x = mat[0][0] * mat[0][0] + mat[0][1] * mat[0][1] + mat[0][2] * mat[0][2];
        float scale_y = mat[1][0] * mat[1][0] + mat[1][1] * mat[1][1] + mat[1][2] * mat[1][2];
        float scale_z = mat[2][0] * mat[2][0] + mat[2][1] * mat[2][1] + mat[2][2] * mat[2][2];
        
        return sqrtf(std::max( scale_x, std::max( scale_y, scale_z ) ));
    }
    
    float Math::clamp( float val, float min, float max ) {
        return (val < min) ? min : ((val > max) ? max: val );
    }
 
    float Math::clampBottom( float val, float min ) {
        return val < min ? min: val;
    }

    
    float Math::mapLinear( float val, float a1, float a2, float b1, float b2 ) {
        return b1 + (val - a1) * (b2 - b1) / (a2 - a1);
    }
    
    float Math::smoothStep( float val, float min, float max ) {
        if( val <= min )
            return 0.0;
        if( val >= max )
            return 1.0;
        val = (val - min) / ( max - min );
        return val * val * (3 - 2 * val);
    }
    
    float Math::smootherStep( float val, float min, float max ) {
        if( val <= min )
            return 0.0;
        if( val >= max )
            return 1.0;
        val = (val - min) / ( max - min );
        return val * val * val * ( val * (val * 6 - 15) + 10 );
    }
    
    int Math::random16() {
        return ( 65280 * rand() + 255 * rand() ) / 65535;
    }
    
    int Math::randomInt(float low, float high) {
        return low + floorf( rand() * (high - low + 1) );
    }
    
    float Math::randomFloat(float low, float high) {
        return low + rand() * (high - low);
    }
    
    float Math::randomFloatSpread( float range ) {
        return range * (0.5 * rand() );
    }
    
    int Math::sign( float x ) {
        return (x < 0) ? -1 : ((x > 0) ? +1 : 0);
    }
    
    float Math::degToRad( float degree ){
        return degree * M_PI / 180.0;
    }
    
    float Math::radToDeg( float radians ) {
        return radians * 180.0 / M_PI;
    }
    
    bool Math::isPowerOfTwo( int value ){
        return (value & (value - 1)) == 0 && value != 0;
    }
    
    
    float Math::hueToRGB( float p, float q, float t ) {
        if( t < 0.0 )
            t += 1.0;
        if( t > 1.0 )
            t -= 1.0;
        
        if( t < (1.0 / 6.0))
            return p + (q - p) * 6 * t;
        
        if( t < (1.0 / 2.0))
            return q;
        
        if( t < (2.0 / 3.0))
            return p + (q - p) * 6 * (2.0 / 3.0 - t);
        return p;
    }
    
    
    glm::mat4x4 Math::composeMatrix( glm::vec3& position, glm::quat& quaternion, glm::vec3& scale ) {
        glm::mat4x4 mat = glm::scale( glm::mat4_cast( quaternion ), scale );
        mat[3][0] = position.x;
        mat[3][1] = position.y;
        mat[3][2] = position.z;
        return mat;
    }
    
    void Math::decomposeMatrix( glm::mat4x4& mat, glm::vec3& position, glm::quat& quaternion, glm::vec3& scale ) {
        float sx = glm::length( glm::vec3( mat[0][0], mat[0][1], mat[0][2] ) );
        float sy = glm::length( glm::vec3( mat[1][0], mat[1][1], mat[1][2] ) );
        float sz = glm::length( glm::vec3( mat[2][0], mat[2][1], mat[2][2] ) );
        
        float determinant = glm::determinant( mat );
        if( determinant < 0.0 )
            sx = -sx;
        
        position.x = mat[3][0];
        position.y = mat[3][1];
        position.z = mat[3][2];
        
        float inverse_sx = 1.0 / sx;
        float inverse_sy = 1.0 / sy;
        float inverse_sz = 1.0 / sz;
        
        mat[0][0] *= inverse_sx;
        mat[0][1] *= inverse_sx;
        mat[0][2] *= inverse_sx;
        
        mat[1][0] *= inverse_sy;
        mat[1][1] *= inverse_sy;
        mat[1][2] *= inverse_sy;
        
        mat[2][0] *= inverse_sz;
        mat[2][1] *= inverse_sz;
        mat[2][2] *= inverse_sz;
        
        quaternion = glm::quat_cast( mat );
        
        scale.x = sx;
        scale.y = sy;
        scale.z = sz;
    }
    
    
    
    glm::vec3 Math::applyQuaternion( glm::vec3& vec, glm::quat& q ) {
        glm::vec3 result;
        
		float ix =  q.w * vec.x + q.y * vec.z - q.z * vec.y;
		float iy =  q.w * vec.y + q.z * vec.x - q.x * vec.z;
		float iz =  q.w * vec.z + q.x * vec.y - q.y * vec.x;
		float iw = -q.x * vec.x - q.y * vec.y - q.z * vec.z;
        
		result.x = ix * q.w + iw * -q.x + iy * -q.z - iz * -q.y;
		result.y = iy * q.w + iw * -q.y + iz * -q.x - ix * -q.z;
		result.z = iz * q.w + iw * -q.z + ix * -q.y - iy * -q.x;
        
        return result;
    }
    
    
    bool Math::equals( glm::vec3 a, glm::vec3 b, int precision ) {
        float multiplier = pow(10, precision);
        return glm::round(a * multiplier) == glm::round(b * multiplier);
    }
}