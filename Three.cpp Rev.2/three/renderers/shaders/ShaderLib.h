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
        
        bool empty();
        
        ptr<Shader> getShader();
        
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
        
    public:
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
    
    static const ptr<ShaderLib> SHADERLIB_SIMPLE_PASS = ShaderLib::create(
        "depthRGBA",
        {"precision highp float;", "precision highp int;"},
        {},
        Utils::join({
            Chunks::simplePassVertexParams,
        }),
        Utils::join({
            "void main(){",
            Chunks::simplePassVertex,
            "}",
        }),
        
        Utils::join({
            Chunks::simplePassFragmentParams,
        }),
        Utils::join({
            "void main(){",
            Chunks::simplePassFragment,
            "}",
        })
    );
    
    static const ptr<ShaderLib> SHADERLIB_DEPTH_RGBA = ShaderLib::create(
        "depthRGBA",
         {"precision highp float;", "precision highp int;"},
         {},
         Utils::join({
            Chunks::standardVertexParams,
         }),
         Utils::join({
            "void main(){",
                Chunks::depthRGBAVertex,
            "}",
         }),
                                                                         
         Utils::join({
            Chunks::depthRGBAFragmentParams,
         }),
         Utils::join({
            "void main(){",
                Chunks::depthRGBAFragment,
            "}",
         })
    );
    
    
    static const ptr<ShaderLib> SHADERLIB_LAMBERT = ShaderLib::create(
      "lambert",
                                                                      
      {"precision highp float;", "precision highp int;"},
      {"#define LAMBERT"},
                                                                      
      Utils::join({
         Chunks::standardVertexParams,
         Chunks::lambertVertexParams,
         Chunks::pointLightsParams,
         Chunks::directionalLightsParams,
         Chunks::hemisphereLightsParams,
         Chunks::spotLightsParams,
         Chunks::textureVertexParams,
         Chunks::envMapVertexParams,
      }),
      Utils::join({
        "void main(){",
             Chunks::lambertVertex_1,
             Chunks::textureVertex,
             Chunks::envMapVertex,
             Chunks::lambertVertex_2,
        
             Chunks::lambertPointLightsVertex,
             Chunks::lambertDirectionalLightsVertex,
             Chunks::lambertHemisphereLightsVertex,
             Chunks::lambertSpotLightsVertex,
        
             Chunks::lambertVertex_3,
        "}",
      }),
                                                                      
      Utils::join({
         Chunks::standardFragmentParams,
         Chunks::lambertFragmentParams,
         Chunks::textureFragmentParams,
         Chunks::fogFragmentParams,
         Chunks::specularMapFragmentParams,
         Chunks::envMapFragmentParams,
      }),
      Utils::join({
        "void main(){",
            Chunks::lambertFragment_1,
            Chunks::textureFragment,
            Chunks::alphaTestFragment,
            Chunks::specularMapFragment,
            Chunks::lambertFragment_2,
            Chunks::envMapFragment,
            Chunks::lambertFragment_3,
            Chunks::fogFragment,
        "}",
      })
    );
    
    static const ptr<ShaderLib> SHADERLIB_BASIC = ShaderLib::create(
        "basic",
        {},
        {},
                                                                    
        Utils::join({
            Chunks::standardVertexParams,
            Chunks::basicVertexParams,
            Chunks::textureVertexParams,
            Chunks::envMapVertexParams,
        }),
        
        Utils::join({
            "void main(){",
                Chunks::textureVertex,
                Chunks::basicVertex,
                Chunks::envMapVertex,
            "}",
        }),
                                                                    
        Utils::join({
            Chunks::standardFragmentParams,
            Chunks::basicFragmentParams,
            Chunks::directionalLightsParams,
            Chunks::pointLightsParams,
            Chunks::hemisphereLightsParams,
            Chunks::spotLightsParams,
            Chunks::fogFragmentParams,
            Chunks::textureFragmentParams,
            Chunks::normalMapFragmentParams,
            Chunks::specularMapFragmentParams,
            Chunks::envMapFragmentParams,
        }),
                                                                    
        Utils::join({
            "void main() {",
                Chunks::basicFragment_1,
                Chunks::textureFragment,
                Chunks::alphaTestFragment,
                Chunks::specularMapFragment,
                Chunks::normalMapFragment,
                Chunks::envMapFragment,
                Chunks::basicFragment_2,
                Chunks::fogFragment,
            "}",
        })
    );
    
    static const ptr<ShaderLib> SHADERLIB_CUBEMAP = ShaderLib::create(
      "cubemap",
      {},
      {},
      Utils::join({
        Chunks::standardVertexParams,
        Chunks::cubeMapVertexParams,
      }),
                                                                      
      Utils::join({
        "void main() {",
            Chunks::cubeMapVertex,
        "}",
      }),
                                                                      
      Utils::join({
        Chunks::standardFragmentParams,
        Chunks::cubeMapFragmentParams,
      }),
                                                                      
      Utils::join({
        "void main() {",
            Chunks::cubeMapFragment,
        "}",
      })
    );
    
    static const ptr<ShaderLib> SHADERLIB_PHONG = ShaderLib::create(
        "phong",
        {"precision highp float;", "precision highp int;"},
        {"#define PHONG"},
        Utils::join({
            Chunks::standardVertexParams,
            Chunks::phongVertexParams,
            Chunks::textureVertexParams,
            Chunks::envMapVertexParams,
            Chunks::shadowVertexParams,
        }),
        Utils::join({
            "void main() {",
                Chunks::textureVertex,
                Chunks::phongVertex,
                Chunks::envMapVertex,
                Chunks::shadowVertex,
            "}",
        }),
        Utils::join({
            Chunks::standardFragmentParams,
            Chunks::phongFragmentParams,
            Chunks::directionalLightsParams,
            Chunks::pointLightsParams,
            Chunks::hemisphereLightsParams,
            Chunks::spotLightsParams,
            Chunks::fogFragmentParams,
            Chunks::textureFragmentParams,
            Chunks::normalMapFragmentParams,
            Chunks::specularMapFragmentParams,
            Chunks::envMapFragmentParams,
            Chunks::shadowFragmentParams,
        }),
        Utils::join({
            "void main() {",
                Chunks::phongFragment_1,
                Chunks::textureFragment,
                Chunks::alphaTestFragment,
                Chunks::specularMapFragment,
                Chunks::normalMapFragment,
                Chunks::phongPointLightsFragment,
                Chunks::phongDirectionalLightsFragment,
                Chunks::phongHemisphereLightsFragment,
                Chunks::phongSpotLightsFragment,
                Chunks::phongFragment_2,
                Chunks::envMapFragment,
                Chunks::shadowFragment,
                Chunks::phongFragment_3,
                Chunks::fogFragment,
            "}",
        })
    );
}

#endif /* defined(__Three_cpp_Rev_2__SHADERLIB__) */
