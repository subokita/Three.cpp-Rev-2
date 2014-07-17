//
//  Geometry.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Geometry.h"
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>

using namespace std;

namespace three {
    Geometry::Geometry() :
        noOfElements(0),
        glBuffersInitialized(false)
    {}
    
    Geometry::~Geometry() {}
    
    void Geometry::computeFaceNormals() {
        for( PTR(Face3) face: faces ) {
            glm::vec3 a = this->vertices[face->a];
            glm::vec3 b = this->vertices[face->b];
            glm::vec3 c = this->vertices[face->c];
            face->normal = glm::normalize( glm::cross(c - b, a - b) );
            
            face->vertexNormals.push_back( face->normal );
            face->vertexNormals.push_back( face->normal );
            face->vertexNormals.push_back( face->normal );
        }
    }
    
    
    void Geometry::computeTangents() {
        
        vector<glm::vec3> tan_1( vertices.size() );
        vector<glm::vec3> tan_2( vertices.size() );
        
//	computeTangents: function () {
//        
//		// based on http://www.terathon.com/code/tangent.html
//		// tangents go to vertices
//        
//		var f, fl, v, vl, i, il, vertexIndex,
//        face, uv, vA, vB, vC, uvA, uvB, uvC,
//        x1, x2, y1, y2, z1, z2,
//        s1, s2, t1, t2, r, t, test,
//        tan1 = [], tan2 = [],
//        sdir = new THREE.Vector3(), tdir = new THREE.Vector3(),
//        tmp = new THREE.Vector3(), tmp2 = new THREE.Vector3(),
//        n = new THREE.Vector3(), w;
//        
//		for ( v = 0, vl = this.vertices.length; v < vl; v ++ ) {
//            
//			tan1[ v ] = new THREE.Vector3();
//			tan2[ v ] = new THREE.Vector3();
//            
//		}
//        
//		function handleTriangle( context, a, b, c, ua, ub, uc ) {
//            
//			vA = context.vertices[ a ];
//			vB = context.vertices[ b ];
//			vC = context.vertices[ c ];
//            
//			uvA = uv[ ua ];
//			uvB = uv[ ub ];
//			uvC = uv[ uc ];
//            
//			x1 = vB.x - vA.x;
//			x2 = vC.x - vA.x;
//			y1 = vB.y - vA.y;
//			y2 = vC.y - vA.y;
//			z1 = vB.z - vA.z;
//			z2 = vC.z - vA.z;
//            
//			s1 = uvB.x - uvA.x;
//			s2 = uvC.x - uvA.x;
//			t1 = uvB.y - uvA.y;
//			t2 = uvC.y - uvA.y;
//            
//			r = 1.0 / ( s1 * t2 - s2 * t1 );
//			sdir.set( ( t2 * x1 - t1 * x2 ) * r,
//                     ( t2 * y1 - t1 * y2 ) * r,
//                     ( t2 * z1 - t1 * z2 ) * r );
//			tdir.set( ( s1 * x2 - s2 * x1 ) * r,
//                     ( s1 * y2 - s2 * y1 ) * r,
//                     ( s1 * z2 - s2 * z1 ) * r );
//            
//			tan1[ a ].add( sdir );
//			tan1[ b ].add( sdir );
//			tan1[ c ].add( sdir );
//            
//			tan2[ a ].add( tdir );
//			tan2[ b ].add( tdir );
//			tan2[ c ].add( tdir );
//            
//		}
//        
//		for ( f = 0, fl = this.faces.length; f < fl; f ++ ) {
//            
//			face = this.faces[ f ];
//			uv = this.faceVertexUvs[ 0 ][ f ]; // use UV layer 0 for tangents
//            
//			handleTriangle( this, face.a, face.b, face.c, 0, 1, 2 );
//            
//		}
//        
//		var faceIndex = [ 'a', 'b', 'c', 'd' ];
//        
//		for ( f = 0, fl = this.faces.length; f < fl; f ++ ) {
//            
//			face = this.faces[ f ];
//            
//			for ( i = 0; i < Math.min( face.vertexNormals.length, 3 ); i++ ) {
//                
//				n.copy( face.vertexNormals[ i ] );
//                
//				vertexIndex = face[ faceIndex[ i ] ];
//                
//				t = tan1[ vertexIndex ];
//                
//				// Gram-Schmidt orthogonalize
//                
//				tmp.copy( t );
//				tmp.sub( n.multiplyScalar( n.dot( t ) ) ).normalize();
//                
//				// Calculate handedness
//                
//				tmp2.crossVectors( face.vertexNormals[ i ], t );
//				test = tmp2.dot( tan2[ vertexIndex ] );
//				w = (test < 0.0) ? -1.0 : 1.0;
//                
//				face.vertexTangents[ i ] = new THREE.Vector4( tmp.x, tmp.y, tmp.z, w );
//                
//			}
//            
//		}
//        
//		this.hasTangents = true;
//        
//	},
    }
    
    
    void Geometry::computeVertexNormals( bool area_weighted ) {
        vector<glm::vec3> vertices( this->vertices.size() );
        
        if( area_weighted ) {
            for( PTR(Face3) face : faces ) {
                glm::vec3 a = this->vertices[face->a];
                glm::vec3 b = this->vertices[face->b];
                glm::vec3 c = this->vertices[face->c];
                
                glm::vec3 cb = c - b;
                glm::vec3 ab = a - b;
                cb = glm::cross(cb, ab);
                
                vertices[face->a] += cb;
                vertices[face->b] += cb;
                vertices[face->c] += cb;
            }
        }
        else {
            for( PTR(Face3) face : faces ) {
                vertices[face->a] += face->normal;
                vertices[face->b] += face->normal;
                vertices[face->c] += face->normal;
            }
        }
        
        for( glm::vec3& vertex : vertices )
            vertex = glm::normalize( vertex );
        
        for( PTR(Face3) face: faces ) {
            face->vertexNormals.resize(3);
            
            face->vertexNormals[0] = vertices[face->a];
            face->vertexNormals[1] = vertices[face->b];
            face->vertexNormals[2] = vertices[face->c];
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
            PTR(Face3) face = faces[i];
            
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
            PTR(Face3) face_1 = faces[i];
            
            vector<glm::vec3> vertices_1 = {
                vertices[face_1->a],
                vertices[face_1->b],
                vertices[face_1->c]
            };
            
            for( int j = i + 1; j < faces.size(); j++ ){
                PTR(Face3) face_2 = faces[j];
                
                vector<glm::vec3> vertices_2 = {
                    vertices[face_2->a],
                    vertices[face_2->b],
                    vertices[face_2->c]
                };
                
                /* If they same the same vertices, then we average the normal vectors (by summing and normalizing) */
                for( int a = 0; a < 3; a++ ) {
                    for( int b = 0; b < 3; b++ ) {
                        if( Math::equals(vertices_1[a], vertices_2[b], 3)) {
                            face_1->vertexNormals[a] += face_2->vertexNormals[b];
                            face_2->vertexNormals[b] += face_1->vertexNormals[a];
                        }
                    }
                }
            }
        }
        
        /* Normalizing the vertex normals */
        for( auto face: faces ) {
            face->vertexNormals[0] = glm::normalize(face->vertexNormals[0] );
            face->vertexNormals[1] = glm::normalize(face->vertexNormals[1] );
            face->vertexNormals[2] = glm::normalize(face->vertexNormals[2] );
        }
    }
    
    void Geometry::initGLBuffers() {
        noOfElements = static_cast<int>(faces.size() * 3);
        
        std::vector<unsigned short> index;
        std::vector<glm::vec3> normals( vertices.size() );
        std::vector<glm::vec2> uvs( vertices.size() );
        
        for( PTR(Face3) face: faces ) {
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
        
        //FIXME: Indexed VBO messes the uv for texture

        glGenBuffers( 4, bufferIDs );
        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[0] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * vertices.size(), &vertices[0], GL_STATIC_DRAW );
        
        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[1] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * normals.size(), &normals[0], GL_STATIC_DRAW );
        
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bufferIDs[2] );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned short ) * faces.size() * 3, &index[0], GL_STATIC_DRAW );
        
        glBindBuffer( GL_ARRAY_BUFFER, bufferIDs[3] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec2 ) * uvs.size(), &uvs[0], GL_STATIC_DRAW );
        
        glBuffersInitialized = true;
        
        vertices.clear();
    }
    
}