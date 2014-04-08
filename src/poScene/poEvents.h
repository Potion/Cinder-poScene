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

namespace po {
    class Node;
    typedef std::shared_ptr<Node> NodeRef;
    
    //----------------------------------
    //BASE EVENT
    #pragma mark Base Event
    
    class Event {
        friend class Node;
    public:
        Event() : shouldPropagate(false) {};
        void setShouldPropagate(bool shouldPropagate) { this->shouldPropagate = shouldPropagate; };
        bool getShouldPropagate() { return shouldPropagate; };
        
        NodeRef getSource() { return mSource.lock(); };
    private:
        bool shouldPropagate;
        std::weak_ptr<Node> mSource;
    };

    
    
    //----------------------------------
    //MOUSE EVENTS
    #pragma mark Mouse Event
    
    class MouseEvent
    : public ci::app::MouseEvent
    , public po::Event
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
        
        float getX() { return mPos.x; };
        float getY() { return mPos.y; };
        ci::Vec2f getPos() { return mPos; };
        
        float getSceneX() { return mScenePos.x; };
        float getSceneY() { return mScenePos.y; };
        ci::Vec2f getScenePos() { return mScenePos; };
        
        float getWindowX() { return mWindowPos.x; };
        float getWindowY() { return mWindowPos.y; };
        ci::Vec2f getWindowPos() { return mWindowPos; };
        
    private:
        ci::Vec2f mPos, mScenePos, mWindowPos;
    };
    
    
    
    //----------------------------------
    //TOUCH EVENT
    #pragma mark Touch Event
    
    class TouchEvent
    : public ci::app::TouchEvent
    , public po::Event
    {
        friend class EventCenter;
    public:
        enum Type {
            BEGAN,
            BEGAN_INSIDE,
            MOVED,
            MOVED_INSIDE,
            ENDED,
            ENDED_INSIDE
        };
        
        class Touch {
            friend class Node;
        public:
            Touch(ci::app::TouchEvent::Touch event);
            
            float getX() { return mPos.x; };
            float getY() { return mPos.y; };
            ci::Vec2f getPos() { return mPos; };
            
            float getSceneX() { return mScenePos.x; };
            float getSceneY() { return mScenePos.y; };
            ci::Vec2f getScenePos() { return mScenePos; };
            
            float getWindowX() { return mWindowPos.x; };
            float getWindowY() { return mWindowPos.y; };
            ci::Vec2f getWindowPos() { return mWindowPos; };
            
            uint32_t	getId() const { return mId; }
            double		getTime() const { return mTime; }
            const void*	getNative() const { return mNative; }
            
            NodeRef getSource() { return mSource.lock(); };
            
        private:
            ci::Vec2f       mPos, mScenePos, mWindowPos;
            uint32_t        mId;
            double          mTime;
            const void		*mNative;
            
            std::weak_ptr<Node> mSource;
        };
        
        TouchEvent(ci::app::TouchEvent event);
        
        std::vector<po::TouchEvent::Touch>& getTouches() { return mTouches; }
        
    private:
        std::vector<Touch> mTouches;
    };
    
    
    
    //KEY EVENTS
    #pragma mark KEY Event
    
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

