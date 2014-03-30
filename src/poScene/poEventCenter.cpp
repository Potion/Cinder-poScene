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
        
        ci::app::getWindow()->connectKeyDown(&EventCenter::keyDown, this);
        ci::app::getWindow()->connectKeyUp(&EventCenter::keyUp, this);
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
                po::MouseEvent poEvent(ciEvent, type);
                notifyAllNodes(nodes,   poEvent);
                notifyCallbacks(nodes,  poEvent);
            }
            
            //Clear out the events
            queue.second.clear();
        }
    }
    
    //Dispatch to the appropriate mouse event function for each node in the scene
    void EventCenter::notifyAllNodes(std::vector<NodeRef> &nodes, po::MouseEvent event) {
        for(NodeRef &node : nodes) {
            //Check if it is valid (the item hasn't been deleted) and if it is enabled for events
            if(!node->hasScene() || !node->isInteractionEnabled()) continue;
            
            event.setShouldPropagate(true);
            
            //Notify the node
            node->notifyGlobal(event);
        }
    }
    
    #pragma message "I def think this could be done in a better way, too much code"
    //Dispatch callback to top item, going up through draw tree
    void EventCenter::notifyCallbacks(std::vector<NodeRef> &nodes, po::MouseEvent event)
    {
        switch (event.getType()) {
            case MouseEvent::Type::DOWN:
                event.mType = MouseEvent::Type::DOWN_INSIDE;
                break;
                
            case MouseEvent::Type::MOVE:
                event.mType = MouseEvent::Type::MOVE_INSIDE;
                break;
                
            case MouseEvent::Type::DRAG:
                event.mType = MouseEvent::Type::DRAG_INSIDE;
                break;
                
            case MouseEvent::Type::UP:
                event.mType = MouseEvent::Type::UP_INSIDE;
                break;
        }
        
        for(NodeRef &node : nodes) {
            if(node->hasScene() &&
               node->isInteractionEnabled() &&
               node->hasConnection(event.getType()) &&
               node->pointInside(event.getWindowPos())
            ) {
                node->emitEvent(event);
                if(event.getShouldPropagate()) {
                    event.setShouldPropagate(false);
                } else {
                    return;
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
                po::KeyEvent poEvent(ciEvent, type);
                notifyAllNodes(nodes, poEvent);
            }
            
            //Clear out the events
            queue.second.clear();
        }
    }
    
    //Dispatch to the appropriate key event function for each node in the scene
    void EventCenter::notifyAllNodes(std::vector<NodeRef> &nodes, po::KeyEvent event) {
        for(NodeRef &node : nodes) {
            //Check if it is valid (the item hasn't been deleted) and if it is enabled for events
            if(!node->hasScene() || !node->isInteractionEnabled()) continue;
            
            event.setShouldPropagate(true);
            
            //Notify the node
            node->notifyGlobal(event);
        }
    }
    
    #pragma mark - Touch Events -
    
}
