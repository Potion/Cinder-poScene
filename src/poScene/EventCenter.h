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

#include "cinder/app/App.h"

#include "poScene/Events.h"
#include "poScene/View.h"

namespace po
{
	namespace scene
	{
		// EventCenter is an internal class that is used by Scene's to process their child views
		// and distribute events to the draw tree
		// It should never be accessed directly except by Scenes.

		// Base class for event processors
		class EventProcessorBase
		{
			public:
				virtual void connectEvents() = 0;
				virtual void processEvents( std::vector<ViewRef>& views ) = 0;
			private:
		};

		typedef std::shared_ptr<EventProcessorBase> EventProcessorBaseRef;

		// Main Class
		typedef std::shared_ptr<class EventCenter> EventCenterRef;

		class EventCenter
		{
			public:
				static EventCenterRef create();

				void addEventProcessor( const EventProcessorBaseRef& processor )
				{
					mEventProcessors.push_back( processor );
					mEventProcessors.back()->connectEvents();
				};

				void processEvents( std::vector<ViewRef> views );

			private:
				EventCenter();

				std::vector<EventProcessorBaseRef> mEventProcessors;
		};

		//	Event Processor, create one of these for each category of event, i.e. touch or mouse
		template<typename CiEventT, typename EventT, typename EventTypeT, typename SignalTypeT>
		class EventProcessor : public EventProcessorBase
		{
			public:
				virtual void connectEvents() = 0;
				virtual void addToQueue( EventTypeT type, CiEventT ciEvent ) { mQueue[type].push_back( ciEvent ); }

				void processEvents( std::vector<ViewRef>& views )
				{
					//	Go through the queue
					for( auto& eventQueue : mQueue ) {
						//	Get the type for this item in the std::map
						EventTypeT type = ( EventTypeT )eventQueue.first;

						//	Go through all the events for this type
						for( CiEventT& ciEvent : eventQueue.second ) {
							EventT poEvent( ciEvent, type );
							notifyAllviews( views, poEvent );
							notifyCallbacks( views, poEvent );
						}

						//	Clear out the events
						eventQueue.second.clear();
					}
				}

			protected:
				void notifyAllviews( std::vector<ViewRef>& views, EventT event )
				{
					for( ViewRef& view : views ) {
						//	Check if it is valid (the item hasn't been deleted) and if it is enabled for events
						if( view == nullptr || !( view->isEligibleForInteractionEventT<EventT, EventTypeT, SignalTypeT>( event.getType() ) ) ) { continue; }

						event.setPropagationEnabled( true );

						//	Notify the view
						view->emitEventT<EventT, EventTypeT, SignalTypeT>( event );
					}
				}

				//  Extend this function to define custom callback type
				virtual void notifyCallbacks( std::vector<ViewRef>& views, EventT& event )
				{
					//	Go through the draw tree, notifying views that are listening
					for( ViewRef& view : views ) {
						if( view->isEligibleForInteractionEvents()
						        && view->isEligibleForInteractionEventT<EventT, EventTypeT, SignalTypeT>( event.getType() )
						        && hitTest( view, event ) ) {
							view->emitEventT<EventT, EventTypeT, SignalTypeT>( event );

							if( event.getPropagationEnabled() ) {
								event.setPropagationEnabled( false );
							}
							else {
								return;
							}
						}
					}
				}

				// Hit testing, override in subclass for custom hit testing, i.e. overlapping shapes
				virtual bool hitTest( const ViewRef& view, const EventT& event )
				{
					return view->pointInside( event.getWindowPos() );
				}

				std::map<EventTypeT, std::vector<CiEventT> > mQueue;
		};




		//------------------------------------
		//	Mouse Events
		//------------------------------------
		class MouseEventProcessor
			: public EventProcessor<ci::app::MouseEvent, MouseEvent, MouseEvent::Type, MouseEventSignal>
		{
			public:
				MouseEventProcessor() {};

				void connectEvents() override
				{
					mConnections += ci::app::getWindow()->getSignalMouseDown().connect( std::bind( &MouseEventProcessor::addToQueue,	this, MouseEvent::Type::DOWN,	std::placeholders::_1 ) );
					mConnections += ci::app::getWindow()->getSignalMouseMove().connect( std::bind( &MouseEventProcessor::addToQueue,	this, MouseEvent::Type::MOVE,	std::placeholders::_1 ) );
					mConnections += ci::app::getWindow()->getSignalMouseDrag().connect( std::bind( &MouseEventProcessor::addToQueue,	this, MouseEvent::Type::DRAG,	std::placeholders::_1 ) );
					mConnections += ci::app::getWindow()->getSignalMouseUp().connect( std::bind( &MouseEventProcessor::addToQueue,	this, MouseEvent::Type::UP,		std::placeholders::_1 ) );
					mConnections += ci::app::getWindow()->getSignalMouseWheel().connect( std::bind( &MouseEventProcessor::addToQueue, this, MouseEvent::Type::WHEEL,	std::placeholders::_1 ) );
				}

				void notifyCallbacks( std::vector<ViewRef>& views, MouseEvent& event ) override
				{
					MouseEvent::Type callbackType = MouseEvent::Type::UNKOWN;

					switch( event.getType() ) {
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
					if( callbackType == MouseEvent::Type::UNKOWN ) {
						return;
					}

					event.setType( callbackType );
					EventProcessor::notifyCallbacks( views, event );
				}

			private:
				ci::signals::ConnectionList	mConnections;
		};



		//------------------------------------
		//	Touch Events
		//------------------------------------
		class TouchEventProcessor
			: public EventProcessor<ci::app::TouchEvent::Touch, TouchEvent, TouchEvent::Type, TouchEventSignal>
		{
			public:
				void connectEvents() override
				{
					//	Connect touch events
					mConnections += ci::app::getWindow()->getSignalTouchesBegan().connect( std::bind( &TouchEventProcessor::addToQueue, this, TouchEvent::Type::BEGAN, std::placeholders::_1 ) );
					mConnections += ci::app::getWindow()->getSignalTouchesMoved().connect( std::bind( &TouchEventProcessor::addToQueue, this, TouchEvent::Type::MOVED, std::placeholders::_1 ) );
					mConnections += ci::app::getWindow()->getSignalTouchesEnded().connect( std::bind( &TouchEventProcessor::addToQueue, this, TouchEvent::Type::ENDED, std::placeholders::_1 ) );
				}

				//	Needs to break about grouped touches from Cinder
				void addToQueue( TouchEvent::Type type, ci::app::TouchEvent event )
				{
					for( auto& ciTouch : event.getTouches() ) {
						mQueue[type].push_back( ciTouch );
					}
				}

			private:
				void notifyCallbacks( std::vector<ViewRef>& views, TouchEvent& event ) override
				{
					//	Set the callback type
					TouchEvent::Type callbackType = TouchEvent::Type::UNKOWN;

					switch( event.getType() ) {
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
					if( callbackType == TouchEvent::Type::UNKOWN ) {
						return;
					}

					event.setType( callbackType );
					EventProcessor::notifyCallbacks( views, event );
				}

				ci::signals::ConnectionList	mConnections;
		};

	}
} //	namespace po::scene