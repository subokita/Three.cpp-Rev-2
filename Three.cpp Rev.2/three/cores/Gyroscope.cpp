//
//  Gyroscope.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 26/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Gyroscope.h"

namespace three {
    ptr<Gyroscope> Gyroscope::create(){
        return make_shared<Gyroscope>();
    }
    
    Gyroscope::Gyroscope(){}
    
    Gyroscope::~Gyroscope(){}
    
    void Gyroscope::updateMatrixWorld(bool force){
        if( matrixAutoUpdate )
            updateMatrix();
        
        if( matrixWorldNeedsUpdate || force ) {
            if( parent != nullptr ) {
                matrixWorld = parent->matrixWorld * matrix;
                Math::decomposeMatrix(matrixWorld, translationWorld, quaternionWorld, scaleWorld );
                Math::decomposeMatrix(matrix, translationObject, quaternionObject, scaleObject );
                
                matrixWorld = Math::composeMatrix( translationWorld, quaternionObject, scaleWorld );
            }
            else {
                matrixWorld = matrix;
            }
            
            matrixWorldNeedsUpdate = false;
            force = true;
        }
        
        for( auto entry: children )
            entry.second->updateMatrixWorld(force);
    }
   
}