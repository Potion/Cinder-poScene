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
        
        MouseEvent(ci::app::MouseEvent event, Type type);
        
        float getX() { return pos.x; };
        float getY() { return pos.y; };
        ci::Vec2f getPos() { return pos; };
        
        float getSceneX() { return scenePos.x; };
        float getSceneY() { return scenePos.y; };
        ci::Vec2f getScenePos() { return scenePos; };
        
        float getWindowX() { return windowPos.x; };
        float getWindowY() { return windowPos.y; };
        ci::Vec2f getWindowPos() { return windowPos; };
        
        Type getType() { return type; };
        NodeRef getSource() { return source.lock(); };
        
    private:
        std::weak_ptr<Node> source;
        Type type;
        ci::Vec2f pos, scenePos, windowPos;
    };
    
    
    
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
        
        KeyEvent(ci::app::KeyEvent event, Type type);
        
        Type getType() { return type; }
        
    private:
        Type type;
    };
}

