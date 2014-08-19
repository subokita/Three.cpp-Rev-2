//
//  FontStashAdapter.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 17/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "FontStashAdapter.h"
#include "Utils.h"
#include "ShaderLib.h"
#include "ShaderLibraries.h"
#include "Shader.h"

#define FONTSTASH_IMPLEMENTATION
#include "fontstash.h"


namespace three {
    TextInfo::TextInfo( std::string text, int x, int y, int font_id,
                        Color color, float font_size, float spacing, float blur ) :
        text    ( text ),
        x       ( x ),
        y       ( y ),
        fontID  ( font_id ),
        color   ( Utils::rgbaTo32Bit(color) ),
        fontSize( font_size ),
        spacing ( spacing ),
        blur    ( blur )
    {}
    
    TextInfo::~TextInfo() {}
    
    
    FontStashAdapter* FontStashAdapter::instance = nullptr;
    
    FontStashAdapter::FontStashAdapter() :
        initialized( false ),
        context    ( NULL )
    {}
    
    /**
     * Check if FontStashAdapter is initialized
     */
    bool FontStashAdapter::isInitialized() {
        return initialized;
    }
    
    FontStashAdapter::~FontStashAdapter() {
        if( context != NULL )
            fonsDeleteInternal(context);
        
        instance = nullptr;
    }
    
    /**
     * Initialize FontStashAdapter, it will initialize necessary data structure for fonstate.h, 
     * generate GL buffers, setting the matrix, etc
     */
    void FontStashAdapter::init(int width, int height, int flags, Rect& viewport) {
        this->instance             = this;
        this->viewportSize         = viewport;
        this->orthogonalProjection = glm::ortho(0.0f, viewportSize.width, viewportSize.height, 0.0f, -1.0f, 1.0f );
        
        glGenBuffers(3, this->bufferIDs);
        this->shaderLib = SHADERLIB_FONT_PASS->clone();
        this->shaderLib->compileShader();
        
        FONSparams params;
        memset(&params, 0, sizeof(params));
        
        params.width    = width;
        params.height   = height;
        params.flags    = (unsigned char)flags;
        params.userPtr  = NULL; /* No user data is needed, we're going to refer to this instance's data member */
        
        /* Function callback handlers */
        params.renderCreate = FontStashAdapter::renderCreate;
        params.renderResize = FontStashAdapter::renderResize;
        params.renderUpdate = FontStashAdapter::renderUpdate;
        params.renderDraw   = FontStashAdapter::renderDraw;
        params.renderDelete = FontStashAdapter::renderDelete;

        context  = fonsCreateInternal(&params);
        
        initialized = true;
    }
    
    /**
     * Add and load a new font
     */
    void FontStashAdapter::addFont( const std::string& font_name, const std::string& filename ) {
        if( fonsAddFont(context, font_name.c_str(), filename.c_str() ) == FONS_INVALID )
            throw runtime_error( "Unable to load font: " + filename );
    }
    
    /**
     * Add text into the draw list
     */
    unsigned int FontStashAdapter::addText( std::string text, int x, int y,
                                    std::string font_name, Color color, float font_size, float spacing, float blur ) {
        /* Remember where the index of this text in the vector */
        unsigned int text_entry_id = static_cast<unsigned int>(texts.size());
        
        int font_ID = fonsGetFontByName(context, font_name.c_str() );
        texts.push_back( TextInfo( text, x, y, font_ID, color, font_size, spacing, blur ) );
        
        return text_entry_id;
    }
    
    /**
     * Update the existing text
     */
    void FontStashAdapter::setText( unsigned int text_entry_ID, std::string text ) {
        texts[text_entry_ID].text = text;
    }
    
    /**
     * Update the existing text and its coordinate
     */
    void FontStashAdapter::setText( unsigned int text_entry_ID, std::string text, int x, int y  ) {
        texts[text_entry_ID].text = text;
        texts[text_entry_ID].x    = x;
        texts[text_entry_ID].y    = y;
    }
    
    /**
     * Update the existing text, coordinate of the text, and also other parameters
     */
    void FontStashAdapter::setText( unsigned int text_entry_ID, std::string text, int x, int y,
                                    std::string font_name, Color color, float font_size, float spacing, float blur ) {
        texts[text_entry_ID].text       = text;
        texts[text_entry_ID].x          = x;
        texts[text_entry_ID].y          = y;
        texts[text_entry_ID].fontID     = fonsGetFontByName(context, font_name.c_str() );
        texts[text_entry_ID].color      = Utils::rgbaTo32Bit(color);
        texts[text_entry_ID].fontSize   = font_size;
        texts[text_entry_ID].spacing    = spacing;
        texts[text_entry_ID].blur       = blur;
    }
    
    
    /**
     * Clear all the texts in the draw list
     */
    void FontStashAdapter::clearText() {
        texts.clear();
    }
    
    
    /**
     * Create function, generates and binds necessary texture, invoked by fonstate.h
     */
    int FontStashAdapter::renderCreate( void* user_ptr, int width, int height ) {
        if( instance->textureID != 0 ){
            glDeleteTextures(1, &instance->textureID);
            instance->textureID = 0;
        }
        
        glGenTextures(1, &instance->textureID);
        if (!instance->textureID)
            return 0;
        
        instance->width  = width;
        instance->height = height;
        glActiveTexture ( GL_TEXTURE0 );
        glBindTexture   ( GL_TEXTURE_2D, instance->textureID);
        glTexImage2D    ( GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
        return 1;
    }
    
    /**
     * Resize function, invoked by fonstate.h
     */
    int FontStashAdapter::renderResize(void* user_ptr, int width, int height){
        return renderCreate(user_ptr, width, height);
    }
    
    /**
     * Update function, invoked by fonstate.h
     */
    void FontStashAdapter::renderUpdate(void* user_ptr, int* rect, const unsigned char* data){
        int w = rect[2] - rect[0];
        int h = rect[3] - rect[1];
        
        if (instance->textureID == 0)
            return;
        
        glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture(GL_TEXTURE_2D, instance->textureID );
        glPixelStorei(GL_UNPACK_ALIGNMENT,1);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, instance->width);
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, rect[0]);
        glPixelStorei(GL_UNPACK_SKIP_ROWS, rect[1]);
        glTexSubImage2D(GL_TEXTURE_2D, 0, rect[0], rect[1], w, h, GL_RED, GL_UNSIGNED_BYTE, data);
        glPopClientAttrib();
    }
    
    /**
     * Render / draw function, invoked by fonstate.h
     */
    void FontStashAdapter::renderDraw(void* user_ptr, const float* verts, const float* tcoords, const unsigned int* colors, int nverts){
        
        if( instance->textureID == 0 )
            return;
        
        glBindBuffer( GL_ARRAY_BUFFER, instance->bufferIDs[0] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * 2 * nverts, verts, GL_STATIC_DRAW );
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0 );
        
        glBindBuffer( GL_ARRAY_BUFFER, instance->bufferIDs[1] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * 2 * nverts, tcoords, GL_STATIC_DRAW );
        glEnableVertexAttribArray( 1 );
        glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0 );
        
        /* It's unsigned int, but we're going to chop it automatically by stating that it's passed in as GL_UNSIGNED_BYTE */
        glBindBuffer( GL_ARRAY_BUFFER, instance->bufferIDs[2] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( unsigned int ) * nverts, colors, GL_STATIC_DRAW );
        glEnableVertexAttribArray( 2 );
        glVertexAttribPointer( 2, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, (void*) 0 );
        
        glDrawArrays(GL_TRIANGLES, 0, nverts);
        
        glDisableVertexAttribArray( 0 );
        glDisableVertexAttribArray( 1 );
        glDisableVertexAttribArray( 2 );
    }
    
    /**
     * Cleanup function, deletes the texture and buffers, invoked by fonstate.h
     */
    void FontStashAdapter::renderDelete(void* user_ptr) {
        if( instance->textureID != 0 )
            glDeleteTextures(1, &instance->textureID);
        
        if( instance->bufferIDs[0] != 0 )
            glDeleteBuffers(3, instance->bufferIDs);
    }
    
    /**
     * Render the font, called by Renderer.cpp
     */
    void FontStashAdapter::render() {
        glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_CULL_FACE);
        glFrontFace( GL_CCW );
        
        shaderLib->bind();
        shaderLib->getShader()->setUniform("ortho", orthogonalProjection );
        
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D , textureID );
        shaderLib->getShader()->setUniform("font", 0 );
        fonsClearState(context);
        
        for( auto text_info: texts ){
            fonsSetFont   ( context, text_info.fontID );
            fonsSetSize   ( context, text_info.fontSize );
            fonsSetColor  ( context, text_info.color );
            fonsSetSpacing( context, text_info.spacing );
            fonsSetBlur   ( context, text_info.blur );
            fonsDrawText  ( context, text_info.x, text_info.y, text_info.text.c_str(), NULL );
        }
        
        shaderLib->unbind();
    }
    
    
}