//
//  Mesh.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 09/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Mesh.h"
#include "MeshPhongMaterial.h"

using namespace std;

namespace  three {
    
    PTR(Mesh) Mesh::create(){
        return make_shared<Mesh>();
    }
    
    PTR(Mesh) Mesh::create(PTR(Geometry) geometry, PTR(Material) material) {
        return make_shared<Mesh>( Mesh(geometry, material) );
    }
    
    Mesh::Mesh()
    {}
    
    Mesh::Mesh( PTR(Geometry) geometry, PTR(Material) material ) :
        geometry( geometry ),
        material( material )
    {}
    
    Mesh::~Mesh()
    {}
    
    void Mesh::draw(PTR(Shader) shader, bool gamma ) {
        
        glPolygonMode( GL_FRONT_AND_BACK, material->wireframe ? GL_LINE : GL_FILL );
        glLineWidth( material->wireframeLinewidth );
        
        PTR(MeshPhongMaterial) phong_material;
        if( INSTANCE_OF(material, MeshPhongMaterial )) {
            phong_material = DOWNCAST(material, MeshPhongMaterial );
            
            shader->setUniform( "opacity",   phong_material->opacity );
            shader->setUniform( "emissive",  phong_material->emissive, 1.0, gamma );
            shader->setUniform( "ambient",   phong_material->ambient, 1.0, gamma );
            shader->setUniform( "diffuse",   phong_material->color, 1.0, gamma );
            shader->setUniform( "specular",  phong_material->specular, 1.0, gamma );
            shader->setUniform( "shininess", phong_material->shininess );
            shader->setUniform( "model_mat", matrixWorld );
        }
        
        if( texture == nullptr ) {
            if( emptyWhiteTexture == nullptr )
                emptyWhiteTexture = TextureUtils::loadEmptyWhiteTexture();
            texture = emptyWhiteTexture;
        }
        
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, texture->textureID );
        shader->setUniform("map", 0);
        
        /* Vertices */
        glEnableVertexAttribArray( 0 );
        glBindBuffer( GL_ARRAY_BUFFER, geometry->bufferIDs[0] );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );
        
        /* Normals */
        glEnableVertexAttribArray( 1 );
        glBindBuffer( GL_ARRAY_BUFFER, geometry->bufferIDs[1] );
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );
        
        /* UVs */
        glEnableVertexAttribArray( 2 );
        glBindBuffer( GL_ARRAY_BUFFER, geometry->bufferIDs[3] );
        glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0 );
        
        /* Indices */
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, geometry->bufferIDs[2] );
        glDrawElements( GL_TRIANGLES, geometry->noOfElements, GL_UNSIGNED_SHORT, (void*) 0);
        
        glDisableVertexAttribArray( 0 );
        glDisableVertexAttribArray( 1 );
        glDisableVertexAttribArray( 2 );
        glBindTexture( GL_TEXTURE_2D, 0 );
    }
}