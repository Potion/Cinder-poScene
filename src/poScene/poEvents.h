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
        void continuePropagation();
    private:
        bool shouldPropagate;
    };

    class MouseEvent
    : public ci::app::MouseEvent
    , public po::Event
    {
    public:
        enum Type {
            DOWN,
            MOVE,
            DRAG,
            UP,
            WHEEL
        };
        
        MouseEvent(Type type, ci::Vec2f windowPos);
        
        void getX();
        void getY();
        void getPos();
        
        void getSceneX();
        void getSceneY();
        void getScenePos();
        
        void getWindowX();
        void getWindowY();
        void getWindowPos();
        
        Type getType();
        
    private:
        Type type;
        ci::Vec2f pos, scenePos, windowPos;
    };
}

