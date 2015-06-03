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
    public:
        Event();
        
        void setShouldPropagate(bool shouldPropagate) { mShouldPropagate = shouldPropagate; };
        bool getShouldPropagate() { return mShouldPropagate; };
        
        ci::Vec2f getWindowPos() { return mWindowPos; }
        ci::Vec2f getLocalPos();
        ci::Vec2f getScenePos();
        
        void setSource(NodeRef source);
        NodeRef getSource() { return mSource.lock(); };
        
    protected:
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
    : public ci::app::TouchEvent
    , public Event
    {
    public:
        class Touch;
        
        TouchEvent(ci::app::TouchEvent event, ci::Vec2f offset);
        std::vector<TouchEvent::Touch>& getTouches() { return mTouches; }
    
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
            Touch(ci::app::TouchEvent::Touch event);
            
            uint32_t	getId() const { return mId; }
            double		getTime() const { return mTime; }
            
            
        private:
            uint32_t        mId;
            double          mTime;

			//Native events currently can't be passed through the po::Scene event system :/
            const void      *getNative() const { return mNative; }
            void			*mNative;
            
            std::weak_ptr<Node> mSource;
        };
    };
} } //  Namespace: po::scene