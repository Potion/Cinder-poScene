//
//  poEventCenter.h
//  BasicTest
//
//  Created by Stephen Varga on 3/17/14.
//
//

#pragma once

#include "cinder/app/MouseEvent.h"

#include "poEvents.h"
#include "poNode.h"

namespace po {
    
    class EventCenter;
    typedef std::shared_ptr<EventCenter> EventCenterRef;
    
    class EventCenter
    {
    public:
        static EventCenterRef create();
        
        void processEvents(std::vector<NodeRef> &nodes);
        
        void setInteractionOffset(ci::Vec2f offset);
        
    private:
        EventCenter();
        
        ci::Vec2f mOffset;
        
        //------------------------------------
        //Mouse Events
        #pragma mark - Mouse Events -
        
        void processMouseEvents(std::vector<NodeRef> &nodes);
        void notifyAllNodes(std::vector<NodeRef> &nodes, po::MouseEvent event, const po::MouseEvent::Type &type);
        void notifyCallbacks(std::vector<NodeRef> &nodes, po::MouseEvent event, const po::MouseEvent::Type &type);
        
        //Mouse Event Cinder Callbacks
        virtual void	mouseDown(ci::app::MouseEvent event)    { mMouseEventQueues[MouseEvent::Type::DOWN].push_back(event);    };
        virtual void	mouseMove(ci::app::MouseEvent event)    { mMouseEventQueues[MouseEvent::Type::MOVE].push_back(event);    };
        virtual void	mouseDrag(ci::app::MouseEvent event)    { mMouseEventQueues[MouseEvent::Type::DRAG].push_back(event);    };
        virtual void	mouseUp(ci::app::MouseEvent event)      { mMouseEventQueues[MouseEvent::Type::UP].push_back(event);      };
        virtual void	mouseWheel(ci::app::MouseEvent event)   { mMouseEventQueues[MouseEvent::Type::WHEEL].push_back(event);   };
        
        //Mouse Event Queue
        std::map<po::MouseEvent::Type, std::vector<ci::app::MouseEvent> > mMouseEventQueues;
        
        
        
        //------------------------------------
        //Mouse Events
        #pragma mark - Touch Events -
        
        void processTouchEvents(std::vector<NodeRef> &nodes);
        void notifyAllNodes(std::vector<NodeRef> &nodes,     po::TouchEvent event, const po::TouchEvent::Type &type);
        void notifyCallbacks(std::vector<NodeRef> &nodes,   po::TouchEvent event, po::TouchEvent::Type &type);
        
        //Touch Event Cinder Callbacks
        void touchesBegan(ci::app::TouchEvent event)    { mTouchEventQueues[po::TouchEvent::Type::BEGAN].push_back(event); };
        void touchesMoved(ci::app::TouchEvent event)    { mTouchEventQueues[po::TouchEvent::Type::MOVED].push_back(event); };;
        void touchesEnded(ci::app::TouchEvent event)    { mTouchEventQueues[po::TouchEvent::Type::ENDED].push_back(event); };;
        
        //Touch Event Queue
        std::map<po::TouchEvent::Type, std::vector<ci::app::TouchEvent> > mTouchEventQueues;
        
        
        
        //------------------------------------
        //Key Events
        #pragma mark - Key Events -
        
        void processKeyEvents(std::vector<NodeRef> &nodes);
        void notifyAllNodes(std::vector<NodeRef> &nodes, po::KeyEvent event, const po::KeyEvent::Type &type);
        
        //Key Event Cinder Callbacks
        virtual void    keyDown(ci::app::KeyEvent event)        { mKeyEventQueues[KeyEvent::Type::DOWN].push_back(event); }
        virtual void    keyUp(ci::app::KeyEvent event)          { mKeyEventQueues[KeyEvent::Type::UP].push_back(event); }
            
        //Key Event Queue
        std::map<po::KeyEvent::Type, std::vector<ci::app::KeyEvent> > mKeyEventQueues;
    };
}