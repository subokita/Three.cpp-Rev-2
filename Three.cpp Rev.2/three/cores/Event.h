//
//  Event.h
//  Three.cpp
//
//  Created by Saburo Okita on 02/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp__Event__
#define __Three_cpp__Event__

#include <iostream>
#include "Object3D.h"
#include "Scene.h"

namespace  three {
    class Event {
    public:
        std::string type;
        ptr<Object3D> object;
        ptr<Object3D> scene;
    };
}


#endif /* defined(__Three_cpp__Event__) */
