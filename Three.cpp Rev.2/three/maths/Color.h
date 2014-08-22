//
//  Color.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Color__
#define __Three_cpp_Rev_2__Color__

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

namespace three {
    /* Small datastructure to represent HSL */
    struct HSL {
        float h, s, l;
    };
    
    class Color {
    public:
        Color();
        Color( glm::vec3 rep );
        Color( int hex_value );
        Color( float r, float g, float b );
        Color(const Color& rhs);
        ~Color();
        
        Color& set( float r, float g, float b );
        Color& set( int hex );
        Color& set( std::string style );
        Color& setHSL( float h, float s, float l );
        Color& operator=( const Color& rhs );
        static Color copyGammaToLinear( Color& rhs );
        static Color copyLinearToGamma( Color& rhs );
        
        Color& convertGammaToLinear();
        Color& convertLinearToGamma();
        
        int getHex();
        std::string getHexString();
        HSL getHSL();
        Color& offsetHSL( float h, float s, float l );
        
        Color& operator=( int hex );
        
        Color operator+(const Color& rhs);
        Color operator+(float scalar);
        Color& operator+=(const Color& rhs);
        Color& operator+=(float scalar);
        
        Color operator*( const Color& rhs );
        Color operator*( float scalar );
        Color& operator*=( const Color& rhs );
        Color& operator*=( float scalar );
        
        Color& lerp(Color& rhs, float alpha);
        bool equals( Color& rhs );
        
        static float hueToRGB( float p, float q, float t );
        
        friend std::ostream &operator <<( std::ostream& os, const Color& color ) {
            os << "(" << color.rep.x << " " << color.rep.y << " " << color.rep.z << ")";
            return os;
        }
        
        friend std::ostream &operator <<( std::ostream& os, const std::vector<Color>& colors ) {
            os << "[";
            for( int i = 0; i < colors.size() - 1; i++ )
                os << colors[i] << ", ";
            os << colors[colors.size() - 1] << ", ";
            os << "]";
            return os;
        }
        
        
    public:
        glm::vec3 rep;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Color__) */
