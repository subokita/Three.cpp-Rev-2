//
//  Color.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Color.h"
#include <sstream>
#include <iomanip>

using std::string;
using std::stringstream;

namespace three {
    
    Color::Color():
        rep(1.0, 1.0, 1.0)
    {}
    
    
    Color::Color( glm::vec3 rep ):
        rep( rep )
    {}
    
    Color::Color( float r, float g, float b ):
        rep(r, g, b)
    {}
    
    Color::Color(const Color& rhs) :
        rep( rhs.rep )
    {}
    
    
    Color::Color( int hex_value ) {
        set( hex_value );
    }
    
    Color::~Color(){
    }
    
    Color& Color::set( float r, float g, float b ){
        rep = glm::vec3( r, g, b );
        return *this;
    }
    
    Color& Color::set( int hex ){
        rep = glm::vec3(
            (hex >> 16 & 255) / 255.0,
            (hex >>  8 & 255) / 255.0,
            (hex       & 255) / 255.0
        );
        return *this;
    }
    
    //FIXME: ???
    Color& Color::set( std::string style ){
        return *this;
    }
    
    Color& Color::setHSL( float h, float s, float l ){
        if( s == 0.0 ) {
            rep = glm::vec3(l, l, l);
        }
        else {
            float p = l <= 0.5 ? l * (1.0 + s) : l + s - (l * s);
            float q = (2 * l) - p;
            
            rep = glm::vec3(
                hueToRGB( q, p, h + 1.0 / 3.0 ),
                hueToRGB( q, p, h ),
                hueToRGB( q, p, h - 1.0 / 3.0 )
            );
            
        }
        
        return *this;
    }
    
    Color& Color::operator=( const Color& rhs ){
        if( this == &rhs )
            return *this;
        
        this->rep = rhs.rep;
        
        return *this;
    }
    
    //FIXME: Test it out
    Color Color::copyGammaToLinear( Color& rhs ){
        Color color;
        color.rep = rhs.rep * rhs.rep;
        return color;
    }
    
    Color Color::copyLinearToGamma( Color& rhs ){
        Color color;
        color.rep = glm::sqrt( rhs.rep );
        return color;
    }
    
    Color& Color::convertGammaToLinear(){
        this->rep = rep * rep;
        return *this;
    }
    
    Color& Color::convertLinearToGamma(){
        this->rep = glm::sqrt( rep );
        return *this;
    }
    
    int Color::getHex(){
        int r = static_cast<int>(rep.x * 255.0);
        int g = static_cast<int>(rep.y * 255.0);
        int b = static_cast<int>(rep.z * 255.0);
        return r << 16 ^ g << 8 ^ b << 0;
    }
    
    string Color::getHexString(){
        std::stringstream ss;
        ss << std::hex << getHex();
        return string( ss.str() );
    }
    
    HSL Color::getHSL(){
        
        HSL hsl;
        hsl.h = 0.0;
        hsl.s = 0.0;
        hsl.l = 0.0;
        
        float max = std::max(std::max( rep.x, rep.y ), rep.z);
        float min = std::min(std::min( rep.x, rep.y ), rep.z);
        
        float hue;
        float saturation;
        float lightness = (min + max) / 2.0;
        
        if( min == max ) {
            hue         = 0.0;
            saturation  = 0.0;
        }
        else {
            float delta = max - min;
            saturation = lightness <= 0.5 ? delta / (max + min) : delta / (2 - max - min);
            
            if( max == rep.x )
                hue = (rep.y - rep.z) / delta + (rep.y < rep.z ? 6.0 : 0.0);
            else if ( max == rep.y )
                hue = (rep.z - rep.x) / delta + 2;
            else if ( max == rep.z )
                hue = (rep.x - rep.y) / delta + 4;
            
            hue /= 6.0;
        }
        
        hsl.h = hue;
        hsl.s = saturation;
        hsl.l = lightness;
        
        return hsl;
    }
    
    Color& Color::offsetHSL( float h, float s, float l ){
        HSL hsl = getHSL();
        hsl.h += h;
        hsl.s += s;
        hsl.l += l;
        setHSL( hsl.h, hsl.s, hsl.l );
        return *this;
    }
    
    
    Color& Color::operator=( int hex ){
        this->set(hex);
        return *this;
    }
    
    Color Color::operator+(const Color& rhs){
        return Color( rep + rhs.rep );
    }
    
    Color& Color::operator+=(const Color& rhs){
        rep += rhs.rep;
        return *this;
    }
    
    Color Color::operator+(float scalar){
        return Color( rep + scalar );
    }
    
    Color& Color::operator+=(float scalar){
        rep += scalar;
        return (*this);
    }
    
    Color Color::operator*( const Color& rhs ){
        return Color( rep * rhs.rep );
    }
    
    Color& Color::operator*=( const Color& rhs ){
        rep *= rhs.rep;
        return *this;
    }
    
    Color Color::operator*( float scalar ){
        return Color( rep * scalar);
    }
    
    Color& Color::operator*=( float scalar ){
        rep *= scalar;
        return *this;
    }
    
    Color& Color::lerp(Color& rhs, float alpha) {
        rep += (rhs.rep - rep) * alpha;
        return *this;
    }
    
    bool Color::equals( Color& rhs ){
        return rep == rhs.rep;
    }
    
    float Color::hueToRGB( float p, float q, float t ) {
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
}