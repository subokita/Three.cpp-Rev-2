//
//  HasID.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 16/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__HasID__
#define __Three_cpp_Rev_2__HasID__

#include <iostream>
namespace three {
    /***
     * Most objects should inherit this to have (presumably) unique-id
     */
    class HasID {
    public:
        HasID( const unsigned int id );
        virtual ~HasID();
        
    public:
        unsigned int id;
        std::string uuid;
        std::string name;
    };
}

#endif /* defined(__Three_cpp_Rev_2__HasID__) */
