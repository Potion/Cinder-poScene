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
        
        MouseEvent(ci::app::MouseEvent event, ci::Vec2f offset);
        
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
        class Touch;
        
        TouchEvent(ci::app::TouchEvent event, ci::Vec2f offset);
        std::vector<po::TouchEvent::Touch>& getTouches() { return mTouches; }
    
    private:
        std::vector<Touch> mTouches;
        
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
            Touch(ci::app::TouchEvent::Touch event, ci::Vec2f offset);
            
            float getX() { return getPos().x; };
            float getY() { return getPos().y; };
            ci::Vec2f getPos();

			float getPrevX() { return mPrevPos.x; };
			float getPrevY() { return mPrevPos.y; };
			ci::Vec2f getPrevPos() { return mPrevPos; };
            
            float getSceneX() { return getScenePos().x; };
            float getSceneY() { return getScenePos().y; };
            ci::Vec2f getScenePos();
            
            float getWindowX() { return mWindowPos.x; };
            float getWindowY() { return mWindowPos.y; };
            ci::Vec2f getWindowPos() { return mWindowPos; };
            
            uint32_t	getId() const { return mId; }
            double		getTime() const { return mTime; }
			NodeRef getSource() { return mSource.lock(); };
            
            
        private:
            ci::Vec2f       mWindowPos;
			ci::Vec2f		mPrevPos;
            uint32_t        mId;
            double          mTime;

			//Native events currently can't be passed through the po::Scene event system :/
            const void*	getNative() const { return mNative; }
            void			*mNative;
            
            std::weak_ptr<Node> mSource;
        };
    };
}

