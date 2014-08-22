//
//  Gyroscope.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 26/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Gyroscope.h"
#include "MathUtils.h"

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
                MathUtils::decomposeMatrix(matrixWorld, translationWorld, quaternionWorld, scaleWorld );
                MathUtils::decomposeMatrix(matrix, translationObject, quaternionObject, scaleObject );
                
                matrixWorld = MathUtils::composeMatrix( translationWorld, quaternionObject, scaleWorld );
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