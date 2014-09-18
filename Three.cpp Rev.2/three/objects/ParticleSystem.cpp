//
//  ParticleSystem.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 04/09/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "ParticleSystem.h"
#include "Geometry.h"
#include "Face3.h"
#include "Box3.h"
#include "Material.h"
#include "Texture.h"
#include "Shader.h"
#include "ShaderLib.h"

namespace three {
    
    ptr<ParticleSystem> ParticleSystem::create(ptr<Geometry> geometry, ptr<Material> material) {
        ptr<ParticleSystem> system = make_shared<ParticleSystem>();
        system->setGeometry( geometry );
        system->setMaterial( material );
        return system;
    }

    ParticleSystem::ParticleSystem() {
    }
    
    ParticleSystem::~ParticleSystem() {
        if( this->glBuffersInitialized ) {
            glDeleteBuffers(1, bufferIDs );
            this->glBuffersInitialized = false;
        }
    }
    
    void ParticleSystem::setUniforms(ptr<ShaderLib> shader_lib, bool gamma ){
        auto shader = shader_lib->getShader();
        shader->setUniform( "model_mat", matrixWorld );
        
        if( material->doesWrapAround() )
            shader->setUniform("wrapRGB", material->getWrapRGB() );
        
        if( texture != nullptr )
            texture->setUniforms(shader_lib, gamma );
        
        material->setUniforms( shader_lib, gamma );
    }
    
    void ParticleSystem::setSortParticles( bool flag ) {
        this->sortParticles = flag;
    }
    
    void ParticleSystem::sort(ptr<Camera> camera) {
        if( sortParticles ) {
            std::sort(geometry->vertices.begin(), geometry->vertices.end(), [&](glm::vec3& a, glm::vec3& b){
                return glm::length(camera->position - a) > glm::length(camera->position - b );
            });
        }
    }
    
    void ParticleSystem::initGLBuffers() {
        geometry->noOfElements = static_cast<int>(geometry->vertices.size());
        
        glGenBuffers( 1, bufferIDs );
        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[0] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * geometry->vertices.size(), &geometry->vertices[0], GL_STREAM_DRAW );
        
//        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[1] );
//        glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * normals.size(), &normals[0], GL_STATIC_DRAW );
//        
//        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bufferIDs[2] );
//        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned short ) * index.size(), &index[0], GL_STATIC_DRAW );
//        
//        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[3] );
//        glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec2 ) * uvs.size(), &uvs[0], GL_STATIC_DRAW );
//        
//        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[4] );
//        glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * colors.size(), &colors[0], GL_STREAM_DRAW );
        
        glBuffersInitialized = true;
        
        boundingBox = Box3::create();
        boundingBox->setFrom( geometry->vertices );
    }
    
    void ParticleSystem::setGLState() {
        glPolygonMode( GL_FRONT_AND_BACK, static_cast<GLint>(material->getPolygonMode()) );
        material->setGLState();
    }
    
    void ParticleSystem::draw() {
        /* Vertices */
        glEnableVertexAttribArray( 0 );
        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[0] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * geometry->vertices.size(), NULL, GL_STREAM_DRAW );
        glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( glm::vec3 ) * geometry->vertices.size(), &geometry->vertices[0] );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );
        
//        /* Normals */
//        glEnableVertexAttribArray( 1 );
//        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[1] );
//        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );
//        
//        /* UVs */
//        glEnableVertexAttribArray( 2 );
//        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[3] );
//        glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0 );
//        
//        /* Colors */
//        glEnableVertexAttribArray( 3 );
//        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[4] );
//        glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );
//        
//        /* Indices */
//        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bufferIDs[2] );
//        glDrawElements( GL_TRIANGLES, geometry->noOfElements, GL_UNSIGNED_SHORT, (void*) 0);
        glDrawArrays( GL_POINTS, 0, geometry->noOfElements );
        
        glDisableVertexAttribArray( 0 );
//        glDisableVertexAttribArray( 1 );
//        glDisableVertexAttribArray( 2 );
//        glDisableVertexAttribArray( 3 );
        glBindTexture( GL_TEXTURE_2D, 0 );
    }
}