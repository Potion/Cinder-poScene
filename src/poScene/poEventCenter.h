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
        
        void processEvents(std::vector<NodeRef> nodes);
        
    private:
        EventCenter();
        
        //Mouse Events
        void processMouseEvents(std::vector<NodeRef> &nodes);
        void notifyAllNodes(std::vector<NodeRef> &nodes, po::MouseEvent event);
        void notifyCallbacks(std::vector<NodeRef> &nodes, po::MouseEvent event);
        
        
        //----------------------------
        //CALLBACKS
        //Callbacks are connected to the ci::app signals, and queued up in order to be fired when we proces events
        
        //Mouse Event Callbacks,
        virtual void	mouseDown(ci::app::MouseEvent event)    { mouseEventQueues[MouseEvent::Type::DOWN].push_back(event);    };
        virtual void	mouseMove(ci::app::MouseEvent event)    { mouseEventQueues[MouseEvent::Type::MOVE].push_back(event);    };
        virtual void	mouseDrag(ci::app::MouseEvent event)    { mouseEventQueues[MouseEvent::Type::DRAG].push_back(event);    };
        virtual void	mouseUp(ci::app::MouseEvent event)      { mouseEventQueues[MouseEvent::Type::UP].push_back(event);      };
        virtual void	mouseWheel(ci::app::MouseEvent event)   { mouseEventQueues[MouseEvent::Type::WHEEL].push_back(event);   };
        
        //Queues for mouse events that need to be fired
        std::map<MouseEvent::Type, std::vector<ci::app::MouseEvent> > mouseEventQueues;
    };
}