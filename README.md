Three.cpp (Rev 2)
=========

A port of Three.js into C++. 

Rev 2, because originally I tried to import by porting JavaScript code directly starting from the more modular ones (e.g. Math packages) to the more encompassing classes (e.g. Renderer), however it backfired, because of JavaScript's dynamic typing, everything becomes more confusing, data members can be added way later and hidden in the codes. In Rev 2 approach, I try to create a working example as soon as possible.


Overview
========
This is an effort to port Three.js to OpenGL. The main idea is not to create the fastest 3D engine out there, but to create a 3D engine that's simple enough to instantiate, so that other field of computing could use it rapidly for prototyping. Thus reducing the unnecessary hoops for OpenGL newcomers when it comes to thing such as context creation, loading shaders, etc.

I'm not a 3D programmer, I wanted to have a 3D library that I can use to show the results of SLAM / PTAM or Bundle Adjustment, while most sample codes out there use pre OpenGL 3.0 for visualization, and most of them implement their own 3D viewers.

Current Progress
================
* The library is able to render primitives such as textured cube or sphere on a scene, utilizing blinn-phong shading. 
* Normal mapping and texture mapping are implemented
* Shader is now one per configuration, e.g. if there are 2 meshes in the scene, one uses phong lighting and normal map, while the others only have texture mapping, then 2 shaders are created for the purposes.
* Shadow mapping is implemented, you can now cast shadow using directional and spot lights (cascade shadow is still pending though)

Many codes are still unoptimized, and some of the codes are patchy in the nature, aka not the optimal solution, but at least able to show working example. 



FAQ
===
* Why those 3rd party libraries?
I played with those before, so it's better for me to use something that I already know. I want to have something that's simpler and direct, not an end product that new users would need to spent hours wrangling with where to install the libraries, what should be included, how to compile with libc++ instead of libstdc++, etc.

* Can I contribute?
Please do so, I'm not super programmer, any help is welcomed.

* Why are these codes so unoptimized?
I'm fairly average programmer, experienced, but not super programmer. Plus, it's in the early phase, I'd like to go for optimization later on.

* Why is it OS X centric (e.g. XCode, Homebrew, etc)?
I only own a Macbook Air.

