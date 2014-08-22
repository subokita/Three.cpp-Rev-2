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

#include "../internal_headers.h"
#include "MathUtils.h"
#include "../interfaces/HasID.h"
#include "../interfaces/HasShaderUniforms.h"

using namespace std;

namespace three {
    static int MaterialIDCount = 0;
    
    class Material : public HasID, public HasShaderUniforms {
    public:
        Material();
        virtual ~Material();
        
        /** Output stream overloading */
        friend std::ostream &operator <<( std::ostream& os, const Material& obj ) {
            os << "Material {" << std::endl;
            os << "\tside               : " << &obj.side                << std::endl;
            os << "\topacity            : " << obj.opacity             << std::endl;
            os << "\ttransparent        : " << obj.transparent         << std::endl;
            os << "\tblending           : " << static_cast<int>(obj.blending) << std::endl;
            os << "\tblendSrc           : " << obj.blendSrc            << std::endl;
            os << "\tblendDst           : " << obj.blendDst            << std::endl;
            os << "\tblendEquation      : " << obj.blendEquation       << std::endl;
            os << "\tpolygonOffset      : " << obj.polygonOffset       << std::endl;
            os << "\tpolygonOffsetFactor: " << obj.polygonOffsetFactor << std::endl;
            os << "\tpolygonOffsetUnits : " << obj.polygonOffsetUnits  << std::endl;
            os << "\talphaTest          : " << obj.alphaTest           << std::endl;
            os << "\tvisible            : " << obj.visible             << std::endl;
            os << "\tneedsUpdate        : " << obj.needsUpdate         << std::endl;
            os << "}";
            return os;
        }
        
        const POLYGON_MODE getPolygonMode();
        bool isVisible();
        bool requiresUpdate();
        bool doesWrapAround();
        
        int getLineWidth();
        const SIDE getSide();
        const glm::vec3 getWrapRGB();
        float getAlphaTest();
        const SHADING getShading();
        float getOpacity();
        bool isTransparent();
        const BLENDING_MODE getBlendingMode();
        
        void setLineWidth( float width );
        void setPolygonMode( POLYGON_MODE mode );
        void setSide(SIDE side);
        void setVisible( bool flag );
        void setRequiresUpdate( bool flag );
        void setWrapAround( bool flag );
        void setWrapRGB( const glm::vec3 wrap_rgb );
        void setAlphaTest(const float alpha);
        void setShading( SHADING shading );
        void setOpacity( float opacity );
        void setTransparent( bool flag );
        void setBlendingMode( BLENDING_MODE mode );

    protected:
        SIDE side;
        
        POLYGON_MODE polygonMode;
        float       lineWidth;
        std::string wireframeLinecap;
        std::string wireframeLinejoin;

        bool  polygonOffset;
        float polygonOffsetFactor;
        float polygonOffsetUnits;
        
        bool  visible;
        bool  needsUpdate;
        bool  wrapAround;
        glm::vec3 wrapRGB;
        
        int   blendSrc;
        int   blendDst;
        int   blendEquation;
        float alphaTest;
        
        SHADING shading;
        
        float opacity;
        bool  transparent;
        BLENDING_MODE blending;
    };
}
#endif /* defined(__Three_cpp_Rev_2__Material__) */
