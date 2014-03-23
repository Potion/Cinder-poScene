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
    }
    
    
    //------------------------------------------------------------------------
    //Sort nodes to be top down
    bool sortByDrawOrderFunc(NodeRef &a, NodeRef &b) {
        return a->getDrawOrder() < b->getDrawOrder();
    }
    
    //Process all the event queues for this scene
    void EventCenter::processEvents(SceneRef scene)
    {
        std::sort(scene->allChildren.begin(), scene->allChildren.end(), sortByDrawOrderFunc);
        processMouseEvents(scene);
    }
    
    
    #pragma mark - Mouse Events -
    void EventCenter::processMouseEvents(SceneRef scene)
    {
        //Go through the queue
        for(auto& queue : mouseEventQueues) {
            //Get the type for this item in the std::map
            po::MouseEvent::Type type = (po::MouseEvent::Type)queue.first;
            
            //Go through all the ci::MouseEvents for this type
            for(ci::app::MouseEvent ciEvent : queue.second) {
                //Create a po::MouseEvent
                po::MouseEvent poEvent(type, ciEvent.getPos());
                notifyAllNodes(scene, poEvent);
            }
            
            //Clear out the events
            queue.second.clear();
        }
    }
    
    //Dispatch to the appropriate mouse event function for each node in the scene
    void EventCenter::notifyAllNodes(SceneRef scene, po::MouseEvent event) {
        for(NodeRef node : scene->allChildren) {
            if(!node->isInteractionEnabled()) continue;
            switch (event.getType()) {
                case po::MouseEvent::Type::DOWN:
                    node->mouseDown(event);
                    break;
                    
                case po::MouseEvent::Type::MOVE:
                    node->mouseMove(event);
                    break;
                    
                case po::MouseEvent::Type::DRAG:
                    node->mouseDrag(event);
                    break;
                    
                case po::MouseEvent::Type::UP:
                    node->mouseUp(event);
                    break;
                    
                case po::MouseEvent::Type::WHEEL:
                    node->mouseWheel(event);
                    break;
                    
                default:
                    break;
            }
            
            #pragma message "If we just have one mouse event handler this gets infinitely cleaner...i.e. just node->mouseEvent(event)"
        }

    }
}
