//
//  Shader.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Shader__
#define __Three_cpp_Rev_2__Shader__

#include "internal_headers.h"

#include "ShaderLib.h"
#include "Color.h"

namespace three {
    class Shader {
    public:
        static ptr<Shader> create( const char * vertex_filename, const char * fragment_filename );
        static ptr<Shader> create( ShaderLib& shader_code );
        
        void initFromSource( const char * vertex_filename, const char * fragment_filename );
        void initFromSource( ShaderLib& shader );
        
        Shader();
        ~Shader();
        
        void bind();
        void unbind();
        unsigned int getId();
        
        GLint getAttribute( const char * attribute_name );
        
        bool setUniform( const char * uniform_name, Color color, float intensity, bool gamma_input );
        bool setUniform( const char * uniform_name, std::vector<Color>& colors, std::vector<float>& intensities, bool gamma_input );
        
        bool setUniform( const char * uniform_name, GLint v0 );
        bool setUniform( const char * uniform_name, GLfloat v0 );
        bool setUniform( const char * uniform_name, glm::mat3 matrix );
        bool setUniform( const char * uniform_name, glm::mat4 matrix );
        bool setUniform( const char * uniform_name, glm::vec2 vector );
        bool setUniform( const char * uniform_name, glm::vec3 vector );
        bool setUniform( const char * uniform_name, glm::vec4 vector );
        
        bool setUniform( const char * uniform_name, std::vector<GLint>& vector );
        bool setUniform( const char * uniform_name, std::vector<float>& vector );
        bool setUniform( const char * uniform_name, std::vector<glm::mat4>& matrix );
        bool setUniform( const char * uniform_name, std::vector<glm::vec2>& vector );
        bool setUniform( const char * uniform_name, std::vector<glm::vec3>& vector );
        
        bool setUniform( const char * struct_name, const char * variable, GLfloat v0 );
        bool setUniform( const char * struct_name, const char * variable, glm::vec3 vector );
        bool setUniform( const char * struct_name, const char * variable, glm::vec4 vector );
    
    protected:
        GLuint vertexShaderId;
        GLuint fragmentShaderId;
        GLuint programId;
        std::map<std::string, int> uniforms;
        
    protected:
        std::string readShader( const char * filename );
        
        void compileShader( const std::string& vertex_code, const std::string& fragment_code );
        void parseShader( ShaderLib& shader );
        void getUniform( const std::string uniform_name );
        
        static bool getShaderLog( const char * message, std::ostream& os, GLuint shader_id );
        static bool getProgramLog( std::ostream& os, GLuint program_id );
        GLint getStructLocation( const char * struct_name, const char * variable );
    };
}

#endif /* defined(__Three_cpp_Rev_2__Shader__) */
