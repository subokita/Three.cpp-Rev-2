//
//  Mesh.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 09/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Mesh.h"
#include "three.h"

using namespace std;

namespace three {
    ptr<Mesh> Mesh::create(){
        return make_shared<Mesh>();
    }
    
    ptr<Mesh> Mesh::create(ptr<Geometry> geometry, ptr<Material> material) {
        return make_shared<Mesh>( Mesh(geometry, material) );
    }
    
    Mesh::Mesh()
    {}
    
    Mesh::Mesh( ptr<Geometry> geometry, ptr<Material> material ) :
        geometry            ( geometry ),
        material            ( material ),
        glBuffersInitialized(false),
        bufferIDs           ( vector<GLuint>(4, 0) )
    {}
    
    Mesh::~Mesh() {
        if( glBuffersInitialized )
            glDeleteBuffers(4, &bufferIDs[0] );
    }
    
    
    void Mesh::setUniforms(ptr<ShaderLib> shader_lib, bool gamma ) {
        auto shader = shader_lib->getShader();
        shader->setUniform( "model_mat", matrixWorld );
        
        if( material->wrapAround )
            shader->setUniform("wrapRGB", material->wrapRGB );
        
        if( texture != nullptr )
            texture->setUniforms(shader_lib, gamma );
        
        if( normalMap != nullptr )
            normalMap->setUniforms( shader_lib, gamma );
        
        if( specularMap != nullptr )
            specularMap->setUniforms( shader_lib, gamma );
        
        if( envMap != nullptr )
            envMap->setUniforms( shader_lib, gamma );
        
        
        material->setUniforms( shader_lib, gamma );
    }
    
    void Mesh::initGLBuffers() {
    
        geometry->noOfElements = static_cast<int>( geometry->faces.size() * 3);
        
        std::vector<unsigned short> index;
        std::vector<glm::vec3> normals( geometry->vertices.size() );
        std::vector<glm::vec2> uvs( geometry->vertices.size() );
    
        for( ptr<Face3> face: geometry->faces ) {
            index.push_back( face->a );
            index.push_back( face->b );
            index.push_back( face->c );
            
            normals[face->a] = face->vertexNormals[0];
            normals[face->b] = face->vertexNormals[1];
            normals[face->c] = face->vertexNormals[2];
            
            uvs[face->a] = face->uvs[0];
            uvs[face->b] = face->uvs[1];
            uvs[face->c] = face->uvs[2];
        }
       
//        for( auto& vert: geometry->vertices)
//            vert = vert * geometry->quaternion;
//        
//        for( auto& normal: normals)
//            normal = normal * geometry->quaternion;
        
        
        //FIXME: Indexed VBO messes the uv for textures
        glGenBuffers( 4, &bufferIDs[0] );
        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[0] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * geometry->vertices.size(), &geometry->vertices[0], GL_STATIC_DRAW );
        
        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[1] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * normals.size(), &normals[0], GL_STATIC_DRAW );
        
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bufferIDs[2] );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned short ) * geometry->faces.size() * 3, &index[0], GL_STATIC_DRAW );
        
        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[3] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec2 ) * uvs.size(), &uvs[0], GL_STATIC_DRAW );
        
        glBuffersInitialized = true;
        
        geometry->vertices.clear();
    }
    
    void Mesh::draw() {
        
        glPolygonMode( GL_FRONT_AND_BACK, material->wireframe ? GL_LINE : GL_FILL );
        glLineWidth( material->wireframeLinewidth );
        
        if( material->side == SIDE::DOUBLE_SIDE )  {
            glDisable( GL_CULL_FACE );
        }
        else {
            glEnable( GL_CULL_FACE );
            glCullFace( GL_BACK );
        }
        
        /* Vertices */
        glEnableVertexAttribArray( 0 );
        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[0] );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );
        
        /* Normals */
        glEnableVertexAttribArray( 1 );
        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[1] );
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );
        
        /* UVs */
        glEnableVertexAttribArray( 2 );
        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[3] );
        glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0 );
        
        /* Indices */
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bufferIDs[2] );
        glDrawElements( GL_TRIANGLES, geometry->noOfElements, GL_UNSIGNED_SHORT, (void*) 0);
        
        glDisableVertexAttribArray( 0 );
        glDisableVertexAttribArray( 1 );
        glDisableVertexAttribArray( 2 );
        glBindTexture( GL_TEXTURE_2D, 0 );
    }
}