//
//  EventDispatcher.cpp
//  Three.cpp
//
//  Created by Saburo Okita on 30/06/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "EventDispatcher.h"
#include "Event.h"

using std::string;
using std::find;

namespace three {
    void EventDispatcher::addEventListener(string type, ptr<EventDispatcher> obj ){
        if( find( listeners[type].begin(), listeners[type].end(), obj) == listeners[type].end() )
            listeners[type].push_back( obj );
    }
    
    bool EventDispatcher::hasEventListener( string type, ptr<EventDispatcher> obj ) {
        if( listeners.count(type) == 0 )
            return false;
        
        return find(listeners[type].begin(), listeners[type].end(), obj ) != listeners[type].end();
    }
    
    void EventDispatcher::removeEventListener( string type, ptr<EventDispatcher> obj ) {
        if( hasEventListener(type, obj) )
            remove( listeners[type].begin(), listeners[type].end(), obj );
    }
    
    void EventDispatcher::dispatchEvent(Event event){
        if( listeners.count( event.type ) == 0 )
            return;
        
        for( auto listener: listeners[event.type] )
            listener->eventHandler( *this, event );
    }
    
    void EventDispatcher::setEventHandler( std::function<void(EventDispatcher&, Event&)>  handler ) {
        this->eventHandler = handler;
    }
}