//
//  PhongMaterial.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 09/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__PhongMaterial__
#define __Three_cpp_Rev_2__PhongMaterial__

#include <iostream>
#include "internal_headers.h"
#include "Material.h"
#include "Color.h"
#include "Constants.h"

namespace three {
    class PhongMaterial: public Material {
    public:
        static ptr<PhongMaterial> create(
             Color color        = 0xFFFFFF,
             Color ambient      = 0xFFFFFF,
             Color emissive     = 0x000000,
             Color specular     = 0x111111,
             float shininess    = 30.0,
             bool metal = false
         );
        
        virtual void setUniforms( ptr<ShaderLib> shader_lib, bool gamma ) override;
        ~PhongMaterial();
        
        const Color& getDiffuseColor();
        const Color& getEmissiveColor();
        const Color& getAmbientColor();
        const Color& getSpecularColor();
        float getShininess();
        bool isMetal();
        bool usesRefraction();
        const TEXTURE_OPERATION getTextureOperation();
        float getReflectivity();
        float getRefractionRatio();
        const COLOR_MODE getVertexColorsMode();
        
        
        void setDiffuseColor( const Color color );
        void setEmissiveColor( const Color color );
        void setAmbientColor( const Color color );
        void setSpecularColor( const Color color );
        void setShininess( float shininess );
        void setMetal( bool flag );
        void setRefraction( bool flag );
        void setTextureOperation( TEXTURE_OPERATION op );
        void setReflectivity( float value );
        void setRefractionRatio( float value );
        void setVertexColorsMode( COLOR_MODE mode );
        
    protected:
        PhongMaterial();
        PhongMaterial( Color color, Color ambient, Color emissive, Color specular, float shininess, bool metal );
        
        
    protected:
        Color       color;
        Color       ambient;
        Color       emissive;
        Color       specular;
        float       shininess;
        bool        metal;
        bool        useRefraction;
        
        TEXTURE_OPERATION combine;
        float       reflectivity;
        float       refractionRatio;
        COLOR_MODE  vertexColors;
        
        bool        skinning;
        bool        morphTargets;
        bool        morphNormals;
    };
}


#endif /* defined(__Three_cpp_Rev_2__PhongMaterial__) */
