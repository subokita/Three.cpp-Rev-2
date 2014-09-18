//
//  ShaderLibraries.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 01/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef Three_cpp_Rev_2_ShaderLibraries_h
#define Three_cpp_Rev_2_ShaderLibraries_h

#include "internal_headers.h"
#include "ShaderLib.h"

namespace three {
#pragma mark PARTICLE_BASIC
static const ptr<ShaderLib> SHADERLIB_PARTICLE_BASIC = ShaderLib::create(
    "particleBasic",
    {"precision highp float;", "precision highp int;"},
    {},
    Utils::join({
        Chunks::standardVertexParams,
        Chunks::particleBasicVertexParams,
        Chunks::colorVertexParams,
        Chunks::textureVertexParams,
    }),
    Utils::join({
        "void main() {",
            Chunks::pointSizeVertex,
            Chunks::colorVertex,
            Chunks::particleBasicVertex,
        "}",
    }),
                                                                         
    Utils::join({
        Chunks::standardFragmentParams,
        Chunks::particleBasicFragmentParams,
        Chunks::colorFragmentParams,
        Chunks::textureFragmentParams,
    }),
    Utils::join({
        "void main() {",
            Chunks::particleBasicFragment_1,
            Chunks::textureParticleFragment,
            Chunks::alphaTestFragment,
            Chunks::colorFragment,
        "}",
    })
);
    
    
#pragma mark FONT_PASS
static const ptr<ShaderLib> SHADERLIB_FONT_PASS = ShaderLib::create(
    "fontPass",
    {"precision highp float;", "precision highp int;"},
    {},
    Utils::join({
        Chunks::fontVertexParams,
    }),
    
    Utils::join({
        "void main(){",
            Chunks::fontVertex,
        "}",
    }),
    Utils::join({
        Chunks::fontFragmentParams,
    }),
    
    Utils::join({
        "void main(){",
            Chunks::fontFragment,
        "}",
    })
);
    
#pragma mark SIMPLE_PASS
static const ptr<ShaderLib> SHADERLIB_SIMPLE_PASS = ShaderLib::create(
    "simplePass",
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

#pragma mark DEPTH_PASS
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

#pragma mark LAMBERTIAN_SHADING
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
        Chunks::pointSizeVertex,
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

#pragma mark BASIC_SHADING
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
        Chunks::pointSizeVertex,
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
    "void main(){",
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

#pragma mark CUBEMAP
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
    
#pragma mark PHONG_SHADING
static const ptr<ShaderLib> SHADERLIB_PHONG = ShaderLib::create(
    "phong",
    {"precision highp float;", "precision highp int;"},
    {"#define PHONG"},
    Utils::join({
        Chunks::standardVertexParams,
        Chunks::colorVertexParams,
        Chunks::phongVertexParams,
        Chunks::textureVertexParams,
        Chunks::envMapVertexParams,
        Chunks::shadowVertexParams,
    }),
    Utils::join({
    "void main() {",
        Chunks::pointSizeVertex,
        Chunks::colorVertex,
        Chunks::textureVertex,
        Chunks::phongVertex,
        Chunks::envMapVertex,
        Chunks::shadowVertex,
    "}",
    }),
    Utils::join({
        Chunks::standardFragmentParams,
        Chunks::colorFragmentParams,
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
        Chunks::shadowFragment,
        Chunks::phongPointLightsFragment,
        Chunks::phongDirectionalLightsFragment,
        Chunks::phongHemisphereLightsFragment,
        Chunks::phongSpotLightsFragment,
        Chunks::phongFragment_2,
        Chunks::colorFragment,
        Chunks::envMapFragment,
        Chunks::phongFragment_3,
        Chunks::fogFragment,
    "}",
    })
);
}
#endif
