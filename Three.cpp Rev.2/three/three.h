//
//  three.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 10/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef Three_cpp_Rev_2_three_h
#define Three_cpp_Rev_2_three_h

#include "Constants.h"

/* ./three/cameras */
#include "Camera.h"
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"

/* ./three/controls/ */
#include "CameraControl.h"
#include "Arcball.h"

/* ./three/cores/ */
#include "cores/Event.h"
#include "cores/EventDispatcher.h"
#include "cores/Face3.h"
#include "cores/Geometry.h"
#include "cores/Object3D.h"
#include "cores/Gyroscope.h"
#include "cores/Projector.h"
#include "cores/Raycaster.h"
#include "cores/Rect.h"

/* ./three/helpers/ */


/* ./three/lights */
#include "Light.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "HemisphereLight.h"
#include "SpotLight.h"
#include "CastsShadow.h"
#include "VirtualLight.h"
#include "DirectionalLightsArray.h"
#include "HemisphereLightsArray.h"
#include "PointLightsArray.h"
#include "SpotLightsArray.h"


/* ./three/materials */
#include "Material.h"
#include "MeshCubeMapMaterial.h"
#include "BasicMaterial.h"
#include "LineBasicMaterial.h"
#include "LambertMaterial.h"
#include "PhongMaterial.h"

/* ./three/maths */
#include "maths/Box3.h"
#include "maths/Color.h"
#include "maths/Frustum.h"
#include "maths/Line3.h"
#include "maths/MathUtils.h"
#include "maths/Plane.h"
#include "maths/Ray.h"
#include "maths/Sphere.h"
#include "maths/Triangle.h"

/* ./three/objects */
#include "Mesh.h"
#include "Loader.h"

/* ./three/others */
#include "Utils.h"

/* ./three/plugins */
#include "RenderPlugin.h"
#include "ShadowMapPlugin.h"

/* ./three/primitives */
#include "CubeGeometry.h"
#include "CylinderGeometry.h"
#include "PlaneGeometry.h"
#include "PolyhedronGeometry.h"
#include "IcosahedronGeometry.h"
#include "OctahedronGeometry.h"
#include "TetrahedronGeometry.h"
#include "SphereGeometry.h"
#include "TorusGeometry.h"

/* ./three/renderers */
#include "Renderer.h"
#include "RenderTarget.h"

/* ./three/textures */
#include "Texture.h"
#include "EnvMap.h"
#include "NormalMap.h"
#include "SpecularMap.h"
#include "TextureUtils.h"
#include "ShadowTexture.h"

/* ./three/renderers/scenes */
#include "IFog.h"
#include "Fog.h"
#include "FogExp2.h"
#include "Scene.h"

/* ./three/renderers/shaders */
#include "Shader.h"
#include "ShaderChunks.h"
#include "ShaderLib.h"
#include "ShaderLibraries.h"


/* external libs */
#include <assimp/postprocess.h>

#endif
