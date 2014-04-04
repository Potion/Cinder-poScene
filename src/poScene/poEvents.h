//
//  poEvents.h
//  BasicScene
//
//  Created by Stephen Varga on 3/22/14.
//
//

#pragma once

#include "poEvents.h"

namespace po {
    class Node;
    typedef std::shared_ptr<Node> NodeRef;
    
    //BASE EVENT
    class Event {
    public:
        Event() : shouldPropagate(false) {};
        void setShouldPropagate(bool shouldPropagate) { this->shouldPropagate = shouldPropagate; };
        bool getShouldPropagate() { return shouldPropagate; };
    private:
        bool shouldPropagate;
    };

    
    //MOUSE EVENTS
    class MouseEvent
    : public ci::app::MouseEvent
    , public po::Event
    {
        friend class EventCenter;
        friend class Node;
    public:
        enum Type {
            DOWN,
            DOWN_INSIDE,
            MOVE,
            MOVE_INSIDE,
            DRAG,
            DRAG_INSIDE,
            UP,
            UP_INSIDE,
            WHEEL
        };
        
        MouseEvent(ci::app::MouseEvent event);
        
        float getX() { return mPos.x; };
        float getY() { return mPos.y; };
        ci::Vec2f getPos() { return mPos; };
        
        float getSceneX() { return mScenePos.x; };
        float getSceneY() { return mScenePos.y; };
        ci::Vec2f getScenePos() { return mScenePos; };
        
        float getWindowX() { return mWindowPos.x; };
        float getWindowY() { return mWindowPos.y; };
        ci::Vec2f getWindowPos() { return mWindowPos; };
        
        NodeRef getSource() { return mSource.lock(); };
        
    private:
        std::weak_ptr<Node> mSource;
        ci::Vec2f mPos, mScenePos, mWindowPos;
    };
    
    //KEY EVENTS
    
    
    
    
    //KEY EVENTS
    class KeyEvent
    : public ci::app::KeyEvent
    , public po::Event
    {
        friend class EventCenter;
    public:
        enum Type {
            DOWN,
            UP
        };
        
        KeyEvent(ci::app::KeyEvent event);
        
    private:
    };
}

