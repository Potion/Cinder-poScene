//
//  poEvent.cpp
//  BasicScene
//
//  Created by Stephen Varga on 3/22/14.
//
//

#include "poEvents.h"
#include "poNode.h"
#include "poScene.h"

namespace po { namespace scene {
    
    #pragma mark - Event -
    Event::Event()
    : mShouldPropagate(false)
    , mWindowPos(0,0)
    {
    }
    
    ci::Vec2f Event::getScenePos()
    {
        NodeRef source = getSource();
        if(source) {
            return source->windowToScene(getWindowPos());
        }
        
        return getWindowPos();
    }
    
    ci::Vec2f Event::getLocalPos()
    {
        NodeRef source = getSource();
        if(source)
        {
            return source->windowToLocal(getWindowPos());
        }
        
        return getWindowPos();
    }
    
    
    #pragma mark - Mouse Event -
    //Brutal copy-constructor replication...def not fun
    MouseEvent::MouseEvent(ci::app::MouseEvent event)
    : mCiEvent(event)
    {
        mWindowPos = event.getPos();
    }
    
    
    #pragma mark - Touch Event -
    TouchEvent::TouchEvent(ci::app::TouchEvent::Touch event)
    : mCiEvent(event)
    {
        mWindowPos = event.getPos();
    }
} } //  Namespace: po::scene