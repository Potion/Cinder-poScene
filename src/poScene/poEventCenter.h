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

namespace po { namespace scene {
    
    class EventCenter;
    typedef std::shared_ptr<EventCenter> EventCenterRef;
    
    class EventCenter
    {
    public:
        static EventCenterRef create();
        
        void processEvents(std::vector<NodeRef> nodes);
        
    private:
        EventCenter();
        
        //  Event Processor Template Class
        //  99% of code is the same for Mouse + Touch
        
        template<typename CiEventT, typename EventT, typename EventTypeT>
        class EventProcessor {
        public:
            //EventProcessor();
            void addToQueue(EventTypeT type, CiEventT ciEvent) { mQueue[type].push_back(ciEvent); }
            
            void processEvents(std::vector<NodeRef> &nodes) {
                //Go through the queue
                for(auto& eventQueue : mQueue) {
                    //Get the type for this item in the std::map
                    EventTypeT type = (EventTypeT)eventQueue.first;
                    
                    //Go through all the ci::MouseEvents for this type
                    for(CiEventT &ciEvent : eventQueue.second) {
                        EventT poEvent(ciEvent);
                        notifyAllNodes(nodes, poEvent, type);
                        notifyCallbacks(nodes, poEvent, type);
                    }
                    
                    //Clear out the events
                    eventQueue.second.clear();
                }
            }
            
        protected:
            void notifyAllNodes(std::vector<NodeRef> &nodes, EventT event, const EventTypeT &type) {
                for(NodeRef &node : nodes) {
                    //Check if it is valid (the item hasn't been deleted) and if it is enabled for events
                    if(node == nullptr || (!node->isEligibleForInteractionEvents())) {
                        continue;
                    }
                    
                    event.setShouldPropagate(true);
                    
                    //Notify the node
                    node->emitEvent(event, type);
                }
            }
            
            //  Extend this function to define custom callback type
            virtual void notifyCallbacks(std::vector<NodeRef> &nodes, EventT event, EventTypeT &type)
            {
                //Go through the draw tree, notifying nodes that are listening
                for(NodeRef &node : nodes) {
                    if(node->isEligibleForInteractionEvent(type) && node->pointInside(event.getWindowPos()))
                    {
                        node->emitEvent(event, type);
                        if(event.getShouldPropagate()) {
                            event.setShouldPropagate(false);
                        } else {
                            return;
                        }
                    }
                }
            }
            
            std::map<EventTypeT, std::vector<CiEventT> > mQueue;
        };
        
        
        
        
        // ------------------------------------
        // Mouse Events
        #pragma mark - Mouse Events -
        
        class MouseEventProcessor
        : public EventProcessor<ci::app::MouseEvent, MouseEvent, MouseEvent::Type>
        {
            void notifyCallbacks(std::vector<NodeRef> &nodes, MouseEvent event, MouseEvent::Type &type)
            {
                MouseEvent::Type callbackType;
                switch (type) {
                    case MouseEvent::Type::DOWN:
                        callbackType = MouseEvent::Type::DOWN_INSIDE; break;
                    case MouseEvent::Type::MOVE:
                        callbackType = MouseEvent::Type::MOVE_INSIDE; break;
                    case MouseEvent::Type::DRAG:
                        callbackType = MouseEvent::Type::DRAG_INSIDE; break;
                    case MouseEvent::Type::UP:
                        callbackType = MouseEvent::Type::UP_INSIDE; break;
                }
                
                EventProcessor::notifyCallbacks(nodes, event, callbackType);
            }
        };
        
        MouseEventProcessor mMouseProcessor;
        
        // Mouse Event Cinder Callbacks
        virtual void	mouseDown(ci::app::MouseEvent event)    { mMouseProcessor.addToQueue(MouseEvent::Type::DOWN,    event); };
        virtual void	mouseMove(ci::app::MouseEvent event)    { mMouseProcessor.addToQueue(MouseEvent::Type::MOVE,    event); };
        virtual void	mouseDrag(ci::app::MouseEvent event)    { mMouseProcessor.addToQueue(MouseEvent::Type::DRAG,    event); };
        virtual void	mouseUp(ci::app::MouseEvent event)      { mMouseProcessor.addToQueue(MouseEvent::Type::UP,      event); };
        virtual void	mouseWheel(ci::app::MouseEvent event)   { mMouseProcessor.addToQueue(MouseEvent::Type::WHEEL,   event); };
        
        
        
        // ------------------------------------
        // Touch Events
        #pragma mark - Touch Events -
        
        class TouchEventProcessor
        : public EventProcessor<ci::app::TouchEvent::Touch, TouchEvent, TouchEvent::Type>
        {
        public:
            //  Needs to break about grouped touches from Cinder
            void addToQueue(TouchEvent::Type type, ci::app::TouchEvent event) {
                for(auto &ciTouch : event.getTouches()) {
                    mQueue[type].push_back(ciTouch);
                }
            }
            
        private:
            void notifyCallbacks(std::vector<NodeRef> &nodes, TouchEvent event, TouchEvent::Type &type)
            {
                //Set the callback type
                TouchEvent::Type callbackType;
                switch (type) {
                    case TouchEvent::Type::BEGAN:
                        callbackType = TouchEvent::Type::BEGAN_INSIDE; break;
                    case TouchEvent::Type::MOVED:
                        callbackType = TouchEvent::Type::MOVED_INSIDE; break;
                    case TouchEvent::Type::ENDED:
                        callbackType = TouchEvent::Type::ENDED_INSIDE; break;
                }
                
                EventProcessor::notifyCallbacks(nodes, event, callbackType);
            }
        };
        
        TouchEventProcessor mTouchProcessor;
        
        // Touch Event Cinder Callbacks
        void touchesBegan(ci::app::TouchEvent event)    { mTouchProcessor.addToQueue(TouchEvent::Type::BEGAN, event); };
        void touchesMoved(ci::app::TouchEvent event)    { mTouchProcessor.addToQueue(TouchEvent::Type::MOVED, event); };
        void touchesEnded(ci::app::TouchEvent event)    { mTouchProcessor.addToQueue(TouchEvent::Type::ENDED, event); };
    };
    
    
} } //  Namespace: po::scene