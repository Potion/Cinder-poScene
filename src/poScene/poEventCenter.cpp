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

#include "cinder/app/Window.h"
#include "cinder/app/App.h"

#include "poEventCenter.h"
#include "poScene.h"

namespace po { namespace scene {

	EventCenterRef EventCenter::create()
	{
		return EventCenterRef(new EventCenter());
	}
	
	EventCenter::EventCenter()
	{
		//	Connect mouse events
        ci::app::getWindow()->getSignalMouseDown().connect(std::bind(&EventCenter::mouseDown, this, std::placeholders::_1));
        ci::app::getWindow()->getSignalMouseMove().connect(std::bind(&EventCenter::mouseMove, this, std::placeholders::_1));
        ci::app::getWindow()->getSignalMouseDrag().connect(std::bind(&EventCenter::mouseDrag, this, std::placeholders::_1));
        ci::app::getWindow()->getSignalMouseUp().connect(std::bind(&EventCenter::mouseUp, this, std::placeholders::_1));
        ci::app::getWindow()->getSignalMouseWheel().connect(std::bind(&EventCenter::mouseWheel, this, std::placeholders::_1));
		
        //	Connect touch events
        ci::app::getWindow()->getSignalTouchesBegan().connect(std::bind(&EventCenter::touchesBegan, this, std::placeholders::_1));
        ci::app::getWindow()->getSignalTouchesMoved().connect(std::bind(&EventCenter::touchesMoved, this, std::placeholders::_1));
        ci::app::getWindow()->getSignalTouchesEnded().connect(std::bind(&EventCenter::touchesEnded, this, std::placeholders::_1));
	}
	
	//	Process all the event queues for this scene
	void EventCenter::processEvents(std::vector<NodeRef> nodes)
	{
		//	Sort nodes to be top down
		std::sort(nodes.begin(), nodes.end(),
			[&nodes] (const NodeRef &a, const NodeRef &b) {
			  return a->getDrawOrder() > b->getDrawOrder();
			}
		);
		
		//	Process them
		mMouseProcessor.processEvents(nodes);
		mTouchProcessor.processEvents(nodes);
	}
		
} } //  namespace po::scene