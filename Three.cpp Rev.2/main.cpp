//
//  main.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>
#include "three/three.h"
#include "examples/examples_header.h"

using namespace std;
using namespace three;

int main(int argc, const char * argv[])
{
//    Ex_001_SimplePrimitives::create()->run();
//    Ex_002_TextureSpecularNormalMappings::create()->run();
    Ex_003_EnvironmentMapping::create()->run();
    
    return 0;
}

