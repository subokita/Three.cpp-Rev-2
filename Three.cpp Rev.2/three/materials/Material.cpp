//
//  Material.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Material.h"
#include "../Constants.h"

namespace three {
    Material::Material() :
        HasID(MaterialIDCount++)
    {
        // FIXME: should be in initializer list
        
        side                = SIDE::FRONT_SIDE;
        shading             = SHADING::SMOOTH_SHADING;
        
        opacity             = 1.0;
        transparent         = false;
        
        blending            = BLENDING_MODE::NORMAL;
        blendSrc            = three::SrcAlphaFactor;
        blendDst            = three::OneMinusSrcAlphaFactor;
        blendEquation       = three::AddEquation;
        
        polygonOffset       = false;
        polygonOffsetFactor = 0.0;
        polygonOffsetUnits  = 0.0;
        
        alphaTest           = 0.0;
        
        visible             = true;
        needsUpdate         = true;
        
        polygonMode         = POLYGON_MODE::POLYGON;
        lineWidth           = 1;
        wireframeLinecap    = "round";
        wireframeLinejoin   = "round";
        
        wrapAround          = false;
        wrapRGB             = glm::vec3(1.0, 1.0, 1.0);
    }
    
    Material::~Material() {
        
    }
    
    void Material::setPolygonMode(three::POLYGON_MODE mode) {
        this->polygonMode = mode;
    }
    
    const POLYGON_MODE Material::getPolygonMode() {
        return polygonMode;
    }
    
    int Material::getLineWidth() {
        return lineWidth;
    }
    
    void Material::setLineWidth( float width ) {
        lineWidth = width;
    }
    
    const SIDE Material::getSide() {
        return side;
    }
    
    void Material::setSide(SIDE side) {
        this->side = side;
    }
    bool Material::isVisible() {
        return visible;
    }
    
    void Material::setVisible( bool flag ) {
        this->visible = flag;
    }
    
    bool Material::requiresUpdate() {
        return this->needsUpdate;
    }
    
    void Material::setRequiresUpdate( bool flag ) {
        this->needsUpdate = flag;
    }

    bool Material::doesWrapAround() {
        return wrapAround;
    }
    
    void Material::setWrapAround(bool flag ) {
        this->wrapAround = flag;
    }
    
    const glm::vec3 Material::getWrapRGB() {
        return wrapRGB;
    }
    
    void Material::setWrapRGB( const glm::vec3 wrap_rgb ) {
        this->wrapRGB = wrap_rgb;
    }
    
    float Material::getAlphaTest() {
        return this->alphaTest;
    }
    
    void Material::setAlphaTest(const float alpha) {
        this->alphaTest = alpha;
    }
    
    const SHADING Material::getShading() {
        return this->shading;
    }
    
    void Material::setShading( SHADING shading ) {
        this->shading = shading;
    }
    
    float Material::getOpacity() {
        return this->opacity;
    }
    
    void Material::setOpacity( float opacity ) {
        this->opacity = opacity;
    }
    
    bool Material::isTransparent() {
        return this->transparent;
    }
    
    void Material::setTransparent( bool flag ) {
        this->transparent = flag;
    }
    const BLENDING_MODE Material::getBlendingMode() {
        return this->blending;
    }
    
    void Material::setBlendingMode( BLENDING_MODE mode ) {
        this->blending = mode;
    }
}