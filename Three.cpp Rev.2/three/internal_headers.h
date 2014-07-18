//
//  internal_headers.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 10/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef Three_cpp_Rev_2_internal_headers_h
#define Three_cpp_Rev_2_internal_headers_h

#include <iostream>
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "Constants.h"

/* FORWARD DECLARATIONS */
namespace three {
    
    /* ./three/cameras/ */
    class Camera;
    class OrthographicCamera;
    class PerspectiveCamera;
    
    /* ./three/cores/ */
    class Face3;
    class Geometry;
    class Object3D;
    
    /* ./three/lights */
    class Light;
    class AmbientLight;
    class DirectionalLight;
    class PointLight;
    class HemisphereLight;
    class SpotLight;
    
    /* ./three/materials */
    class Material;
    class MeshPhongMaterial;
    
    /* ./three/maths */
    class Box3;
    class Color;
    class Frustum;
    class Line3;
    class Math;
    class Plane;
    class Sphere;

    
    /* ./three/objects */
    class Mesh;
    
    /* ./three/plugins */
    class RenderPlugin;
    
    /* ./three/others */
    class Utils;
    
    /* ./three/primitives */
    class CubeGeometry;
    class CylinderGeometry;
    class PlaneGeometry;
    class SphereGeometry;
    
    /* ./three/renderers */
    class Renderer;
    
    /* ./three/renderers/scenes */
    class IFog;
    class Fog;
    class FogExp2;
    class Scene;
    
    /* ./three/renderers/shaders */
    class Shader;
    class ShaderChunks;
    
    
    /* ./three/renderers/textures */
    class Texture;
    class NormalTexture;
    
    namespace ShaderLib {
        class Shader;
    }
};

#endif
