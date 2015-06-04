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
        mMouseProcessor.processEvents(nodes);
        mTouchProcessor.processEvents(nodes);
    }
} } //  Namespace: po::scene