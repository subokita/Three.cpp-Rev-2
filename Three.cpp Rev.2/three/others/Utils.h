//
//  Utils.h
//  Three.cpp
//
//  Created by Saburo Okita on 30/06/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp__Utils__
#define __Three_cpp__Utils__

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "internal_headers.h"
#include "Color.h"

namespace three {
    
    class Utils {
    public:
        static void printWithLineNumbers( const std::string str );
        
        static unsigned int rgbaTo32Bit(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
        static unsigned int rgbaTo32Bit(Color color);
        
        static std::string toString( ShaderLib& shader );
        
        static std::string join( std::initializer_list<std::string> vec );
        static std::string join( const std::vector<std::string>& vec, const char* start = "", const char * end = "\n" );
        static std::string& ltrim( std::string& str );
        static std::string& rtrim( std::string& str );
        static std::string& trim( std::string& str );
        static std::string toString( const std::vector<int>& vec );
        static std::string toString( const std::vector<float>& vec );
        
        
        static std::string toString( const glm::vec2 * vec, const uint size );
        static std::string toString( const std::vector<glm::vec2>& vec );
        static std::string toString( const glm::vec3 * vec, const uint size );
        static std::string toString( const std::vector<glm::vec3>& vec );
        
        static std::string toString( const glm::vec2& vec );
        static std::string toString( const glm::vec3& vec );
        static std::string toString( const glm::vec4& vec );
        static std::string toString( const glm::quat& vec );
        static std::string toString( const glm::mat4& mat );
        static std::string toString( const glm::mat3& mat );
    };
    
    
}


static std::ostream &operator <<( std::ostream& os, const std::vector<glm::vec2>& obj ) {
    return os << three::Utils::toString(obj);
}

static std::ostream &operator <<( std::ostream& os, const glm::vec2& obj ) {
    return os << three::Utils::toString(obj);
}

static std::ostream &operator <<( std::ostream& os, const glm::vec3& obj ) {
    return os << three::Utils::toString(obj);
}

static std::ostream &operator <<( std::ostream& os, const glm::vec4& obj ) {
    return os << three::Utils::toString(obj);
}

static std::ostream &operator <<( std::ostream& os, const glm::mat4& obj ) {
    return os << three::Utils::toString(obj);
}

static std::ostream &operator <<( std::ostream& os, const glm::mat3& obj ) {
    return os << three::Utils::toString(obj);
}

static std::ostream &operator <<( std::ostream& os, const glm::quat& obj ) {
    return os << three::Utils::toString(obj);
}

#endif /* defined(__Three_cpp__Utils__) */
