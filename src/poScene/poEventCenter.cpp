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


namespace po {
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
        
        ci::app::getWindow()->connectKeyDown(&EventCenter::keyDown, this);
        ci::app::getWindow()->connectKeyUp(&EventCenter::keyUp,     this);
    }
    
    //Process all the event queues for this scene
    void EventCenter::processEvents(std::vector<NodeRef> &nodes)
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
        processKeyEvents(nodes);
    }
    
    
    #pragma mark - Mouse Events -
    void EventCenter::processMouseEvents(std::vector<NodeRef> &nodes)
    {
        //Go through the queue
        for(auto& queue : mMouseEventQueues) {
            //Get the type for this item in the std::map
            po::MouseEvent::Type type = (po::MouseEvent::Type)queue.first;
            
            //Go through all the ci::MouseEvents for this type
            for(ci::app::MouseEvent &ciEvent : queue.second) {
                //Create a po::MouseEvent
                po::MouseEvent poEvent(ciEvent);
                notifyAllNodes(nodes,   poEvent, type);
                notifyCallbacks(nodes,  poEvent, type);
            }
            
            //Clear out the events
            queue.second.clear();
        }
    }
    
    //Dispatch to the appropriate mouse event function for each node in the scene
    void EventCenter::notifyAllNodes(std::vector<NodeRef> &nodes, po::MouseEvent event, const po::MouseEvent::Type &type) {
        for(NodeRef &node : nodes) {
            //Check if it is valid (the item hasn't been deleted) and if it is enabled for events
            if(!node->hasScene() || !node->isInteractionEnabled()) continue;
            
            event.setShouldPropagate(true);
            
            //Notify the node
            node->notifyGlobal(event, type);
        }
    }
    
    //Dispatch callback to top item, going up through draw tree
    void EventCenter::notifyCallbacks(std::vector<NodeRef> &nodes, po::MouseEvent event, const po::MouseEvent::Type &type)
    {
        po::MouseEvent::Type callbackType;
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
            if(node->hasScene() &&
               node->isInteractionEnabled() &&
               node->hasConnection(callbackType) &&
               node->pointInside(event.getWindowPos())
            ) {
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
            po::TouchEvent::Type type = (po::TouchEvent::Type)queue.first;
            
            //Go through all the ci::MouseEvents for this type
            for(ci::app::TouchEvent &ciEvent : queue.second) {
                //Create a po::MouseEvent
                po::TouchEvent poEvent(ciEvent);
                notifyAllNodes(nodes, poEvent, type);
                notifyCallbacks(nodes, poEvent, type);
            }
            
            //Clear out the events
            queue.second.clear();
        }
    }
    
    //Dispatch to the appropriate touch event function for each node in the scene
    void EventCenter::notifyAllNodes(std::vector<NodeRef> &nodes, po::TouchEvent event, const po::TouchEvent::Type &type) {
        for(NodeRef &node : nodes) {
            //Check if it is valid (the item hasn't been deleted) and if it is enabled for events
            if(!node->hasScene() || !node->isInteractionEnabled()) continue;
            
            event.setShouldPropagate(true);
            
            //Notify the node
            node->notifyGlobal(event, type);
        }
    }    //Dispatch callback to top item, going up through draw tree
    
    
    void EventCenter::notifyCallbacks(std::vector<NodeRef> &nodes, po::TouchEvent event, po::TouchEvent::Type &type)
    {
        //Set the callback type
        po::TouchEvent::Type callbackType;
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
            if(node->hasScene() &&
               node->isInteractionEnabled() &&
               node->hasConnection(callbackType)
               )
            {
                
                //Go through all the touches, see if any are inside of this item
                std::vector<po::TouchEvent::Touch> foundTouches;
                for(po::TouchEvent::Touch &touch : event.getTouches()) {
                    if(node->pointInside(touch.getWindowPos())) {
                        foundTouches.push_back(touch);
                    }
                }
                
                //If we found one or more that are inside,
                //create a new event with just these touches
                if(foundTouches.size()) {
                    po::TouchEvent t = event;
                    t.mTouches       = foundTouches;
                    node->emitEvent(t, callbackType);
                    if(!t.getShouldPropagate()) {
                        return;
                    }
                }
            }
        }
    }
    
    
    #pragma mark - KeyEvents -
    void EventCenter::processKeyEvents(std::vector<NodeRef> &nodes)
    {
        //Go through the queue
        for(auto& queue : mKeyEventQueues) {
            //Get the type for this item in the std::map
            po::KeyEvent::Type type = (po::KeyEvent::Type)queue.first;
            
            //Go through all the ci::MouseEvents for this type
            for(ci::app::KeyEvent &ciEvent : queue.second) {
                //Create a po::MouseEvent
                po::KeyEvent poEvent(ciEvent);
                notifyAllNodes(nodes, poEvent, type);
            }
            
            //Clear out the events
            queue.second.clear();
        }
    }
    
    //Dispatch to the appropriate key event function for each node in the scene
    void EventCenter::notifyAllNodes(std::vector<NodeRef> &nodes, po::KeyEvent event, const po::KeyEvent::Type &type) {
        for(NodeRef &node : nodes) {
            //Check if it is valid (the item hasn't been deleted) and if it is enabled for events
            if(!node->hasScene() || !node->isInteractionEnabled()) continue;
            
            event.setShouldPropagate(true);
            
            //Notify the node
            node->notifyGlobal(event, type);
        }
    }
}
