//
//  FontStashAdapter.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 17/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__FontStashAdapter__
#define __Three_cpp_Rev_2__FontStashAdapter__

#include <iostream>

#include "fontstash.h"
#include "internal_headers.h"
#include "Color.h"
#include "Rect.h"

namespace three {
    class TextInfo;
    
    /**
     * An adapter class for FontStash library,
     * this replaces glfontstash.h, since glfonstash uses pre OpenGL 3.0 syntax (e.g. no shaders)
     * Special thanks to Mikko Mononen for providing fontstash library at https://github.com/memononen/fontstash/
     */
    class FontStashAdapter{
    public:
        FontStashAdapter();
        ~FontStashAdapter();
        
        void init(int width, int height, int flags, Rect& viewport);
        void render();
        void addFont( const std::string& font_name, const std::string& filename );
        unsigned int addText( std::string text, int x, int y,
                              std::string font_name, Color color, float font_size, float spacing, float blur );
        void setText( unsigned int text_entry_ID, std::string text );
        void setText( unsigned int text_entry_ID, std::string text, int x, int y );
        void setText( unsigned int text_entry_ID, std::string text, int x, int y,
                      std::string font_name, Color color, float font_size, float spacing, float blur );
        void clearText();
        
        bool isInitialized();
        
        static int renderCreate( void* user_ptr, int width, int height );
        static int renderResize(void* user_ptr, int width, int height);
        static void renderUpdate(void* user_ptr, int* rect, const unsigned char* data);
        static void renderDraw(void* user_ptr, const float* verts, const float* tcoords, const unsigned int* colors, int nverts);
        static void renderDelete(void* user_ptr);
        
    protected:
        std::vector<TextInfo> texts;
        
        glm::mat4 orthogonalProjection;
        
        Rect viewportSize;
        ptr<ShaderLib> shaderLib;
        GLuint bufferIDs[3];
        GLuint textureID;
        int width;
        int height;
        bool initialized;
        
        static FontStashAdapter* instance;
        FONScontext * context;
    };
    
    
    /**
     * Internally used datastructure to store the string, coordinate, and other font related parameters
     */
    class TextInfo {
    public:
        TextInfo( std::string text, int x, int y, int font_id, Color color, float font_size, float spacing = 0.0f, float blur = 0.0f );
        ~TextInfo();
        
    public:
        friend class FontStashAdapter;
        
    protected:
        std::string text;
        int x;
        int y;
        
        float spacing;
        float blur;
        int fontID;
        unsigned int color;
        float fontSize;
    };

}

#endif /* defined(__Three_cpp_Rev_2__FontStashAdapter__) */
