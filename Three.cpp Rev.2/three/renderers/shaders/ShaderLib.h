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
#include "Utils.h"
#include "ShaderChunks.h"

namespace three {
    namespace ShaderLib {
        static const std::string VERSION = "#version 410 core";
        
        struct Shader {
            std::vector<std::string> precisions;
            std::vector<std::string> defines;
            std::string vertexParams;
            std::string vertexShader;
            std::string fragmentParams;
            std::string fragmentShader;
        };
        
        static const Shader phong = {
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
            }),
        };
    }
}

#endif /* defined(__Three_cpp_Rev_2__ShaderLib__) */
