//
//  Scene.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Scene__
#define __Three_cpp_Rev_2__Scene__

#include <iostream>
#include "IFog.h"
#include "internal_headers.h"
#include "Object3D.h"
#include "Mesh.h"
#include "AmbientLight.h"
#include "HasShaderUniforms.h"
#include "DirectionalLightsArray.h"
#include "HemisphereLightsArray.h"
#include "PointLightsArray.h"
#include "SpotLightsArray.h"
#include "Rect.h"

namespace three {
    class Scene : public Object3D, public HasShaderUniforms {
    public:
        static ptr<Scene> create();
        ~Scene();
        void add( ptr<Object3D> object );
        void add( ptr<Mesh> object );
        void addLight( ptr<Light> light );
        
        unsigned int getShadowCasterCount();
        
        void update();
        virtual void setUniforms( ptr<ShaderLib> shader_lib, bool gamma ) override;
        
        bool isAutoUpdate();
        const ptr<IFog> getFog();
        const ptr<AmbientLight> getAmbientLight();
        const Rect& getViewportSize();
        
        void setViewport( const float x, const float y, const float width, const float height );
        void setFog(const ptr<IFog> fog);
        void setShadowMapType(SHADOW_MAP type);
        
        SHADOW_MAP              getShadowMapType();
        DirectionalLightsArray& getDirectionalLights();
        HemisphereLightsArray&  getHemisphereLights();
        PointLightsArray&       getPointLights();
        SpotLightsArray&        getSpotLights();
        
    protected:
        Scene();
        void setShadows(ptr<ShaderLib> shader_lib, bool gamma);
        
    protected:
        Rect viewportSize;
        bool autoUpdate;
        unsigned int shadowCasterCount;
        SHADOW_MAP shadowMapType;
        
        ptr<IFog> fog;
        ptr<AmbientLight> ambientLight;
        DirectionalLightsArray directionalLights;
        HemisphereLightsArray hemisphereLights;
        PointLightsArray pointLights;
        SpotLightsArray spotLights;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Scene__) */
