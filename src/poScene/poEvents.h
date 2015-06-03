//
//  poEvents.h
//  BasicScene
//
//  Created by Stephen Varga on 3/22/14.
//
//

#pragma once

#include "cinder/app/App.h"

#include "poEvents.h"

namespace po { namespace scene {
    class Node;
    typedef std::shared_ptr<Node> NodeRef;
    
    //----------------------------------
    //BASE EVENT
    #pragma mark Base Event
    
    class Event {
        friend class Node;
    public:
        Event();
        
        void setShouldPropagate(bool shouldPropagate) { mShouldPropagate = shouldPropagate; };
        bool getShouldPropagate() { return mShouldPropagate; };
        
        ci::Vec2f getWindowPos() { return mWindowPos; }
        ci::Vec2f getLocalPos();
        ci::Vec2f getScenePos();
        NodeRef getSource() { return mSource.lock(); };
        
    protected:
        void setSource(NodeRef source) { mSource = source; };
        ci::Vec2f mWindowPos;
        
    private:
        bool mShouldPropagate;
        std::weak_ptr<Node> mSource;
    };

    
    
    //----------------------------------
    //MOUSE EVENTS
    #pragma mark Mouse Event
    
    class MouseEvent
    : public po::scene::Event
    {
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
        
        ci::app::MouseEvent getCiEvent() { return mCiEvent; };
        
    private:
        ci::app::MouseEvent mCiEvent;
    };
    
    
    
    //----------------------------------
    //TOUCH EVENT
    #pragma mark Touch Event
    
    class TouchEvent
    : public Event
    {
    public:
        enum Type {
            BEGAN,
            BEGAN_INSIDE,
            MOVED,
            MOVED_INSIDE,
            ENDED,
            ENDED_INSIDE
        };
        
        TouchEvent(ci::app::TouchEvent::Touch event);
        
        ci::app::TouchEvent::Touch getCiEvent() { return mCiEvent; };
        
    private:
        ci::app::TouchEvent::Touch mCiEvent;
    };
} } //  Namespace: po::scene