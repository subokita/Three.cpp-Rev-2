//
//  EventDispatcher.h
//  Three.cpp
//
//  Created by Saburo Okita on 30/06/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp__EventDispatcher__
#define __Three_cpp__EventDispatcher__

#include <iostream>
#include <map>
#include <vector>
#include "internal_headers.h"

namespace three {
    class Event;
    
    class EventDispatcher {
    public:
        void addEventListener( std::string type, ptr<EventDispatcher> obj );
        bool hasEventListener( std::string type, ptr<EventDispatcher> obj );
        void removeEventListener( std::string type, ptr<EventDispatcher> obj );
        void dispatchEvent( Event event );
        void setEventHandler( std::function<void(EventDispatcher&, Event&)> handler );
        
    protected:
        std::function<void(EventDispatcher&, Event&)> eventHandler = [](EventDispatcher&, Event&){};
        std::map<std::string, std::vector<ptr<EventDispatcher> >> listeners;
    };
}

#endif /* defined(__Three_cpp__EventDispatcher__) */
