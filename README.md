Three.cpp (Rev 2)
=========

A port of Three.js into C++. 

Rev 2, because originally I tried to import by porting JavaScript code directly starting from the more modular ones (e.g. Math packages) to the more encompassing classes (e.g. Renderer), however it backfired, because of JavaScript's dynamic typing, everything becomes more confusing, data members can be added way later and hidden in the codes. In Rev 2 approach, I try to create a working example as soon as possible.


Overview
========
This is an effort to port Three.js to OpenGL. The main idea is not to create the fastest 3D engine out there, but to create a 3D engine that's simple enough to instantiate, so that other field of computing could use it rapidly for prototyping. Thus reducing the unnecessary hoops for OpenGL newcomers when it comes to thing such as context creation, loading shaders, etc.

I'm not a 3D programmer, I wanted to have a 3D library that I can use to show the results of SLAM / PTAM or Bundle Adjustment, while most sample codes out there use pre OpenGL 3.0 for visualization, and most of them implement their own 3D viewers.

[Installation](https://github.com/subokita/Three.cpp-Rev-2/wiki/Installation)
============
Just be warned that I haven't tried out the part where it automatically installs Homebrew and all the necessary libraries. And for now it's only for OS X, because I don't have a Windows PC

After cloning the repository, go to build/ and run install.sh, it should install Homebrew and the necessary libraries (e.g. glfw3, glm, etc) when it's needed. After that it will proceed to run cmake and make to build the three.cpp library, and run a test program

Current Progress
================
Read more about it here: https://github.com/subokita/Three.cpp-Rev-2/wiki/Current-Progress

The code is doing okay, might not be game developers' level, but it works, it shows what it needs to show, and it's quite straightforward.


Working Examples
================
* Simple primitives (https://www.youtube.com/watch?v=itGcH7vIeBY):

![Simple primitives](http://subokita.files.wordpress.com/2014/07/screen-shot-2014-07-17-at-14-57-53.png?w=400)

* Texture and Normal mappings (https://www.youtube.com/watch?v=SIQfvJHAWus):

![Texture and normal Mappings](http://subokita.files.wordpress.com/2014/07/texture-and-normal-mappings.gif?w=400)

* Environment / cube map (https://www.youtube.com/watch?v=ZHX935KbJRY):

![Environment map](http://subokita.files.wordpress.com/2014/07/2014-07-22-20_37_43.gif?w=400)

* Shadow mapping (https://www.youtube.com/watch?v=N6oa2CyxTFQ):

![Shadow mapping](http://subokita.files.wordpress.com/2014/08/2014-08-03-11_26_29.gif?w=400)

* Loading PLY models (https://www.youtube.com/watch?v=xL2jixliwb4):

![Loading PLY Models](https://subokita.files.wordpress.com/2014/08/2014-08-04-18_53_49.gif?w=400)

* Ray casting / picking (https://www.youtube.com/watch?v=Ku-UwyC8RAc)

![Ray casting](http://subokita.files.wordpress.com/2014/08/2014-08-11-11_23_27.gif?w=400)

* Integration with FontStash (https://www.youtube.com/watch?v=82OoIITVs80)

![Integration with FontStash](https://subokita.files.wordpress.com/2014/08/2014-08-18-14_19_12.gif?w=400)

FAQ
===
Read more about it here: https://github.com/subokita/Three.cpp-Rev-2/wiki/FAQs
