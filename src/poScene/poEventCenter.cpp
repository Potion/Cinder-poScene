//
//  poEventCenter.cpp
//  BasicTest
//
//  Created by Stephen Varga on 3/17/14.
//
//
#include "cinder/app/Window.h"

#include "poEventCenter.h"
#include "poScene.h"


namespace po { namespace scene {
    //------------------------------------------------------------------
    EventCenterRef EventCenter::create() {
        return EventCenterRef(new EventCenter());
    }
    
    EventCenter::EventCenter()
    {
        //Connect mouse events
        ci::app::getWindow()->connectMouseDown(&EventCenter::mouseDown,   this);
        ci::app::getWindow()->connectMouseMove(&EventCenter::mouseMove,   this);
        ci::app::getWindow()->connectMouseDrag(&EventCenter::mouseDrag,   this);
        ci::app::getWindow()->connectMouseUp(&EventCenter::mouseUp,       this);
        ci::app::getWindow()->connectMouseWheel(&EventCenter::mouseWheel, this);
        
        ci::app::getWindow()->connectTouchesBegan(&EventCenter::touchesBegan, this);
        ci::app::getWindow()->connectTouchesMoved(&EventCenter::touchesMoved, this);
        ci::app::getWindow()->connectTouchesEnded(&EventCenter::touchesEnded, this);
    }
    
    //Process all the event queues for this scene
    void EventCenter::processEvents(std::vector<NodeRef> nodes)
    {
        //Sort nodes to be top down
        std::sort(nodes.begin(), nodes.end(),
                  [&nodes] (const NodeRef &a, const NodeRef &b)
                  {
                      return a->getDrawOrder() > b->getDrawOrder();
                  }
        );
        
        //Process them
        processMouseEvents(nodes);
        processTouchEvents(nodes);
    }
    
    
    #pragma mark - Mouse Events -
    void EventCenter::processMouseEvents(std::vector<NodeRef> &nodes)
    {
        //Go through the queue
        for(auto& queue : mMouseEventQueues) {
            //Get the type for this item in the std::map
            MouseEvent::Type type = (MouseEvent::Type)queue.first;
            
            //Go through all the ci::MouseEvents for this type
            for(ci::app::MouseEvent &ciEvent : queue.second) {
                //Create a po::MouseEvent
                MouseEvent poEvent(ciEvent);
                notifyAllNodes(nodes,   poEvent, type);
                notifyCallbacks(nodes,  poEvent, type);
            }
            
            //Clear out the events
            queue.second.clear();
        }
    }
    
    //Dispatch to the appropriate mouse event function for each node in the scene
    void EventCenter::notifyAllNodes(std::vector<NodeRef> &nodes, MouseEvent event, const MouseEvent::Type &type) {
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
    
    //Dispatch callback to top item, going up through draw tree
    void EventCenter::notifyCallbacks(std::vector<NodeRef> &nodes, MouseEvent event, const MouseEvent::Type &type)
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
        
        for(NodeRef &node : nodes) {
            if(node->isEligibleForInteractionEvent(callbackType) && node->pointInside(event.getWindowPos())) {
                node->emitEvent(event, callbackType);
                if(event.getShouldPropagate()) {
                    event.setShouldPropagate(false);
                } else {
                    return;
                }
            }
        }
    }
    
    #pragma mark - Touch Events -
    void EventCenter::processTouchEvents(std::vector<NodeRef> &nodes)
    {
        //Go through the queue
        for(auto& queue : mTouchEventQueues) {
            //Get the type for this item in the std::map
            TouchEvent::Type type = (TouchEvent::Type)queue.first;
            
            //Go through all the ci::TouchEvents for this type
            for(ci::app::TouchEvent &ciTouchEvent : queue.second) {
                for(ci::app::TouchEvent::Touch &ciTouch : ciTouchEvent.getTouches()) {
                    TouchEvent poEvent(ciTouch);
                    notifyAllNodes(nodes, poEvent, type);
                    notifyCallbacks(nodes, poEvent, type);
                }
            }
            
            //Clear out the events
            queue.second.clear();
        }
    }
    
    //Dispatch to the appropriate touch event function for each node in the scene
    void EventCenter::notifyAllNodes(std::vector<NodeRef> &nodes, TouchEvent event, const TouchEvent::Type &type) {
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
    
    
    //Dispatch callback to top item, going up through draw tree
    void EventCenter::notifyCallbacks(std::vector<NodeRef> &nodes, TouchEvent event, TouchEvent::Type &type)
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
        
        //Go through the draw tree, notifying nodes that are listening
        for(NodeRef &node : nodes) {
            if(node->isEligibleForInteractionEvent(callbackType) && node->pointInside(event.getWindowPos()))
            {
                node->emitEvent(event, callbackType);
                if(event.getShouldPropagate()) {
                    event.setShouldPropagate(false);
                } else {
                    return;
                }
            }
        }
    }
} } //  Namespace: po::scene