//
//  RenderTarget.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 22/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "RenderTarget.h"


namespace three {
    /**
     * Create a new RenderTarget instance
     * @param   target      the framebuffer render target, commonly GL_FRAMEBUFFER
     * @param   framebuffer the framebuffer ID, 0 if it's not using already generated framebuffer
     * @return  the pointer to new render target
     */
    ptr<RenderTarget> RenderTarget::create( GLenum target, GLuint framebuffer ){
        return make_shared<RenderTarget>(RenderTarget(target, framebuffer));
    }
    
    /**
     * RenderTarget constructor
     * @param   target      the framebuffer render target, commonly GL_FRAMEBUFFER
     * @param   framebuffer the framebuffer ID, 0 if it's not using already generated framebuffer
     */
    RenderTarget::RenderTarget( GLenum target, GLuint framebuffer ) :
        target      ( target ),
        framebuffer ( framebuffer )
    {}
    
    
    /**
     * RenderTarget destructor
     */
    RenderTarget::~RenderTarget(){
        if( framebuffer > 0 )
            glDeleteFramebuffers(1, &framebuffer);
    }
    
    /**
     * Generate a new frame buffer
     * @param   target
     * @param   framebuffer
     */
    void RenderTarget::generateFrameBuffer() {
        glGenFramebuffers(1, &framebuffer);
    }
    
    /**
     * Bind the framebuffer as render target
     */
    void RenderTarget::bind() {
        glBindFramebuffer(target, framebuffer);
    }
    
    /**
     * Unbind the framebuffer
     */
    void RenderTarget::unbind() {
        glBindFramebuffer(target, 0);
    }
}