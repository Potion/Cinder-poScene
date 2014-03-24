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
        
        int getX();
        int getY();
        ci::Vec2i getPos();
        
        void getSceneX();
        void getSceneY();
        void getScenePos();
        
        int getWindowX();
        int getWindowY();
        ci::Vec2i getWindowPos() { return windowPos; };
        
        Type getType() { return type; };
        
    private:
        Type type;
        ci::Vec2i pos, scenePos, windowPos;
    };
}

