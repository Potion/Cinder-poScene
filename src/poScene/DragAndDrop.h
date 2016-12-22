#pragma once

#include "cinder/Signals.h"

#include "poScene/View.h"
#include "poScene/ViewController.h"
#include "poScene/DraggableView.h"

namespace po { namespace scene {

typedef std::shared_ptr<class DropZoneView> DropZoneViewRef;


class DropZoneView : public View {
public:
	static DropZoneViewRef create();

	ci::Rectf getBounds();

	bool getCanHoldMultipleViews() { return mCanHoldMultipleViews; };
	void setCanHoldMultipleViews(bool canHoldMultipleViews) { mCanHoldMultipleViews = canHoldMultipleViews; };

	bool isHoldingViews() { return mDraggableViewsHolder->hasChildren(); };
	virtual void setHighlighted(bool highlighted=true);

	bool addDraggableView(DraggableViewRef view);
	bool removeDraggableView(DraggableViewRef view);

	ViewRef getBackgroundView() { return mBackgroundView; };

protected:
	DropZoneView();
	void setup();

private:
	bool mCanHoldMultipleViews;
	bool mIsHighlighted;
	ViewRef mBackgroundView;
	ViewRef mDraggableViewsHolder;
};

class DragAndDropViewController;
typedef std::shared_ptr<DragAndDropViewController> DragAndDropViewControllerRef;


typedef ci::signals::Signal<void( DropZoneViewRef&, DraggableViewRef& )> DragAndDropSignal;

class DragAndDropViewController : public ViewController {
public:
	static DragAndDropViewControllerRef create();

	DragAndDropSignal& getSignalViewAddedToDropZone() { return mSignalViewAddedToDropZone; };
	DragAndDropSignal& getSignalViewRemovedFromDropZone() { return mSignalViewRemovedFromDropZone; };

	void trackDraggableView(DraggableViewRef view);
	void trackDropZoneView(DropZoneViewRef view);

	const std::vector<DraggableViewRef> &getDraggableViews() { return mDraggableViews; };
	const std::vector<DropZoneViewRef> &getDropZoneViews() { return mDropZoneViews; };

protected:

private:
	DragAndDropViewController();

	void setup();

	bool checkForIntersection(ViewRef view1, ViewRef view2);

	std::vector<DraggableViewRef> mDraggableViews;
	std::vector<po::scene::DropZoneViewRef> mDropZoneViews;

	// Event Handlers
	void viewDragBeganHandler(po::scene::DraggableViewRef &view);
	void viewDraggedHandler(po::scene::DraggableViewRef &view);
	void viewDragEndedHandler(po::scene::DraggableViewRef &view);

	ci::signals::ConnectionList mConnections;

	DragAndDropSignal mSignalViewAddedToDropZone, mSignalViewRemovedFromDropZone;
};
}}