//
//  Material.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Material__
#define __Three_cpp_Rev_2__Material__

#include <iostream>
#include <glm/glm.hpp>

#include "internal_headers.h"
#include "HasID.h"
#include "Math.h"
#include "HasShaderUniforms.h"

using namespace std;

namespace three {
    static int MaterialIDCount = 0;
    
    class Material : public HasID, public HasShaderUniforms {
    public:
        Material();
        virtual ~Material();
        
        
        /* Data members */
        SIDE side;
        SHADING shading;
        
        float opacity;
        bool  transparent;
        BLENDING_MODE blending;
        int   blendSrc;
        int   blendDst;
        int   blendEquation;
        bool  depthTest;
        bool  depthWrite;
        bool  polygonOffset;
        float polygonOffsetFactor;
        float polygonOffsetUnits;
        float alphaTest;
        float overdraw;
        bool  visible;
        bool  needsUpdate;
        bool  wrapAround;
        glm::vec3 wrapRGB;
        
        
        bool        wireframe;
        int         wireframeLinewidth;
        std::string wireframeLinecap;
        std::string wireframeLinejoin;

        
        
        /** Output stream overloading */
        friend std::ostream &operator <<( std::ostream& os, const Material& obj ) {
            os << "Material {" << std::endl;
            os << "\tside               : " << &obj.side                << std::endl;
            os << "\topacity            : " << obj.opacity             << std::endl;
            os << "\ttransparent        : " << obj.transparent         << std::endl;
            os << "\tblending           : " << enum_to_int(obj.blending) << std::endl;
            os << "\tblendSrc           : " << obj.blendSrc            << std::endl;
            os << "\tblendDst           : " << obj.blendDst            << std::endl;
            os << "\tblendEquation      : " << obj.blendEquation       << std::endl;
            os << "\tdepthTest          : " << obj.depthTest           << std::endl;
            os << "\tdepthWrite         : " << obj.depthWrite          << std::endl;
            os << "\tpolygonOffset      : " << obj.polygonOffset       << std::endl;
            os << "\tpolygonOffsetFactor: " << obj.polygonOffsetFactor << std::endl;
            os << "\tpolygonOffsetUnits : " << obj.polygonOffsetUnits  << std::endl;
            os << "\talphaTest          : " << obj.alphaTest           << std::endl;
            os << "\toverdraw           : " << obj.overdraw            << std::endl;
            os << "\tvisible            : " << obj.visible             << std::endl;
            os << "\tneedsUpdate        : " << obj.needsUpdate         << std::endl;
            os << "}";
            return os;
        }
        
    };
}
#endif /* defined(__Three_cpp_Rev_2__Material__) */
