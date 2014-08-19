//
//  Ray.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 25/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Ray.h"
#include "Plane.h"
#include "Box3.h"
#include "Sphere.h"
#include "Triangle.h"

namespace three {
    
    ptr<Ray> Ray::create( glm::vec3 origin, glm::vec3 direction ){
        return make_shared<Ray>(origin, direction);
    }
    
    Ray::Ray(){}
    Ray::~Ray(){}
    
    Ray::Ray( glm::vec3 origin, glm::vec3 direction ) :
        origin   ( origin ),
        direction( direction )
    {}
    
    
    void Ray::set( glm::vec3 origin, glm::vec3 direction ){
        this->origin    = origin;
        this->direction = direction;
    }
    
    glm::vec3 Ray::at(float t){
        return (direction * t) + origin;
    }
    
    glm::vec3 Ray::closestPointTo( glm::vec3 point ){
        float direction_dist = glm::dot(point - origin, direction);
        if( direction_dist < 0.0 )
            return origin;
        return (direction * direction_dist) + origin;
    }
    
    float Ray::distanceTo( glm::vec3 point ){
        float direction_dist = glm::dot( point - origin, direction );
        
        return glm::length(point - (origin + direction * direction_dist));
//        float direction_dist = glm::dot( point - origin, direction );
//        if( direction_dist < 0.0 )
//            return glm::dot( origin, point );
//        
//        return glm::dot((direction * direction_dist) + origin, point);
    }
    
    float Ray::distanceSquaredTo( glm::vec3 v0, glm::vec3 v1 ){
        glm::vec3 segment_center = (v0 + v1);
        segment_center *= 0.5;
        
        glm::vec3 segment_dir = glm::normalize(v1 - v0);
        float segment_extent = glm::dot(v0, v1) * 0.5;
        
        glm::vec3 diff = origin - segment_center;
        float a01   = -glm::dot(direction, segment_dir );
        float b0    =  glm::dot( diff, direction );
        float b1    = -glm::dot( diff, segment_dir );
        float c     =  glm::dot( diff, diff );
        float det   =  fabsf(1.0 - a01 * a01 );
        
        float squared_dist = 0.0;
        float s0, s1, ext_dist;
        
        if( det >= 0.0 ){ /* Ray and segment are not parallel */
            s0 = a01 * b1 - b0;
            s1 = a01 * b0 - b1;
            ext_dist = segment_extent * det;
            
            if( s0 >= 0 ) {
                if(s1 >= -ext_dist) {
                    if( s1 <= ext_dist ) { /* region 0 minimum at interior points */
                        float inv_det = 1.0 / det;
                        s0 *= inv_det;
                        s1 *= inv_det;
                        
                        squared_dist = s0 * (s0 + a01 * s1 + 2 * b0) + s1 * ( a01 * s0 + s1 + 2 * b1) + c;
                    }
                    else { /* region 1 */
                        s1 = segment_extent;
                        s0 = std::max( 0.0f, -(a01 * s1 + b0) );
                        squared_dist = -s0 * s0 + s1 * (s1 + 2 * b1) + c;
                    }
                }
                else { /* region 5 */
                    s1 = -segment_extent;
                    s0 = std::max( 0.0f, -(a01 * s1 + b0) );
                    squared_dist = -s0 * s0 + s1 * (s1 + 2 * b1) + c;
                }
            }
            else {
                if( s1 <= -ext_dist ) { /* region 4 */
                    s0 = std::max(0.0f, -(-a01 * segment_extent + b0) );
                    s1 = (s0 > 0) ? -segment_extent : std::min( std::max( -segment_extent, -b1 ), segment_extent );
                    squared_dist = -s0 * s0 + s1 * (s1 + 2 * b1) + c;
                }
                else if( s1 <= ext_dist ) { /* region 3 */
                    s0 = 0.0;
                    s1 = std::min( std::max( -segment_extent, -b1 ), segment_extent );
                    squared_dist = s1 * (s1 + 2 * b1) + c;
                }
                else { /* region 2 */
                    s0 = std::max(0.0f, -(a01 * segment_extent + b0));
                    s1 = (s0 > 0) ? segment_extent : std::min( std::max(-segment_extent, -b1), segment_extent );
                    squared_dist = -s0 * s0 + s1 * (s1 + 2 * b1) + c;
                }
            }
            
        }
        else { /* Ray and segment are parallel */
            s1 = (a01 > 0.0 ) ? -segment_extent : segment_extent;
            s0 = std::max( 0.0f, -(a01 * s1 + b0) );
            squared_dist = -s0 * s0 + s1 * (s1 + 2 * b1) + c;
        }
        
        return squared_dist;
    }
    
    float Ray::distanceTo( ptr<Plane> plane ){
        float denominator = glm::dot( plane->normal, direction );
        if( denominator == 0.0 ) {
            if( plane->distanceTo(origin) == 0.0 )
                return 0.0;
            return -1.0;
        }
        float t = -( glm::dot(origin, plane->normal) + plane->constant ) / denominator;
        return t >= 0 ? t : -1.0;
    }
    
    bool Ray::intersects( ptr<Sphere> sphere ){
        return distanceTo( sphere->center ) <= sphere->radius;
    }
    
    bool Ray::intersects( ptr<Plane> plane ){
        float dist = plane->distanceTo( origin );
        if( dist <= 0.0 )
            return true;
        
        float denominator = glm::dot(plane->normal, direction);
        if( denominator * dist < 0.0 )
            return true;
        
        return false;
    }
    
    bool Ray::intersects( ptr<Box3> box ){
        return intersectsBoxAt(box) != NULL_VEC3;
    }
    
    glm::vec3 Ray::intersectsPlaneAt( ptr<Plane> plane ){
        float t = distanceTo( plane );
        if( t >= 0.0 )
            return at( t );
        return NULL_VEC3;
    }
    
    /**
     * From http://www.geometrictools.com/LibMathematics/Intersection/Wm5IntrRay3Triangle3.cpp
     */
    glm::vec3 Ray::intersectsTriangleAt( ptr<Triangle> triangle, bool backface_cull ) {
        glm::vec3 edge_1 = triangle->b - triangle->a;
        glm::vec3 edge_2 = triangle->c - triangle->a;
        glm::vec3 normal = glm::cross( edge_1, edge_2 );
        
        
        // Solve Q + t*D = b1*E1 + b2*E2 (Q = kDiff, D = ray direction,
        // E1 = kEdge1, E2 = kEdge2, N = Cross(E1,E2)) by
        //   |Dot(D,N)|*b1 = sign(Dot(D,N))*Dot(D,Cross(Q,E2))
        //   |Dot(D,N)|*b2 = sign(Dot(D,N))*Dot(D,Cross(E1,Q))
        //   |Dot(D,N)|*t = -sign(Dot(D,N))*Dot(Q,N)
        
        float dir_dot_normal = glm::dot( direction, normal );
        float sign;
        if( dir_dot_normal > 0.0 ) {
            if( backface_cull )
                return NULL_VEC3;
            sign = 1.0;
        }
        else if( dir_dot_normal < 0.0 ) {
            sign = -1.0;
            dir_dot_normal = -dir_dot_normal;
        }
        else {
            return  NULL_VEC3;
        }
        
        glm::vec3 diff = origin - triangle->a;
        float dir_dot_e2 = sign * glm::dot( direction, glm::cross(diff, edge_2) );
        if( dir_dot_e2 < 0.0 )
            return NULL_VEC3;
        
        float dir_dot_e1 = sign * glm::dot( direction, glm::cross(edge_1, diff) );
        if( dir_dot_e1 < 0.0 )
            return NULL_VEC3;
        
        if( dir_dot_e2 + dir_dot_e1 > dir_dot_normal )
            return NULL_VEC3;
     
        float diff_dot_normal = -sign * glm::dot(diff, normal );
        if( diff_dot_normal < 0.0 )
            return NULL_VEC3;
        
        return at( diff_dot_normal / dir_dot_normal );
    }
    
    
    /*
     * Taken from:
     * http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-7-intersecting-simple-shapes/ray-box-intersection/
     */
    glm::vec3 Ray::intersectsBoxAt( ptr<Box3> box ){
        float inv_dir_x = 1.0f / this->direction.x;
        float inv_dir_y = 1.0f / this->direction.y;
        float inv_dir_z = 1.0f / this->direction.z;
        
        float t_min, t_max, ty_min, ty_max, tz_min, tz_max;
        if( inv_dir_x >= 0.0 ) {
            t_min = (box->min.x - origin.x) * inv_dir_x;
            t_max = (box->max.x - origin.x) * inv_dir_x;
        }
        else {
            t_min = (box->max.x - origin.x) * inv_dir_x;
            t_max = (box->min.x - origin.x) * inv_dir_x;
        }
        
        
        if( inv_dir_y >= 0.0 ) {
            ty_min = (box->min.y - origin.y) * inv_dir_y;
            ty_max = (box->max.y - origin.y) * inv_dir_y;
        }
        else {
            ty_min = (box->max.y - origin.y) * inv_dir_y;
            ty_max = (box->min.y - origin.y) * inv_dir_y;
        }
        
        if( (t_min > ty_max) || (ty_min > t_max) )
            return NULL_VEC3;
        
        if( ty_min > t_min || isnan(t_min) )
            t_min = ty_min;
        
        if( ty_max < t_max || isnan(t_max) )
            t_max = ty_max;
        
     
        if( inv_dir_z >= 0.0 ) {
            tz_min = (box->min.z - origin.z) * inv_dir_z;
            tz_max = (box->max.z - origin.z) * inv_dir_z;
        }
        else {
            tz_min = (box->max.z - origin.z) * inv_dir_z;
            tz_max = (box->min.z - origin.z) * inv_dir_z;
        }
        
        
        if( (t_min > tz_max) || (tz_min > t_max) )
            return NULL_VEC3;
        
        if( tz_min > t_min || isnan(t_min) )
            t_min = tz_min;
        
        if( tz_max < t_max || isnan(t_max) )
            t_max = tz_max;
        
        if( t_max < 0.0 )
            return NULL_VEC3;
        
        return at( t_min >= 0 ? t_min : t_max );
    }
    
    
    void Ray::applyMatrix( glm::mat4& mat ) {
        direction   = glm::vec3(mat * glm::vec4(direction + origin, 0.0));
        origin      = glm::vec3( mat * glm::vec4(origin, 1.0) );
        direction   = glm::normalize(direction - origin);
    }
    
    bool Ray::equals(ptr<Ray> other) {
        return (this->origin == other->origin) && (this->direction == other->direction);
    }
}