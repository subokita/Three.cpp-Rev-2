//
//  LineBasicMaterial.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 12/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__LineBasicMaterial__
#define __Three_cpp_Rev_2__LineBasicMaterial__

#include <iostream>
#include "BasicMaterial.h"


namespace three {
    class LineBasicMaterial : public BasicMaterial {
    public:
        static ptr<LineBasicMaterial> create( Color color = 0xFFFFFF );
        ~LineBasicMaterial();
        
    protected:
        LineBasicMaterial();
        LineBasicMaterial(Color color);
    };
}


#endif /* defined(__Three_cpp_Rev_2__LineBasicMaterial__) */
