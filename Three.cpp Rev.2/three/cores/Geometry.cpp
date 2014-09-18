//
//  Geometry.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Geometry.h"
#include "Face3.h"
#include "MathUtils.h"

#include <glm/gtc/matrix_transform.hpp>
#include <sstream>

using std::stringstream;
using std::map;
using std::string;
using std::vector;

namespace three {
    ptr<Geometry> Geometry::create() {
        return make_shared<Geometry>();
    }
    
    Geometry::Geometry() :
        noOfElements(0)
    {
    }
    
    Geometry::~Geometry() {
    }
    
    void Geometry::addVertex( glm::vec3 vertex ) {
        vertices.push_back(vertex);
    }
    
    void Geometry::addFace( ptr<Face3> face ) {
        faces.push_back( face );
    }
    
    void Geometry::init() {
        /* does nothing */
    }
    
    
    void Geometry::addVertices( std::vector<glm::vec3>& vertices ) {
        this->vertices.insert( this->vertices.end(), vertices.begin(), vertices.end() );
    }
    
    void Geometry::addFaces( std::vector<ptr<Face3>>& faces ) {
        this->faces.insert( this->faces.end(), faces.begin(), faces.end() );
    }
    
    const std::vector<ptr<Face3>>& Geometry::getFaces() {
        return this->faces;
    }
    
    const std::vector<glm::vec3>& Geometry::getVertices() {
        return this->vertices;
    }
    
    
    void Geometry::setNoOfElements( int size ) {
        this->noOfElements = size;
    }
    
    int Geometry::getNoOfElements() const {
        return this->noOfElements;
    }
    
    void Geometry::computeFaceNormals() {
        for( ptr<Face3> face: faces ) {
            glm::vec3 a = this->vertices[face->a];
            glm::vec3 b = this->vertices[face->b];
            glm::vec3 c = this->vertices[face->c];
            face->setFaceNormal(glm::normalize( glm::cross(c - b, a - b) ));
        }
    }
    
    
    void Geometry::computeVertexNormals( bool area_weighted ) {
        vector<glm::vec3> vertices( this->vertices.size() );
        
        if( area_weighted ) {
            for( ptr<Face3> face : faces ) {
                glm::vec3& a = this->vertices[face->a];
                glm::vec3& b = this->vertices[face->b];
                glm::vec3& c = this->vertices[face->c];
                
                glm::vec3 cb = glm::cross(c - b, a - b);
                
                vertices[face->a] += cb;
                vertices[face->b] += cb;
                vertices[face->c] += cb;
            }
        }
        else {
            for( ptr<Face3> face : faces ) {
                const glm::vec3& normal = face->getFaceNormal();
                vertices[face->a] += normal;
                vertices[face->b] += normal;
                vertices[face->c] += normal;
            }
        }
        
        for( glm::vec3& vertex : vertices )
            vertex = glm::normalize( vertex );
        
        for( ptr<Face3> face: faces ) {
            face->setVertexNormals({
                vertices[face->a],
                vertices[face->b],
                vertices[face->c]
            });
        }
    }
    
    
    void Geometry::mergeVertices() {
        float precision_points = 4;
        float precision = pow( 10, precision_points );
        
        map<string, int> vertices_map;
        vector<glm::vec3> unique;
        vector<int> changes(vertices.size(), 0);
        
        for( int i = 0; i < vertices.size(); i++ ){
            auto v = vertices[i];
            stringstream ss;
            ss << round( v.x * precision ) << "_" << round( v.y * precision ) << "_" << round( v.z * precision ) ;
            string key = ss.str();
            
            if( vertices_map.count(key) == 0 ){
                vertices_map[key] = i;
                unique.push_back( vertices[i] );
                changes[i] = static_cast<int>(unique.size() - 1);
            }
            else {
                changes[i] = changes[ vertices_map[key] ];
            }
        }
        
        vector<int> face_indices_to_be_removed;
        for( int i = 0; i < faces.size(); i++ ) {
            ptr<Face3> face = faces[i];
            
            face->a = changes[face->a];
            face->b = changes[face->b];
            face->c = changes[face->c];
            
            vector<int> indices = { face->a, face->b, face->c };
            
            int dup_index = -1;
            for( int n = 0; n < 3; n++ ) {
                if( indices[n] == indices[ (n+1)%3 ] ) {
                    dup_index = n;
                    face_indices_to_be_removed.push_back( i );
                    break;
                }
            }
        }
        
        for( int i = (int) face_indices_to_be_removed.size() - 1; i > -1; i-- ) {
            int index = face_indices_to_be_removed[i];
            faces.erase( faces.begin() + index );
        }
        
        this->vertices = unique;
    }

    void Geometry::computeVertexNormalsForNonMergedFaces() {
        for( int i = 0; i < faces.size(); i++ ){
            ptr<Face3> face_1 = faces[i];
            
            vector<glm::vec3> vertices_1 = {
                vertices[face_1->a],
                vertices[face_1->b],
                vertices[face_1->c]
            };
            
            for( int j = i + 1; j < faces.size(); j++ ){
                ptr<Face3> face_2 = faces[j];
                
                vector<glm::vec3> vertices_2 = {
                    vertices[face_2->a],
                    vertices[face_2->b],
                    vertices[face_2->c]
                };
                
                /* If they same the same vertices, then we average the normal vectors (by summing and normalizing) */
                for( int a = 0; a < 3; a++ ) {
                    for( int b = 0; b < 3; b++ ) {
                        if( MathUtils::equals(vertices_1[a], vertices_2[b], 3)) {
                            face_1->vertexNormals[a] += face_2->vertexNormals[b];
                            face_2->vertexNormals[b] += face_1->vertexNormals[a];
                        }
                    }
                }
            }
        }
        
        /* Normalizing the vertex normals */
        for( auto face: faces ) {
            face->setVertexNormals({
                glm::normalize(face->vertexNormals[0]),
                glm::normalize(face->vertexNormals[1]),
                glm::normalize(face->vertexNormals[2])
            });
        }
    }
}