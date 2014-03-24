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
    
    class Event {
    public:
        Event() : shouldPropagate(false) {};
        void continuePropagation() { shouldPropagate = true; };
    private:
        bool shouldPropagate;
    };

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
        
        MouseEvent(Type type, ci::Vec2f windowPos);
        
        float getX();
        float getY();
        ci::Vec2f getPos();
        
        float getSceneX();
        float getSceneY();
        ci::Vec2f getScenePos();
        
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
}

