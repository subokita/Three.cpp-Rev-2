//
//  ShaderLib.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 08/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__ShaderLib__
#define __Three_cpp_Rev_2__ShaderLib__

#include <iostream>
#include "internal_headers.h"
#include "Utils.h"
#include "ShaderChunks.h"

namespace three {
    static const std::string SHADER_VERSION = "#version 410 core";
    
    class ShaderLib {
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
        void addDefinitions(ptr<Scene> scene, bool gamma_input, bool gamma_output);
        
        bool empty();
        
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
        
        void draw( ptr<Camera> camera, ptr<Arcball> arcball, ptr<Object3D> object, bool gamma_input );
        
        void setFog              ( ptr<IFog> ifog, bool gamma_input );
        void setAmbientLights    ( ptr<AmbientLight> lights, bool gamma_input );
        void setHemisphereLights ( std::vector<ptr<HemisphereLight>>& lights, bool gamma_input );
        void setDirectionalLights( std::vector<ptr<DirectionalLight>>& lights, bool gamma_input );
        void setPointLights      ( std::vector<ptr<PointLight>>& lights, bool gamma_input );
        void setSpotLights       ( std::vector<ptr<SpotLight>>& lights, bool gamma_input );
        
    protected:
        ShaderLib( std::string name, std::vector<std::string> precisions, std::vector<std::string> defines,
                   std::string vertex_params, std::string vertex_code,
                   std::string fragment_params, std::string fragment_code );
        
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
    
    static const ptr<ShaderLib> ShaderLib_PHONG = ShaderLib::create(
            "phong",
            {"precision highp float;", "precision highp int;"},
            {},
            Utils::join({
                Chunks::standardVertexParams,
                Chunks::phongVertexParams,
                Chunks::textureVertexParams,
                Chunks::normalMapVertexParams,
            }),
            Utils::join({
                "void main() {",
                    Chunks::textureVertex,
                    Chunks::phongVertex,
                "}",
            }),
            Utils::join({
                Chunks::standardFragmentParams,
                Chunks::phongFragmentParams,
                Chunks::directionalLightsFragmentParams,
                Chunks::pointLightsFragmentParams,
                Chunks::hemisphereLightsFragmentParams,
                Chunks::spotLightsFragmentParams,
                Chunks::fogFragmentParams,
                Chunks::textureFragmentParams,
                Chunks::normalMapFragmentParams,
            }),
            Utils::join({
                "void main() {",
                    Chunks::phongFragment_1,
                    Chunks::normalMapFragment,
                    Chunks::textureFragment,
                    Chunks::pointLightsFragment,
                    Chunks::directionalLightsFragment,
                    Chunks::hemisphereLightsFragment,
                    Chunks::spotLightsFragment,
                    Chunks::phongFragment_2,
                    Chunks::fogFragment,
                "}",
            })
    );
}

#endif /* defined(__Three_cpp_Rev_2__ShaderLib__) */
