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
    , mWindowPos(event.getPos())
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
    TouchEvent::TouchEvent(ci::app::TouchEvent event)
    {
        for(const ci::app::TouchEvent::Touch touch : event.getTouches()) {
            mTouches.push_back(TouchEvent::Touch(touch, offset));
        }
    }
    
    TouchEvent::Touch::Touch(ci::app::TouchEvent::Touch touch, ci::Vec2f offset)
    : mPrevPos(touch.getPrevPos())
    , mWindowPos(touch.getPos())
    , mId(touch.getId())
    , mTime(touch.getTime())
    , mNative((void *)touch.getNative())
    {
        mWindowPos += offset;
    }
    
    ci::Vec2f TouchEvent::Touch::getPos()
    {
        NodeRef source = mSource.lock();
        if(source)
            return source->globalToLocal(getWindowPos());
        else
            return getWindowPos();
    };
    
    ci::Vec2f TouchEvent::Touch::getScenePos()
    {
        NodeRef source = mSource.lock();
        if(source && source->hasScene())
            return source->getScene()->getRootNode()->globalToLocal(getWindowPos());
        else
            return getWindowPos();
    }
} } //  Namespace: po::scene