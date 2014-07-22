//
//  RenderTarget.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 22/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "RenderTarget.h"
#include "three.h"

using namespace std;

namespace three {
    
    ptr<RenderTarget> RenderTarget::create( GLenum target, GLuint framebuffer ){
        return make_shared<RenderTarget>(RenderTarget(target, framebuffer));
    }
    
    RenderTarget::RenderTarget( GLenum target, GLuint framebuffer ) :
        target      ( target ),
        framebuffer ( framebuffer )
    {}
    
    RenderTarget::~RenderTarget(){}
    
    void RenderTarget::bind() {
        glBindFramebuffer(target, framebuffer);
    }
}