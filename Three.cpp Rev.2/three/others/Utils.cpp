//
//  Utils.cpp
//  Three.cpp
//
//  Created by Saburo Okita on 30/06/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Utils.h"
#include "Shader.h"

#include <sstream>
#include <glm/gtc/quaternion.hpp>

using std::string;
using std::stringstream;
using std::istringstream;
using std::initializer_list;
using std::vector;

namespace three {
    string Utils::join( initializer_list<string> vec ) {
        stringstream ss;
        int tab_counter = 0;
        for( string str: vec ) {
            
            if( str.find("#endif") != -1 || str.find("#else") != -1 || str.find("}") != -1  )
                tab_counter--;
            
            for( int i = 0; i < tab_counter; i++ )
                ss << "\t";
            
            ss << str << "\n";
            
            if( str.find("#if") != -1 || str.find("#else") != -1 || str.find("{") != -1  )
                tab_counter++;
            
        }
        string result = ss.str();
        return result.substr( 0, result.size() - 1 );
    }
    
    string Utils::join( const vector<string>& vec, const char* start, const char* end ) {
        stringstream ss;
        int tab_counter = 0;
        for( string str: vec ) {
            if( str.find("#endif") != -1 || str.find("#else") != -1 )
                tab_counter--;
            
            for( int i = 0; i < tab_counter; i++ )
                ss << "\t";
            
            ss << start << str << end;
            
            if( str.find("#if") != -1 || str.find("#else") != -1 )
                tab_counter++;
            
        }
        string result = ss.str();
        return result.substr( 0, result.size() - 1 );
    }
    
    string& Utils::ltrim(string& str) {
        str.erase(str.begin(), find_if(str.begin(), str.end(), not1(ptr_fun<int, int>(isspace))));
        return str;
    }
    
    string& Utils::rtrim(string& str) {
        str.erase(find_if(str.rbegin(), str.rend(), not1(ptr_fun<int, int>(isspace))).base(), str.end());
        return str;
    }

    string& Utils::trim(string& str) {
        return ltrim( rtrim( str ));
    }
    
    string Utils::toString( const vector<int>& vec ) {
        stringstream ss;
        ss << "[";
        for( int i = 0; i < vec.size() - 1; i++ )
            ss << i << ", ";
        ss << vec[vec.size()-1] << "]";
        return ss.str();
    }
    
    string Utils::toString( const vector<float>& vec ) {
        stringstream ss;
        ss << "[";
        for( int i = 0; i < vec.size() - 1; i++ )
            ss << i << ", ";
        ss << vec[vec.size()-1] << "]";
        return ss.str();
    }
    
    
    string toString( const glm::vec2 * vec, const uint size ) {
        stringstream ss;
        ss << "[";
        for( int i = 0; i < size - 1; i++ )
            ss << Utils::toString( vec[i] ) << ", ";
        ss << Utils::toString(vec[size-1]) << "]";
        return ss.str();
    }
    
    string Utils::toString( const vector<glm::vec2>& vec ) {
        stringstream ss;
        ss << "[";
        for( int i = 0; i < vec.size() - 1; i++ )
            ss << Utils::toString( vec[i] ) << ", ";
        ss << Utils::toString(vec[vec.size()-1]) << "]";
        return ss.str();
    }
    
    
    string toString( const glm::vec3 * vec, const uint size ) {
        stringstream ss;
        ss << "[";
        for( int i = 0; i < size - 1; i++ )
            ss << Utils::toString( vec[i] ) << ", ";
        ss << Utils::toString(vec[size-1]) << "]";
        return ss.str();
    }
    
    string Utils::toString( const vector<glm::vec3>& vec ) {
        stringstream ss;
        ss << "[";
        for( int i = 0; i < vec.size() - 1; i++ )
            ss << Utils::toString( vec[i] ) << ", ";
        ss << Utils::toString(vec[vec.size()-1]) << "]";
        return ss.str();
    }
    
    string Utils::toString( const glm::vec2& vec ) {
        char temp[100];
        sprintf( temp, "(%+.2f, %+.2f)", vec.x, vec.y );
        return string( temp );
    }
    
    string Utils::toString( const glm::vec3& vec ) {
        char temp[100];
        sprintf( temp, "(%+.2f, %+.2f, %+.2f)", vec.x, vec.y, vec.z );
        return string( temp );
    }
    
    string Utils::toString( const glm::vec4& vec ) {
        char temp[100];
        sprintf( temp, "(%+.4f, %+.4f, %+.4f, %+.4f)", vec.x, vec.y, vec.z, vec.w );
        return string( temp );
    }
    
    string Utils::toString( const glm::quat& vec ) {
        char temp[100];
        sprintf( temp, "(%+.2f, %+.2f, %+.2f, %+.2f)", vec.x, vec.y, vec.z, vec.w );
        return string( temp );
    }
    
    
    string Utils::toString( const glm::mat3& mat ) {
        char temp[255];
        sprintf( temp, "|%7.3f %7.3f %7.3f|\n|%7.3f %7.3f %7.3f|\n|%7.3f %7.3f %7.3f|",
                mat[0][0], mat[1][0], mat[2][0],
                mat[0][1], mat[1][1], mat[2][1],
                mat[0][2], mat[1][2], mat[2][2]);
        return string( temp );
    }
    
    string Utils::toString( const glm::mat4& mat ) {
        char temp[255];
        
        sprintf( temp, "|%7.3f %7.3f %7.3f %7.3f|\n|%7.3f %7.3f %7.3f %7.3f|\n|%7.3f %7.3f %7.3f %7.3f|\n|%7.3f %7.3f %7.3f %7.3f|",
                mat[0][0], mat[1][0], mat[2][0], mat[3][0],
                mat[0][1], mat[1][1], mat[2][1], mat[3][1],
                mat[0][2], mat[1][2], mat[2][2], mat[3][2],
                mat[0][3], mat[1][3], mat[2][3], mat[3][3] );
        return string( temp );
    }
    
    string Utils::toString( ShaderLib& shader ) {
        return Utils::join( {
            "/* ============= VERTEX SHADER ============ */\n",
            shader.getVertexParams(),
            "\n",
            shader.getVertexCode(),
            "\n",
            "/* ============= FRAGMENT SHADER ============ */\n",
            shader.getFragmentParams(),
            "\n",
            shader.getFragmentCode(),
        } );
    }
    
    
    void Utils::printWithLineNumbers( const std::string str ) {
        istringstream ss( str );
        string string;
        
        unsigned int i = 0;
        while( std::getline( ss , string , '\n' ) )
            printf( "%6d:\t%s\n", i++, string.c_str() );
    }
    
    /**
     * Convert color to 32 bit unsigned int representation
     */
    unsigned int Utils::rgbaTo32Bit(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
        return (r) | (g << 8) | (b << 16) | (a << 24);
    }
    
    /**
     * Convert color to 32 bit unsigned int representation
     */
    unsigned int Utils::rgbaTo32Bit(Color color) {
        unsigned char r = static_cast<unsigned char>(color.rep.r * 255);
        unsigned char g = static_cast<unsigned char>(color.rep.g * 255);
        unsigned char b = static_cast<unsigned char>(color.rep.b * 255);
        return rgbaTo32Bit(r, g, b, 255);
    }
}