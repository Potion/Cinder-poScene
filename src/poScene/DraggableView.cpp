#include "DraggableView.h"

#include "cinder/app/App.h"
#include "poScene/ShapeView.h"

namespace po
{
	namespace scene
	{
		// Drag And Drop View
		DraggableViewRef DraggableView::create()
		{
			DraggableViewRef ref( new DraggableView() );
			ref->setup();
			return ref;
		}

		DraggableViewRef DraggableView::create( ci::vec2 snapBackPosition )
		{
			DraggableViewRef ref( new DraggableView( snapBackPosition ) );
			ref->setup();
			return ref;
		}

		void DraggableView::startHoverTimer( float seconds )
		{
			if( !mIsUsingTimer ) {
				mPreviousTime = ci::app::getElapsedSeconds();
				mIsUsingTimer = true;
			}

			mTimerDuration = seconds;

		}

		void DraggableView::stopHoverTimer()
		{
			if( mIsUsingTimer ) {
				mIsUsingTimer = false;
			}

			mHoverLastedTime = 0.f;
		}

		DraggableView::DraggableView()
			: mIsDragging( false )
			, mDraggingEventId( DraggableView::DRAGGING_EVENT_ID_NONE )
			, mSnapsBack( false )
			, mIsUsingTimer( false )
		{
		}

		DraggableView::DraggableView( ci::vec2 snapBackPosition )
			: mIsDragging( false )
			, mDraggingEventId( DraggableView::DRAGGING_EVENT_ID_NONE )
			, mSnapsBack( true )
			, mSnapPosition( snapBackPosition )
		{
			setPosition( snapBackPosition );

		}

		void DraggableView::setup()
		{
			connectEvents();
		}

		void DraggableView::update()
		{
			po::scene::View::update();

			if( !mIsUsingTimer ) {
				mPreviousTime = ci::app::getElapsedSeconds();
			}
			else {
				mHoverLastedTime = ci::app::getElapsedSeconds() - mPreviousTime;
			}

			if( mHoverLastedTime > 1.f ) {
				stopHoverTimer();
				DraggableViewRef ref = std::static_pointer_cast<DraggableView>( shared_from_this() );
				mSignalDragHoverTimersUp.emit( ref );
				mSignalDragEnded.emit( ref );
				mIsDragging = false;
			}

			//ci::app::console() << mHoverLastedTime << std::endl;
		}

		void DraggableView::setSnapBackPosition( ci::vec2 snapBackPosition )
		{
			mSnapPosition = snapBackPosition;
			mSnapsBack = true;
		}

		void DraggableView::connectEvents()
		{
			// Attach drag events
			mConnections += getSignal( po::scene::MouseEvent::DOWN_INSIDE ).connect( std::bind( &DraggableView::mouseEventHandler, this, std::placeholders::_1 ) );
			mConnections += getSignal( po::scene::MouseEvent::DRAG ).connect( std::bind( &DraggableView::mouseEventHandler, this, std::placeholders::_1 ) );
			mConnections += getSignal( po::scene::MouseEvent::UP ).connect( std::bind( &DraggableView::mouseEventHandler, this, std::placeholders::_1 ) );
		}

		void DraggableView::setInteractionEnabled( bool enabled )
		{
			bool isEnabled = isInteractionEnabled();

			if( isEnabled == enabled ) {
				return;
			}

			po::scene::View::setInteractionEnabled( enabled );

			if( !enabled ) {
				bool isDragging = mIsDragging;
				mIsDragging = false;

				if( isDragging ) {
					DraggableViewRef ref = std::static_pointer_cast<DraggableView>( shared_from_this() );
					mSignalDragCancelled.emit( ref );
				}
			}
		}

		void DraggableView::handleDragStartEvent( ci::vec2 localPos, ci::vec2 windowPos, int eventId )
		{
			ci::vec2 parentPos = getSuperview()->windowToLocal( windowPos );

			mPrevDragPosition = parentPos;
			mWindowDragPosition = windowPos;
			mDraggingEventId = eventId;

			DraggableViewRef ref = std::static_pointer_cast<DraggableView>( shared_from_this() );
			mSignalDragBegan.emit( ref );

			mIsDragging = true;
		}

		void DraggableView::handleDragEvent( ci::vec2 localPos, ci::vec2 windowPos, int eventId )
		{
			if( mIsDragging && mDraggingEventId == eventId ) {
				ci::vec2 parentPos = getSuperview()->windowToLocal( windowPos );

				setPosition( getPosition() + ( parentPos - mPrevDragPosition ) );
				mPrevDragPosition = parentPos;
				mWindowDragPosition = windowPos;

				DraggableViewRef ref = std::static_pointer_cast<DraggableView>( shared_from_this() );
				mSignalDragged.emit( ref );
			}
		}

		void DraggableView::handleDragEndEvent( ci::vec2 localPos, ci::vec2 windowPos, int eventId )
		{
			if( mIsDragging ) {
				mIsDragging = false;
				DraggableViewRef ref = std::static_pointer_cast<DraggableView>( shared_from_this() );
				mSignalDragEnded.emit( ref );
			}
		}

		void DraggableView::mouseEventHandler( po::scene::MouseEvent& event )
		{
			switch( event.getType() ) {
				// Start Dragging
				case po::scene::MouseEvent::DOWN_INSIDE:
					handleDragStartEvent( event.getLocalPos(), event.getWindowPos(), 0 );
					break;

				// Move if we are dragging
				case po::scene::MouseEvent::DRAG:
					handleDragEvent( event.getLocalPos(), event.getWindowPos(), 0 );
					break;

				// Stop any dragging
				case po::scene::MouseEvent::UP:
					handleDragEndEvent( event.getLocalPos(), event.getWindowPos(), 0 );
					break;
			}
		}

		void DraggableView::touchEventHandler( po::scene::TouchEvent& event )
		{
			switch( event.getType() ) {
				// Start Dragging
				case po::scene::TouchEvent::BEGAN_INSIDE:
					handleDragStartEvent( event.getLocalPos(), event.getWindowPos(), event.getId() );
					break;

				// Move if we are dragging
				case po::scene::TouchEvent::MOVED:
					handleDragEvent( event.getLocalPos(), event.getWindowPos(), event.getId() );
					break;

				// Stop any dragging
				case po::scene::TouchEvent::ENDED:
					handleDragEndEvent( event.getLocalPos(), event.getWindowPos(), event.getId() );
					break;
			}
		}
	}
}
