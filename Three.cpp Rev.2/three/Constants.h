//
//  Constants.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 08/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Constants__
#define __Three_cpp_Rev_2__Constants__

#include <iostream>
#include <glm/glm.hpp>

#define PTR(type) std::shared_ptr<type>
#define INSTANCE_OF(var, type) ( std::dynamic_pointer_cast<type>(var) != nullptr )
#define DOWNCAST(var, type) std::dynamic_pointer_cast<type>(var)

static const float MAX_FLOAT = std::numeric_limits<float>::max();
static const float MIN_FLOAT = std::numeric_limits<float>::min();
static const glm::vec3 NULL_VEC3 = glm::vec3( MIN_FLOAT, MIN_FLOAT, MIN_FLOAT );


// FIXME: should use enums
namespace three {
    
    /* CULL STATE */
    static const int CullFaceNone        = 0;
    static const int CullFaceBack        = 1;
    static const int CullFaceFront       = 2;
    static const int CullFaceFrontBack   = 3;
    
    /* WINDINGS */
    static const int FrontFaceDirectionCW    = 0;
    static const int FrontFaceDirectionCCW   = 1;
    
    /* SHADOW MAP */
    static const int BasicShadowMap      = 0;
    static const int PCFShadowMap        = 1;
    static const int PCFSoftShadowMap    = 2;
    
    /* SIDES */
    static const int FrontSide   = 0;
    static const int BackSide    = 1;
    static const int DoubleSide  = 2;
    
    /* SHADINGS */
    static const int NoShading     = 0;
    static const int FlatShading   = 1;
    static const int SmoothShading = 2;
    
    /* COLORS */
    static const int NoColors     = 0;
    static const int FaceColors   = 1;
    static const int VertexColors = 2;
    
    /* BLENDING MODES */
    static const int NoBlending          = 0;
    static const int NormalBlending      = 1;
    static const int AdditiveBlending    = 2;
    static const int SubtractiveBlending = 3;
    static const int MultiplyBlending    = 4;
    static const int CustomBlending      = 5;
    
    /* CUSTOM BLENDING EQUATIONS */
    static const int AddEquation             = 100;
    static const int SubtractEquation        = 101;
    static const int ReverseSubtractEquation = 102;
    static const int ZeroFactor              = 200;
    static const int OneFactor               = 201;
    static const int SrcColorFactor          = 202;
    static const int OneMinusSrcColorFactor  = 203;
    static const int SrcAlphaFactor          = 204;
    static const int OneMinusSrcAlphaFactor  = 205;
    static const int DstAlphaFactor          = 206;
    static const int OneMinusDstAlphaFactor  = 207;
    
    // custom blending source factors
    //static const int ZeroFactor             = 200;
    //static const int OneFactor              = 201;
    //static const int SrcAlphaFactor         = 204;
    //static const int OneMinusSrcAlphaFactor = 205;
    //static const int DstAlphaFactor         = 206;
    //static const int OneMinusDstAlphaFactor = 207;
    static const int DstColorFactor           = 208;
    static const int OneMinusDstColorFactor   = 209;
    static const int SrcAlphaSaturateFactor   = 210;
    
    // TEXTURE CONSTANTS
    static const int MultiplyOperation = 0;
    static const int MixOperation      = 1;
    static const int AddOperation      = 2;
    
    // Wrapping modes
    static const int RepeatWrapping         = 1000;
    static const int ClampToEdgeWrapping    = 1001;
    static const int MirroredRepeatWrapping = 1002;
    
    // Filters
    static const int NearestFilter              = 1003;
    static const int NearestMipMapNearestFilter = 1004;
    static const int NearestMipMapLinearFilter  = 1005;
    static const int LinearFilter               = 1006;
    static const int LinearMipMapNearestFilter  = 1007;
    static const int LinearMipMapLinearFilter   = 1008;
    
    // Data types
    static const int UnsignedByteType  = 1009;
    static const int ByteType          = 1010;
    static const int ShortType         = 1011;
    static const int UnsignedShortType = 1012;
    static const int IntType           = 1013;
    static const int UnsignedIntType   = 1014;
    static const int FloatType         = 1015;
    
    // Pixel types
    //static const int UnsignedByteType    = 1009;
    static const int UnsignedShort4444Type = 1016;
    static const int UnsignedShort5551Type = 1017;
    static const int UnsignedShort565Type  = 1018;
    
    // Pixel formats
    static const int AlphaFormat          = 1019;
    static const int RGBFormat            = 1020;
    static const int RGBAFormat           = 1021;
    static const int LuminanceFormat      = 1022;
    static const int LuminanceAlphaFormat = 1023;
    
    // Compressed texture formats
    static const int RGB_S3TC_DXT1_Format  = 2001;
    static const int RGBA_S3TC_DXT1_Format = 2002;
    static const int RGBA_S3TC_DXT3_Format = 2003;
    static const int RGBA_S3TC_DXT5_Format = 2004;
    
    
    static std::function<void()> UVMapping = [](){};
    
    static std::function<void()> CubeReflectionMapping = [](){};
    static std::function<void()> CubeRefractionMapping = [](){};
    
    static std::function<void()> SphericalReflectionMapping = [](){};
    static std::function<void()> SphericalRefractionMapping = [](){};
};


#endif /* defined(__Three_cpp_Rev_2__Constants__) */
