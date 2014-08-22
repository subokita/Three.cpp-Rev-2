//
//  ShaderLib.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 08/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__SHADERLIB__
#define __Three_cpp_Rev_2__SHADERLIB__

#include <iostream>
#include "internal_headers.h"
#include "Utils.h"
#include "ShaderChunks.h"

#define SHADER_VERSION "#version 410 core"

namespace three {
//    static const std::string SHADER_VERSION = "#version 410 core";
    
    class ShaderLib : public std::enable_shared_from_this<ShaderLib> {
    public:
        static ptr<ShaderLib> create( ptr<Mesh> mesh );
        
        static ptr<ShaderLib> create( std::string name, std::vector<std::string> precisions,
                                      std::vector<std::string> defines,
                                      std::string vertex_params, std::string vertex_code,
                                      std::string fragment_params, std::string fragment_code );
        
        ShaderLib( const ShaderLib& rhs );
        ptr<ShaderLib> clone() const;
        
        ~ShaderLib();
        
        void addDefinitions(ptr<Scene> scene, ptr<Mesh> mesh, bool gamma_input, bool gamma_output);
        bool empty();
        
        ptr<Shader> getShader();
        const std::bitset<32> getConfig();
        std::string getID();
        std::string getVertexParams();
        std::string getFragmentParams();
        
        std::string getVertexCode();
        std::string getFragmentCode();
        
        std::string constructFragmentShader();
        std::string constructVertexShader();
        
        void compileShader();
        void bind();
        void unbind();
        
        void draw( ptr<Camera> camera, ptr<CameraControl> arcball, ptr<Object3D> object, bool gamma_input );
        
    protected:
        ShaderLib( std::string name, std::vector<std::string> precisions, std::vector<std::string> defines,
                   std::string vertex_params, std::string vertex_code,
                   std::string fragment_params, std::string fragment_code );
        
        
    protected:
        ptr<Shader> shader;
        std::bitset<32> config;
        
        std::string id;
        std::string name;
        std::string version;
        std::vector<std::string> precisions;
        std::vector<std::string> defines;
        std::string vertexParams;
        std::string vertexCode;
        std::string fragmentParams;
        std::string fragmentCode;
    };
}

#endif /* defined(__Three_cpp_Rev_2__SHADERLIB__) */
