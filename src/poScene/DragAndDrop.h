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

	virtual void setHighlighted(bool highlighted=true);

	virtual bool addDraggableView(DraggableViewRef view);
	virtual bool removeDraggableView(DraggableViewRef view);

protected:

private:
	DropZoneView();
	void setup();

	bool mIsHighlighted;
	ViewRef mBackgroundView;
	ViewRef mDraggableViewsHolder;
};

class DragAndDropViewController;
typedef std::shared_ptr<DragAndDropViewController> DragAndDropViewControllerRef;

class DragAndDropViewController : public ViewController {
public:
	static DragAndDropViewControllerRef create();

	void trackDragAndDropView(DraggableViewRef view);
	void trackDropZoneView(DropZoneViewRef view);

	const std::vector<DraggableViewRef> &getDraggableViews() { return mDraggableViews; };
	const std::vector<DropZoneViewRef> &getDropZoneViews() { return mDropZoneViews; };

protected:
	virtual void viewRemovedFromDropZone(DraggableViewRef view);

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
};
}}