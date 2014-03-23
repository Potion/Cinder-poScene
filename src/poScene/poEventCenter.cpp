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
        ci::app::getWindow()->connectMouseDown(&EventCenter::mouseDown,   this);
        ci::app::getWindow()->connectMouseMove(&EventCenter::mouseMove,   this);
        ci::app::getWindow()->connectMouseDrag(&EventCenter::mouseDrag,   this);
        ci::app::getWindow()->connectMouseUp(&EventCenter::mouseUp,       this);
        ci::app::getWindow()->connectMouseWheel(&EventCenter::mouseWheel, this);
    }
    
    void EventCenter::processEvents(SceneRef scene)
    {
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
                
                //Dispatch to the appropriate function for each node in the scene
                for(NodeRef node : scene->allChildren) {
                    switch (type) {
                        case po::MouseEvent::Type::DOWN:
                            node->mouseDown(poEvent);
                            break;
                            
                        case po::MouseEvent::Type::MOVE:
                            node->mouseMove(poEvent);
                            break;
                            
                        case po::MouseEvent::Type::DRAG:
                            node->mouseDrag(poEvent);
                            break;
                            
                        case po::MouseEvent::Type::UP:
                            node->mouseUp(poEvent);
                            break;
                            
                        case po::MouseEvent::Type::WHEEL:
                            node->mouseWheel(poEvent);
                            break;
                            
                        default:
                            break;
                    }
                }
            }
            
            //Clear out the events
            queue.second.clear();
        }
    }
}
