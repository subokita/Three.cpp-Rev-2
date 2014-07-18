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
#include "Object3D.h"
#include "Mesh.h"
#include "Light.h"
#include "AmbientLight.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "HemisphereLight.h"
#include "SpotLight.h"
#include "IFog.h"

namespace three {
    class Scene : public Object3D {
    public:
        static ptr<Scene> create();
        ~Scene();
        Scene& add( ptr<Object3D> object );
        Scene& add( ptr<Mesh> object );
        Scene& addLight( ptr<Light> light );
        
        ptr<IFog> fog;
        ptr<AmbientLight> ambientLight;
        std::vector<ptr<DirectionalLight>> directionalLights;
        std::vector<ptr<HemisphereLight>> hemisphereLights;
        std::vector<ptr<PointLight>> pointLights;
        std::vector<ptr<SpotLight>> spotLights;
        
    protected:
        Scene();
    };
}

#endif /* defined(__Three_cpp_Rev_2__Scene__) */
