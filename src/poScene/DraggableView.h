#pragma once

#include "poScene/View.h"

#include "cinder/Signals.h"

namespace po
{
	namespace scene
	{
		typedef std::shared_ptr<class DraggableView> DraggableViewRef;

		class DraggableView : public po::scene::View
		{
			public:
				typedef ci::signals::Signal<void( DraggableViewRef& )> DraggableViewSignal;

				static DraggableViewRef create();
				static DraggableViewRef create( ci::vec2 snapBackPosition );

				void setup() override;
				void update() override;

				virtual void connectEvents();

				virtual void setInteractionEnabled( bool enabled ) override;


				virtual void snapBackToPosition() { setPosition( mSnapPosition ); };

				void setSnapBackPosition( ci::vec2 snapBackPosition );
				ci::vec2 getSnapBackPosition() { return mSnapPosition; }

				void setSnapsBackToPosition( bool snapsBackToPosition ) { mSnapsBack = snapsBackToPosition; }
				bool getSnapsBackToPosition() { return mSnapsBack; }

				ci::vec2 getDragWindowPos() { return mWindowDragPosition; };

				DraggableViewSignal& getSignalDragBegan()	{ return mSignalDragBegan;	};
				DraggableViewSignal& getSignalDragged()		{ return mSignalDragged;	};
				DraggableViewSignal& getSignalDragEnded()	{ return mSignalDragEnded;	};
				DraggableViewSignal& getSignalDragCancelled() { return mSignalDragCancelled; };
				DraggableViewSignal& getSignalDragHoverTimersUp() { return mSignalDragHoverTimersUp; };

				void startHoverTimer( float seconds );
				void stopHoverTimer();
				bool getIsTimerStopped() { return !mIsUsingTimer; }

			protected:
				DraggableView();
				DraggableView( ci::vec2 snapBackPosition );

				bool mIsDragging;
				ci::vec2 mDragPosition, mPrevDragPosition;
				ci::vec2 mWindowDragPosition;

				static const int DRAGGING_EVENT_ID_NONE = -1;
				int mDraggingEventId;

				// Signals
				DraggableViewSignal mSignalDragBegan, mSignalDragged, mSignalDragEnded, mSignalDragCancelled, mSignalDragHoverTimersUp;

				void handleDragStartEvent( ci::vec2 localPos, ci::vec2 windowPos, int eventID );
				void handleDragEvent( ci::vec2 localPos, ci::vec2 windowPos, int eventID );
				void handleDragEndEvent( ci::vec2 localPos, ci::vec2 windowPos, int eventID );

			private:
				void mouseEventHandler( po::scene::MouseEvent& event );
				void touchEventHandler( po::scene::TouchEvent& event );

				bool mSnapsBack;
				ci::vec2 mSnapPosition;
				ci::signals::ConnectionList	mConnections;

				// Time
				float mPreviousTime;
				float mHoverLastedTime;
				bool  mIsUsingTimer;
				float mTimerDuration;
		};
	}
}