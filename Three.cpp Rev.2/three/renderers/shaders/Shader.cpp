//
//  Shader.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Shader.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_inverse.hpp>


namespace three {
    
    Shader::Shader() {
        
    }
    
    Shader::~Shader() {
        if( vertexShaderId != 0 ) {
            glDetachShader( programId, vertexShaderId );
            glDeleteShader( vertexShaderId );
        }
        
        if( fragmentShaderId != 0 ) {
            glDetachShader( programId, fragmentShaderId );
            glDeleteShader( fragmentShaderId );
        }
        
        if( programId != 0 )
            glDeleteProgram( programId );
        
        uniforms.clear();
    }
    
    ptr<Shader> Shader::create( const char * vertex_filename, const char * fragment_filename ) {
        shared_ptr<Shader> shader = make_shared<Shader>();
        shader->initFromSource( vertex_filename, fragment_filename );
        return shader;
    }
    
    ptr<Shader> Shader::create( ShaderLib& shader_code ) {
        shared_ptr<Shader> shader = make_shared<Shader>();
        shader->initFromSource(shader_code);
        return shader;
    }
    
    
    void Shader::initFromSource( const char * vertex_filename, const char * fragment_filename ) {
        string vertex_code      = Shader::readShader( vertex_filename );
        string fragment_code    = Shader::readShader( fragment_filename );
        
        compileShader( vertex_code, fragment_code );
    }
    
    void Shader::initFromSource( ShaderLib& shader ) {
        if( shader.empty() )
            throw runtime_error( "Unable to use empty shader code" );
                
        parseShader( shader );
        
        string vertex_code      = shader.constructVertexShader();
        string fragment_code    = shader.constructFragmentShader();

//        cout << shader.getID() << endl;
//        cout << vertex_code << endl;
//        cout << "===================" << endl;
//        cout << fragment_code << endl;
//        Utils::printWithLineNumbers( vertex_code );
//        Utils::printWithLineNumbers( fragment_code );
        compileShader( vertex_code, fragment_code );
    }
    
    
    void Shader::compileShader( const std::string& vertex_code, const std::string& fragment_code ) {
        bool result = true;
        
        vertexShaderId   = glCreateShader( GL_VERTEX_SHADER );
        fragmentShaderId = glCreateShader( GL_FRAGMENT_SHADER );
        
        const char * vcs = vertex_code.c_str();
        glShaderSource( vertexShaderId, 1, &vcs, 0 );
        glCompileShader( vertexShaderId );
        result &= Shader::getShaderLog( "Vertex", cout, vertexShaderId );
        
        if( !result ) {
            Utils::printWithLineNumbers( vertex_code );
            throw runtime_error( "Unable to compile vertex shader code" );
        }
        
        const char * fcs = fragment_code.c_str();
        glShaderSource( fragmentShaderId, 1, &fcs, 0 );
        glCompileShader( fragmentShaderId );
        result &= Shader::getShaderLog( "Fragment", cout, fragmentShaderId );
        
        if( !result ) {
            Utils::printWithLineNumbers( fragment_code );
            throw runtime_error( "Unable to compile fragment shader code" );
        }
        
        programId = glCreateProgram();
        
        glAttachShader( programId, vertexShaderId );
        glAttachShader( programId, fragmentShaderId );
        glLinkProgram( programId );
        Shader::getProgramLog( cout, programId );
        
        for( auto & key_value : uniforms )
            getUniform( key_value.first );
    }
    
    unsigned int Shader::getId() {
        return this->programId;
    }
    
    void Shader::bind() {
        glUseProgram( programId );
    }
    
    void Shader::unbind() {
        glUseProgram( 0 );
    }
    
    void Shader::getUniform( const string uniform_name ) {
        if( uniforms[uniform_name] != -1 )
            cout << uniform_name  << " already generated, using the old one" << endl;
        else {
            uniforms[uniform_name] = glGetUniformLocation( programId, uniform_name.c_str() );
        }
    }
    
    GLint Shader::getStructLocation( const char * struct_name, const char * variable ){
        stringstream ss;
        ss << struct_name << "." << variable;
        GLint location = glGetUniformLocation( programId, ss.str().c_str() );
        if( location == -1 ){
            cerr << "Unable to link to uniform variable " << ss.str() << endl;
            throw -1;
        }
        
        return location;
    }
    
#pragma mark SET_UNIFORMS
    bool Shader::setUniform( const char * struct_name, const char * variable, GLfloat v0 ) {
        GLint location = getStructLocation( struct_name, variable );
        if( location == -1 )
            return false;
        
        glUniform1f( location, v0 );
        return true;
    }
    
    bool Shader::setUniform( const char * struct_name, const char * variable, glm::vec3 vector ) {
        GLint location = getStructLocation( struct_name, variable );
        if( location == -1 )
            return false;

        glUniform3f( location, vector.x, vector.y, vector.z );
        return true;
    }
    
    bool Shader::setUniform( const char * struct_name, const char * variable, glm::vec4 vector ) {
        GLint location = getStructLocation( struct_name, variable );
        if( location == -1 )
            return false;

        glUniform4f( location, vector.x, vector.y, vector.z, vector.w );
        return true;
    }
    
    bool Shader::setUniform( const char * uniform_name, vector<Color>& colors, vector<float>& intensities, bool gamma_input ) {
        if( uniforms.count( uniform_name ) == 0  ) {
            /* cerr << uniform_name << " was not generated previously" << endl; */
            /* throw -1 */ return false;;
        }
        
        vector<glm::vec3> vec(colors.size());
        if( gamma_input ){
            for ( int i = 0; i < colors.size(); i++ )
                vec[i] = Color::copyLinearToGamma(colors[i]).rep * intensities[i]* intensities[i];
        }
        else {
            for ( int i = 0; i < colors.size(); i++ )
                vec[i] = colors[i].rep * intensities[i];
        }
        setUniform( uniform_name, vec );
        
        return true;
    }
    
    bool Shader::setUniform( const char * uniform_name, std::vector<float>& vector ) {
        if( uniforms.count( uniform_name ) == 0  ) {
            /* cerr << uniform_name << " was not generated previously" << endl; */
            /* throw -1 */ return false;;
        }
        glUniform1fv( uniforms[uniform_name], static_cast<GLsizei>(vector.size()), &vector[0] );
        return true;
    }
    
    bool Shader::setUniform( const char * uniform_name, std::vector<glm::vec2>& vector ) {
        if( uniforms.count( uniform_name ) == 0  ) {
            /* cerr << uniform_name << " was not generated previously" << endl; */
            /* throw -1 */ return false;;
        }
        glUniform2fv( uniforms[uniform_name], static_cast<GLsizei>(vector.size()), &vector[0][0] );
        return true;
    }
    
    bool Shader::setUniform( const char * uniform_name, std::vector<glm::vec3>& vector ) {
        if( uniforms.count( uniform_name ) == 0  ) {
            /* cerr << uniform_name << " was not generated previously" << endl; */
            /* throw -1 */ return false;;
        }
        glUniform3fv( uniforms[uniform_name], static_cast<GLsizei>(vector.size()), &vector[0][0] );
        return true;
    }
    
    bool Shader::setUniform( const char * uniform_name, glm::vec4 vector ) {
        if( uniforms.count( uniform_name ) == 0  ) {
            /* cerr << uniform_name << " was not generated previously" << endl; */
            /* throw -1 */ return false;;
        }
        glUniform4f( uniforms[uniform_name], vector.x, vector.y, vector.z, vector.w );
        return true;
    }
    
    bool Shader::setUniform( const char * uniform_name, Color color, float intensity, bool gamma_input ) {
        if( !gamma_input )
            return setUniform( uniform_name, color.rep );
        else
            return setUniform( uniform_name, Color::copyGammaToLinear(color).rep * intensity * intensity );
    }
    
    bool Shader::setUniform( const char * uniform_name, glm::vec2 vector ) {
        if( uniforms.count( uniform_name ) == 0  ) {
            /* cerr << uniform_name << " was not generated previously" << endl; */
            /* throw -1 */ return false;;
        }
        glUniform2f( uniforms[uniform_name], vector.x, vector.y );
        return true;
    }
    
    bool Shader::setUniform( const char * uniform_name, glm::vec3 vector ) {
        if( uniforms.count( uniform_name ) == 0  ) {
            /* cerr << uniform_name << " was not generated previously" << endl; */
            /* throw -1 */ return false;;
        }
        glUniform3f( uniforms[uniform_name], vector.x, vector.y, vector.z );
        return true;
    }
    
    bool Shader::setUniform( const char * uniform_name, std::vector<GLint>& vector ){
        if( uniforms.count( uniform_name ) == 0  ) {
            /* cerr << uniform_name << " was not generated previously" << endl; */
            /* throw -1 */ return false;;
        }
        glUniform1iv( uniforms[uniform_name], static_cast<GLsizei>(vector.size()), &vector[0] );
        return true;
    }
    
    bool Shader::setUniform( const char * uniform_name, GLint v0 ) {
        if( uniforms.count( uniform_name ) == 0  ) {
            /* cerr << uniform_name << " was not generated previously" << endl; */
            /* throw -1 */ return false;;
        }
        glUniform1i( uniforms[uniform_name], v0 );
        return true;
    }
    
    bool Shader::setUniform( const char * uniform_name, GLfloat v0 ) {
        if( uniforms.count( uniform_name ) == 0  ) {
            /* cerr << uniform_name << " was not generated previously" << endl; */
            /* throw -1 */ return false;;
        }
        glUniform1f( uniforms[uniform_name], v0 );
        return true;
    }
    
    bool Shader::setUniform( const char * uniform_name, glm::mat3 matrix ) {
        if( uniforms.count( uniform_name ) == 0  ) {
            /* cerr << uniform_name << " was not generated previously" << endl; */
            /* throw -1 */ return false;;
        }
        glUniformMatrix3fv( uniforms[uniform_name], 1, GL_FALSE, glm::value_ptr( matrix ));
        return true;
    }
    
    bool Shader::setUniform( const char * uniform_name, std::vector<glm::mat4>& matrix ) {
        if( uniforms.count( uniform_name ) == 0  ) {
            /* cerr << uniform_name << " was not generated previously" << endl; */
            /* throw -1 */ return false;;
        }
        
        vector<GLfloat*> matrices;
        for( glm::mat4& mat: matrix )
            matrices.push_back( glm::value_ptr(mat) );

        glUniformMatrix4fv( uniforms[uniform_name], static_cast<GLsizei>(matrix.size()), GL_FALSE, matrices[0] );
        return true;
    }
    
    bool Shader::setUniform( const char * uniform_name, glm::mat4 matrix ) {
        if( uniforms.count( uniform_name ) == 0  ) {
            /* cerr << uniform_name << " was not generated previously" << endl; */
            /* throw -1 */ return false;;
        }
        glUniformMatrix4fv( uniforms[uniform_name], 1, GL_FALSE, glm::value_ptr(matrix));
        return true;
    }
    
    
    void Shader::parseShader( ShaderLib& shader ) {
        std::string line;
        std::istringstream lines( shader.getVertexParams() );
        
        while(std::getline( lines, line, '\n')) {
            vector<string> tokens;
            istringstream iss( line );
            copy( istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens) );
            
            if( tokens.size() >= 3 && tokens[0].compare( "uniform" ) == 0 ) {
                string var_name = "";
                for( int i = 2; i < tokens.size(); i++ )
                    var_name += Utils::trim(tokens[i]);
                
                var_name = var_name.substr( 0, var_name.length() - 1 );
                
                size_t array_bracket_index = var_name.find("[");
                if( array_bracket_index != -1 )
                    var_name = var_name.substr( 0, array_bracket_index );
                
                var_name = Utils::trim( var_name );
                uniforms[var_name]  = -1;
            }
        }
        
        lines = std::istringstream( shader.getFragmentParams() );
        
        while(std::getline( lines, line, '\n')) {
            vector<string> tokens;
            istringstream iss( line );
            copy( istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens) );
            
            if( tokens.size() >= 3 && tokens[0].compare( "uniform" ) == 0 ) {
                string var_name = "";
                for( int i = 2; i < tokens.size(); i++ )
                    var_name += Utils::trim(tokens[i]);
                
                var_name = var_name.substr( 0, var_name.length() - 1 );
                
                size_t array_bracket_index = var_name.find("[");
                if( array_bracket_index != -1 )
                    var_name = var_name.substr( 0, array_bracket_index );
                
                var_name = Utils::trim( var_name );
                uniforms[var_name]  = -1;
            }
        }
    }
    
    bool Shader::getShaderLog( const char * message, ostream& os, GLuint shader_id ) {
        int info_log_length = 0;
        glGetShaderiv( shader_id, GL_INFO_LOG_LENGTH, &info_log_length );
        
        if( info_log_length > 0 ) {
            char * info_log_buffer = new char[info_log_length];
            int info_log_read = 0;
            glGetShaderInfoLog( shader_id, info_log_length, &info_log_read, info_log_buffer );
            
            os << message << " info log: " << info_log_buffer << endl;
            
            delete [] info_log_buffer;
            return false;
        }
        else {
//            os << message << ": OK" << endl;
            return true;
        }
    }
    
    
    bool Shader::getProgramLog( ostream &os, GLuint program_id ) {
        int info_log_length = 0;
        glGetProgramiv( program_id, GL_INFO_LOG_LENGTH, &info_log_length );
        
        if( info_log_length > 0 ) {
            char * info_log_buffer = new char[info_log_length];
            int info_log_read = 0;
            glGetProgramInfoLog( program_id, info_log_length, &info_log_read, info_log_buffer );
            
            os << "Shader info log: " << info_log_buffer << endl;
            
            delete [] info_log_buffer;
            return false;
        }
        else {
            //os << "Shader info log: OK" << endl;
            return true;
        }
    }
    
    string Shader::readShader( const char * filename ) {
        stringstream text;
        string line;
        ifstream stream( filename );
        
        if( stream.is_open() ) {
            while( getline( stream, line ) ) {
                text << "\n" << line;
                
                vector<string> tokens;
                istringstream iss( line );
                copy( istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens) );
                if( tokens.size() >= 3 && tokens[0].compare( "uniform" ) == 0 ) {
                    string var_name = "";
                    for( int i = 2; i < tokens.size(); i++ )
                        var_name += Utils::trim(tokens[i]);
                    
                    var_name = var_name.substr( 0, var_name.length() - 1 );
                    
                    size_t array_bracket_index = var_name.find("[");
                    if( array_bracket_index != -1 )
                        var_name = var_name.substr( 0, array_bracket_index );
                    
                    var_name = Utils::trim( var_name );
                    
                    uniforms[var_name]  = -1;
                }
            }
        }
        stream.close();
        return text.str();
    }
}