/*
 Copyright (c) 2015, Potion Design LLC
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 * Neither the name of copyright holder nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "cinder/app/MouseEvent.h"
#include "poEvents.h"
#include "poNode.h"

namespace po { namespace scene {
    // EventCenter is an internal class that is used by Scene's to process their child nodes
    // and distribute events to the draw tree
    // It should never be accessed directly except by Scenes.
    
    class EventCenter;
    typedef std::shared_ptr<EventCenter> EventCenterRef;
    
    class EventCenter
    {
    public:
        static EventCenterRef create();
        
        void processEvents(std::vector<NodeRef> nodes);
        
    private:
        EventCenter();
        
        //	Event Processor
        template<typename CiEventT, typename EventT, typename EventTypeT>
		
        class EventProcessor {
        public:
            void addToQueue(EventTypeT type, CiEventT ciEvent) { mQueue[type].push_back(ciEvent); }
            
            void processEvents(std::vector<NodeRef> &nodes)
			{
                //	Go through the queue
                for (auto &eventQueue : mQueue) {
                    //	Get the type for this item in the std::map
                    EventTypeT type = (EventTypeT)eventQueue.first;
                    
                    //	Go through all the ci::MouseEvents for this type
                    for (CiEventT &ciEvent : eventQueue.second) {
                        EventT poEvent(ciEvent, type);
                        notifyAllNodes(nodes, poEvent);
                        notifyCallbacks(nodes, poEvent);
                    }
                    
                    //	Clear out the events
                    eventQueue.second.clear();
                }
            }
            
        protected:
            void notifyAllNodes(std::vector<NodeRef> &nodes, EventT event)
			{
                for (NodeRef &node : nodes) {
                    //	Check if it is valid (the item hasn't been deleted) and if it is enabled for events
                    if ( node == nullptr || (!node->isEligibleForInteractionEvent(event.getType())) ) continue;
                    
                    event.setPropagationEnabled(true);
                    
                    //	Notify the node
                    node->emitEvent(event);
                }
            }
            
            //  Extend this function to define custom callback type
            virtual void notifyCallbacks(std::vector<NodeRef> &nodes, EventT event)
            {
                //	Go through the draw tree, notifying nodes that are listening
                for (NodeRef &node : nodes) {
                    if ( node->isEligibleForInteractionEvent(event.getType()) && node->pointInside(event.getWindowPos()) ) {
                        node->emitEvent(event);
                        if (event.getPropagationEnabled()) {
                            event.setPropagationEnabled(false);
                        } else {
                            return;
                        }
                    }
                }
            }
            
            std::map<EventTypeT, std::vector<CiEventT> > mQueue;
			
        };
        
		
        //------------------------------------
        //	Mouse Events
		//------------------------------------
		
        class MouseEventProcessor
        : public EventProcessor<ci::app::MouseEvent, MouseEvent, MouseEvent::Type>
        {
            void notifyCallbacks(std::vector<NodeRef> &nodes, MouseEvent event)
            {
                
                MouseEvent::Type callbackType = MouseEvent::Type::UNKOWN;
                switch (event.getType()) {
                    case MouseEvent::Type::DOWN:
                        callbackType = MouseEvent::Type::DOWN_INSIDE;
						break;
                    case MouseEvent::Type::MOVE:
                        callbackType = MouseEvent::Type::MOVE_INSIDE;
						break;
                    case MouseEvent::Type::DRAG:
                        callbackType = MouseEvent::Type::DRAG_INSIDE;
						break;
                    case MouseEvent::Type::UP:
                        callbackType = MouseEvent::Type::UP_INSIDE;
						break;
                }
                
                // If we didnt' handle it (i.e. wheel event) just return
                if(callbackType == MouseEvent::Type::UNKOWN)
                {
                    return;
                }
                
                event.setType(callbackType);
                EventProcessor::notifyCallbacks(nodes, event);
            }
			
        };
        
        MouseEventProcessor mMouseProcessor;
        
        //	Mouse Event Cinder Callbacks
        void mouseDown(ci::app::MouseEvent event) { mMouseProcessor.addToQueue(MouseEvent::Type::DOWN, event); };
        void mouseMove(ci::app::MouseEvent event) { mMouseProcessor.addToQueue(MouseEvent::Type::MOVE, event); };
        void mouseDrag(ci::app::MouseEvent event) { mMouseProcessor.addToQueue(MouseEvent::Type::DRAG, event); };
        void mouseUp(ci::app::MouseEvent event) { mMouseProcessor.addToQueue(MouseEvent::Type::UP, event); };
        void mouseWheel(ci::app::MouseEvent event) { mMouseProcessor.addToQueue(MouseEvent::Type::WHEEL, event); };
		
		
        //------------------------------------
        //	Touch Events
		//------------------------------------
		
        class TouchEventProcessor
        : public EventProcessor<ci::app::TouchEvent::Touch, TouchEvent, TouchEvent::Type>
        {
        public:
            //	Needs to break about grouped touches from Cinder
            void addToQueue(TouchEvent::Type type, ci::app::TouchEvent event)
			{
                for (auto &ciTouch : event.getTouches()) {
                    mQueue[type].push_back(ciTouch);
                }
            }
            
        private:
            void notifyCallbacks(std::vector<NodeRef> &nodes, TouchEvent event)
            {
                //	Set the callback type
                TouchEvent::Type callbackType = TouchEvent::Type::UNKOWN;
                switch (event.getType()) {
                    case TouchEvent::Type::BEGAN:
                        callbackType = TouchEvent::Type::BEGAN_INSIDE;
						break;
                    case TouchEvent::Type::MOVED:
                        callbackType = TouchEvent::Type::MOVED_INSIDE;
						break;
                    case TouchEvent::Type::ENDED:
                        callbackType = TouchEvent::Type::ENDED_INSIDE;
						break;
                }
                
                // If we didnt' handle it just return
                if(callbackType == TouchEvent::Type::UNKOWN)
                {
                    return;
                }
                
                event.setType(callbackType);
                EventProcessor::notifyCallbacks(nodes, event);
            }
			
        };
        
        TouchEventProcessor mTouchProcessor;
        
        //	Touch Event Cinder Callbacks
        void touchesBegan(ci::app::TouchEvent event) { mTouchProcessor.addToQueue(TouchEvent::Type::BEGAN, event); };
        void touchesMoved(ci::app::TouchEvent event) { mTouchProcessor.addToQueue(TouchEvent::Type::MOVED, event); };
        void touchesEnded(ci::app::TouchEvent event) { mTouchProcessor.addToQueue(TouchEvent::Type::ENDED, event); };
		
    };
    
} } //	namespace po::scene