#include "DragAndDrop.h"

#include "poScene/ShapeView.h"

namespace po
{
	namespace scene
	{
		// -----------------------------------------------
		// Drop Zone View
		DropZoneViewRef DropZoneView::create()
		{
			DropZoneViewRef ref( new DropZoneView() );
			ref->setup();
			return ref;
		}

		DropZoneView::DropZoneView()
			: mBackgroundView( View::create() )
			, mIsHighlighted( false )
			, mCanHoldMultipleViews( false )
			, mIsHighlightingEnabled( true )
		{
		}

		void DropZoneView::setup()
		{
			setHighlighted( false );
			addSubview( mBackgroundView );
		}

		ci::Rectf DropZoneView::getBounds()
		{
			return mBackgroundView->getBounds();
		}

		void DropZoneView::setHighlighted( bool highlighted )
		{
			if( !mIsHighlightingEnabled ) { return; }

			if( highlighted ) {
				mBackgroundView->setAlpha( 1.0f );
			}
			else {
				mBackgroundView->setAlpha( 0.5f );
			}

			mIsHighlighted = highlighted;
		}

		bool DropZoneView::addDraggableView( DraggableViewRef view )
		{
			if( !mCanHoldMultipleViews && mDroppedViews.size() > 0 ) {
				return false;
			}

			mDroppedViews.push_back( view );

			return true;
		}

		bool DropZoneView::removeDraggableView( DraggableViewRef view )
		{
			auto it = std::find( mDroppedViews.begin(), mDroppedViews.end(), view );
			bool found = it != mDroppedViews.end();

			if( found ) {
				mDroppedViews.erase( it );
			}

			return found;
		}

		bool DropZoneView::hasDraggableView( DraggableViewRef view )
		{
			auto it = std::find( mDroppedViews.begin(), mDroppedViews.end(), view );
			return it != mDroppedViews.end();
		}

		// -----------------------------------------------
		// View Controller
		DragAndDropViewControllerRef DragAndDropViewController::create( bool hoverEnabled, float hoverTime )
		{
			DragAndDropViewControllerRef ref( new DragAndDropViewController() );
			ref->setup( hoverEnabled, hoverTime );
			return ref;
		}

		DragAndDropViewController::DragAndDropViewController()
			: mIsHoverEnabled( false ),
			  mHoverDelayTime( 0.f )
		{
		}

		void DragAndDropViewController::setup( bool hoverEnabled, float hoverTime )
		{
			mIsHoverEnabled = hoverEnabled;
			mHoverDelayTime = hoverTime;

		}

		void DragAndDropViewController::trackDraggableView( DraggableViewRef view, DropZoneViewRef dropZone )
		{
			if( std::find( mDraggableViews.begin(), mDraggableViews.end(), view ) == mDraggableViews.end() ) {
				mDraggableViews.push_back( view );

				mConnections += view->getSignalDragBegan().connect( std::bind( &DragAndDropViewController::viewDragBeganHandler, this, std::placeholders::_1 ) );
				mConnections += view->getSignalDragged().connect( std::bind( &DragAndDropViewController::viewDraggedHandler, this, std::placeholders::_1 ) );
				mConnections += view->getSignalDragEnded().connect( std::bind( &DragAndDropViewController::viewDragEndedHandler, this, std::placeholders::_1 ) );
				mConnections += view->getSignalDragCancelled().connect( std::bind( &DragAndDropViewController::viewDragEndedHandler, this, std::placeholders::_1 ) );
				mConnections += view->getSignalDragHoverTimersUp().connect( std::bind( &DragAndDropViewController::viewDragEndedHandler, this, std::placeholders::_1 ) );

			}

			mDraggableViewValidDropZones[view].push_back( dropZone );
		}

		void DragAndDropViewController::trackDropZoneView( DropZoneViewRef view )
		{
			if( std::find( mDropZoneViews.begin(), mDropZoneViews.end(), view ) != mDropZoneViews.end() ) {
				//Already exists, throw exception or something?
				return;
			}

			mDropZoneViews.push_back( view );
		}

		std::vector<DropZoneViewRef> DragAndDropViewController::getValidDropZonesForView( DraggableViewRef view )
		{
			if( mDraggableViewValidDropZones.count( view ) == 0 ) {
				return std::vector<DropZoneViewRef>();
			}
			else {
				return mDraggableViewValidDropZones[view];
			}
		}
		bool DragAndDropViewController::viewIsValidForDropZone( DraggableViewRef view, DropZoneViewRef dropZone )
		{
			if( mDraggableViewValidDropZones.count( view ) != 0 ) {
				if( std::find( mDraggableViewValidDropZones[view].begin(), mDraggableViewValidDropZones[view].end(), dropZone ) != mDraggableViewValidDropZones[view].end() ) {
					return true;
				}
			}

			return false;
		}

		bool DragAndDropViewController::checkForIntersection( DraggableViewRef view, DropZoneViewRef dropZone )
		{
			return ( dropZone->pointInside( view->getDragWindowPos() ) );
		}

		bool DragAndDropViewController::checkForIntersection( ViewRef view1, ViewRef view2 )
		{
			if( view1->hasSuperview() && view2->hasSuperview() ) {
				ci::vec2 tl = view1->getSuperview()->localToWindow( view1->getFrame().getUpperLeft() );
				ci::vec2 br = view1->getSuperview()->localToWindow( view1->getFrame().getLowerRight() );

				//Flip rect coords if they are rotated upside down
				ci::Rectf view1GlobalRect = tl.y > br.y ? ci::Rectf( br, tl ) : ci::Rectf( tl, br );

				tl = view2->getSuperview()->localToWindow( view2->getFrame().getUpperLeft() );
				br = view2->getSuperview()->localToWindow( view2->getFrame().getLowerRight() );

				//Flip rect coords if they are rotated upside down
				ci::Rectf view2GlobalRect = tl.y > br.y ? ci::Rectf( br, tl ) : ci::Rectf( tl, br );

				if( tl.y > br.y ) {
					view2GlobalRect = ci::Rectf( br, tl );
				}
				else {
					view2GlobalRect = ci::Rectf( tl, br );
				}

				//Check for intersection
				ci::Rectf intersection = view1GlobalRect.getClipBy( view2GlobalRect );
				return ( intersection.getWidth() > 0 && intersection.getHeight() > 0 );
			}

			return false;
		}

		void DragAndDropViewController::setDraggingEnabled( bool enabled )
		{
			for( auto& draggableView : mDraggableViews ) {
				draggableView->setInteractionEnabled( enabled );
			}
		}

		void DragAndDropViewController::viewDragBeganHandler( DraggableViewRef& view )
		{
			// See if the view has a drop zone
			for( int i = 0; i < mDraggableViewValidDropZones[view].size(); i++ ) {
				DropZoneViewRef dropZone = mDraggableViewValidDropZones[view][i];

				if( dropZone->hasDraggableView( view ) ) {
					// If so, remove it and notify
					dropZone->removeDraggableView( view );
					mSignalViewRemovedFromDropZone.emit( dropZone, view );
				}
			}
		}

		bool DragAndDropViewController::setDropZoneHighlightForView( DraggableViewRef view, DropZoneViewRef dropZone )
		{
			if( checkForIntersection( view, dropZone ) ) {
				dropZone->setHighlighted( true );
				return true;
			}
			else {
				dropZone->setHighlighted( false );
				return false;
			}
		}
		void DragAndDropViewController::viewDraggedHandler( DraggableViewRef& view )
		{
			if( mDraggableViewValidDropZones.count( view ) != 0 ) {
				for( auto& dropZone : mDraggableViewValidDropZones[view] ) {
					if( setDropZoneHighlightForView( view, dropZone ) ) {
						if( !mIsHoverEnabled ) { return; }

						if( view->getIsTimerStopped() ) {
							view->startHoverTimer( mHoverDelayTime );
						}

						return;
					}
				}
			}
		}

		void DragAndDropViewController::viewDragEndedHandler( DraggableViewRef& view )
		{
			if( mDraggableViewValidDropZones.count( view ) != 0 ) {
				for( auto& dropZone : mDraggableViewValidDropZones[view] ) {
					if( checkForIntersection( view, dropZone ) ) {
						if( dropZone->addDraggableView( view ) ) {
							if( !view->getIsTimerStopped() && mIsHoverEnabled ) {
								view->stopHoverTimer();
							}

							mSignalViewAddedToDropZone.emit( dropZone, view );


							return;
						}
					}
				}
			}

			// Snap it back if needed
			if( view->getSnapsBackToPosition() ) {
				view->snapBackToPosition();
			}
		}

		void DragAndDropViewController::viewDragCancelledHandler( DraggableViewRef& view )
		{
			// Snap it back if needed
			if( view->getSnapsBackToPosition() ) {
				view->snapBackToPosition();
			}
		}
	}
}
