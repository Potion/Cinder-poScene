#pragma once

#include "cinder/Signals.h"

#include "poScene/View.h"
#include "poScene/ViewController.h"
#include "poScene/DraggableView.h"

namespace po
{
	namespace scene
	{

		typedef std::shared_ptr<class DropZoneView> DropZoneViewRef;


		class DropZoneView : public View
		{
			public:
				static DropZoneViewRef create();

				ci::Rectf getBounds();

				bool getCanHoldMultipleViews() { return mCanHoldMultipleViews; };
				void setCanHoldMultipleViews( bool canHoldMultipleViews ) { mCanHoldMultipleViews = canHoldMultipleViews; };

				bool isHoldingViews() { return mDroppedViews.size() > 0; };
				virtual void setHighlighted( bool highlighted = true );

				bool addDraggableView( DraggableViewRef view );
				bool removeDraggableView( DraggableViewRef view );
				bool hasDraggableView( DraggableViewRef view );
				void setEnableHighlight( bool enable )
				{
					mIsHighlightingEnabled = enable;
					mBackgroundView->setAlpha( 1.0f );
				};

				ViewRef getBackgroundView() { return mBackgroundView; };

			protected:
				DropZoneView();
				void setup();

			private:
				bool mCanHoldMultipleViews;
				bool mIsHighlighted;
				bool mIsHighlightingEnabled;
				ViewRef mBackgroundView;
				std::vector<DraggableViewRef> mDroppedViews;


		};

		class DragAndDropViewController;
		typedef std::shared_ptr<DragAndDropViewController> DragAndDropViewControllerRef;


		typedef ci::signals::Signal<void( DropZoneViewRef&, DraggableViewRef& )> DragAndDropSignal;

		class DragAndDropViewController : public ViewController
		{
			public:
				static DragAndDropViewControllerRef create( bool hoverEnabled = false, float hoverTime = 0.f );

				DragAndDropSignal& getSignalViewAddedToDropZone() { return mSignalViewAddedToDropZone; };
				DragAndDropSignal& getSignalViewRemovedFromDropZone() { return mSignalViewRemovedFromDropZone; };

				void trackDraggableView( DraggableViewRef view, DropZoneViewRef dropZone );
				void trackDropZoneView( DropZoneViewRef view );

				const std::vector<DraggableViewRef>& getDraggableViews() { return mDraggableViews; };
				const std::vector<DropZoneViewRef>& getDropZoneViews() { return mDropZoneViews; };

				std::vector<DropZoneViewRef> getValidDropZonesForView( DraggableViewRef view );
				bool viewIsValidForDropZone( DraggableViewRef view, DropZoneViewRef dropZone );

			protected:

			private:
				DragAndDropViewController();

				void setup( bool hoverEnabled = false, float hoverTime = 0.f );

				bool checkForIntersection( DraggableViewRef view, DropZoneViewRef dropZone );
				bool checkForIntersection( ViewRef view1, ViewRef view2 );

				void setDraggingEnabled( bool enabled );

				std::vector<DraggableViewRef> mDraggableViews;
				std::vector<DropZoneViewRef> mDropZoneViews;

				std::map<DraggableViewRef, std::vector<DropZoneViewRef> > mDraggableViewValidDropZones;

				bool setDropZoneHighlightForView( DraggableViewRef view, DropZoneViewRef dropZone );

				// Event Handlers
				void viewDragBeganHandler( DraggableViewRef& view );
				void viewDraggedHandler( DraggableViewRef& view );
				void viewDragEndedHandler( DraggableViewRef& view );
				void viewDragCancelledHandler( DraggableViewRef& view );

				ci::signals::ConnectionList mConnections;

				DragAndDropSignal mSignalViewAddedToDropZone, mSignalViewRemovedFromDropZone;
				// Hover
				bool mIsHoverEnabled;
				float mHoverDelayTime;

		};
	}
}