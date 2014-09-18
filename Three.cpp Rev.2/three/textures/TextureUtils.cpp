//
//  TextureUtils.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 16/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "TextureUtils.h"
#include "EnvMap.h"
#include "NormalMap.h"
#include "SpecularMap.h"

using std::string;
using std::vector;

namespace three {
    
    ptr<fipImage> TextureUtils::loadImage( const string path, const string filename ) {
        // cout << path + "/" + filename << endl;
        
        ptr<fipImage> image = make_shared<fipImage>();
        
        if( image->load( (path + "/" + filename).c_str() ) == false ) {
            image->clear();
            string error_message = "Unable to load image " + path + "/" + filename;
            throw runtime_error( error_message );
        }
        return image;
    }
    
    
    ptr<EnvMap> TextureUtils::loadAsEnvMap( const string path,
                                            const string neg_x, const string neg_y, const string neg_z,
                                            const string pos_x, const string pos_y, const string pos_z ) {
        
        ptr<EnvMap> map = EnvMap::create();
        
        map->genTexture();
        map->bind();
        
        const std::string filenames[6] = {
            neg_x, neg_y, neg_z, pos_x, pos_y, pos_z
        };
        
        const static GLenum targets[6] = {
            GL_TEXTURE_CUBE_MAP_POSITIVE_X,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        };
        
        for ( int i = 0; i < 6; i++ ) {
            ptr<fipImage> image = loadImage(path, filenames[i]);
            
            image->flipVertical();
            image->flipHorizontal();
            
            map->width  = image->getWidth();
            map->height = image->getHeight();

            image->convertTo32Bits();
            glTexImage2D( targets[i], 0, GL_RGBA, map->width, map->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, image->accessPixels() );
            
            image = nullptr;
        }
        
        
        return map;
    }
    
    ptr<SpecularMap> TextureUtils::loadAsSpecularMap( const string path, const string filename ) {
        ptr<fipImage> image = loadImage(path, filename);
        ptr<SpecularMap> map = SpecularMap::create();
        
        map->width  = image->getWidth();
        map->height = image->getHeight();
        
        image->convertToGrayscale();
        
        size_t size = map->width * map->height;
        
        vector<float> speculars(size);
        BYTE* data = image->accessPixels();
        
        for( int i = 0; i < size; i++ ){
            speculars[i] = static_cast<float>(data[i]) / 255.0;
        }

        map->genTexture();
        map->bind();
        
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RED, map->width, map->height, 0, GL_RED, GL_FLOAT, &speculars[0]);
        
        glGenerateMipmap(GL_TEXTURE_2D);
        
        image->clear();
        image = nullptr;
        
        return map;
    }
    
    ptr<NormalMap> TextureUtils::loadAsNormalMap( const string path, const string filename ) {
        ptr<fipImage> image = loadImage(path, filename);
        ptr<NormalMap> map = NormalMap::create();
        
        map->width  = image->getWidth();
        map->height = image->getHeight();
        
        size_t size = map->width * map->height;
        
        image->convertTo24Bits();
        
        vector<glm::vec3> normals(size, glm::vec3(0.0));
        BYTE* data = image->accessPixels();
        
        for( int i = 0, j = 0; i < size * 3; i+=3, j++ ) {
            normals[j].x = static_cast<int>(data[i  ]) / 255.0;
            normals[j].y = static_cast<int>(data[i+1]) / 255.0;
            normals[j].z = static_cast<int>(data[i+2]) / 255.0;
        }

        map->genTexture();
        map->bind();
        
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, map->width, map->height, 0, GL_BGR, GL_FLOAT, &normals[0]);
        
        glGenerateMipmap(GL_TEXTURE_2D);
        
        image->clear();
        image = nullptr;
        
        return map;
    }
    
    
    /**
     * Load an image as a texture.
     * It will generate GL TEXTURE, and bind the image into it.
     */
    ptr<Texture> TextureUtils::loadAsTexture( const string path, const string filename ) {
        ptr<fipImage> image = loadImage(path, filename);
        
        ptr<Texture> texture = Texture::create();
        texture->width  = image->getWidth();
        texture->height = image->getHeight();

        texture->genTexture();
        texture->bind();
        
        
        switch( image->getColorType() ) {
            case FIC_RGB:
                image->convertTo32Bits();
                glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, image->accessPixels() );
                break;
                
            case FIC_PALETTE:
                image->convertTo32Bits();
                glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, image->accessPixels() );
                break;
                
            case FIC_RGBALPHA:
                glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, image->accessPixels() );
                break;
                
            default:
                throw runtime_error( "Unhandled image type" );
                break;
        }
        
        image->clear();
        
        glGenerateMipmap(GL_TEXTURE_2D);
        
        return texture;
    }
    
    /**
     * Load empty white texture, I don't think this is needed anymore
     */
    ptr<Texture> TextureUtils::loadEmptyWhiteTexture() {
        ptr<Texture> texture = Texture::create();
        texture->width  = 1;
        texture->height = 1;

        texture->genTexture();
        texture->bind();
        
        vector<GLubyte> empty_data(3, 255);
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, GL_BGR, GL_UNSIGNED_BYTE, &empty_data[0]);
        
        glGenerateMipmap(GL_TEXTURE_2D);
        
        return texture;
    }

}