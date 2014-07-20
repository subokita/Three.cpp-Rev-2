//
//  TextureUtils.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 16/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "TextureUtils.h"
#include "three.h"

using namespace std;

namespace three {
    
    FIBITMAP * TextureUtils::loadImage( const std::string path, const std::string filename ) {
        const char * file = (path + filename).c_str();
        
        FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(file), file);
        if( bitmap == NULL )
            throw runtime_error( "Unable to load file" );
        
        return FreeImage_ConvertTo24Bits( bitmap );
    }
        
    ptr<NormalTexture> TextureUtils::loadAsNormalMap( const std::string path, const std::string filename ) {
        FIBITMAP * bitmap = loadImage(path, filename);
        
        if( FreeImage_GetColorType( bitmap ) != FIC_RGB ) {
            throw runtime_error("Unable to load normal map which is of not RGB type");
            FreeImage_Unload(bitmap);
        }
        
        ptr<NormalTexture> map = make_shared<NormalTexture>();
        
        map->width  = FreeImage_GetWidth (bitmap);
        map->height = FreeImage_GetHeight(bitmap);
        
        size_t size = map->width * map->height;
        
        vector<glm::vec3> normals(size, glm::vec3(0.0));
        BYTE* data = FreeImage_GetBits( bitmap );
        
        for( int i = 0, j = 0; i < size * 3; i+=3, j++ ) {
            normals[j].x = static_cast<int>(data[i  ]) / 255.0;
            normals[j].y = static_cast<int>(data[i+1]) / 255.0;
            normals[j].z = static_cast<int>(data[i+2]) / 255.0;
        }
        
        FreeImage_Unload( bitmap );

        glGenTextures(1, &map->textureID);
        glBindTexture( GL_TEXTURE_2D, map->textureID );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, map->width, map->height, 0, GL_BGR, GL_FLOAT, &normals[0]);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        return map;
    }
    
    ptr<Texture> TextureUtils::loadImageAsTexture( const std::string path, const std::string filename ) {
        FIBITMAP* bitmap = loadImage( path, filename );
        ptr<Texture> texture = make_shared<Texture>();
        texture->width  = FreeImage_GetWidth (bitmap);
        texture->height = FreeImage_GetHeight(bitmap);
        
        glGenTextures(1, &texture->textureID);
        glBindTexture( GL_TEXTURE_2D, texture->textureID );
        
        FREE_IMAGE_COLOR_TYPE color_type = FreeImage_GetColorType( bitmap );
        
        if( color_type == FIC_RGB ) {
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, GL_BGR, GL_UNSIGNED_BYTE, FreeImage_GetBits( bitmap ));
        }
        else if ( color_type == FIC_PALETTE ) {
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, GL_BGR, GL_UNSIGNED_BYTE, FreeImage_GetBits( bitmap ));
        }
        else if( color_type == FIC_RGBALPHA ) {
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, FreeImage_GetBits( bitmap ));
        }
        else {
            FreeImage_Unload( bitmap );
            throw runtime_error( "Unhandled case of image type" );
        }
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        FreeImage_Unload( bitmap );
        
        return texture;
    }
    
    
    ptr<Texture> TextureUtils::loadEmptyWhiteTexture() {
        ptr<Texture> texture = make_shared<Texture>();
        texture->width  = 1;
        texture->height = 1;

        glGenTextures(1, &texture->textureID);
        glBindTexture( GL_TEXTURE_2D, texture->textureID );
        
        vector<GLubyte> empty_data(3, 255);
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, GL_BGR, GL_UNSIGNED_BYTE, &empty_data[0]);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        return texture;
    }

}