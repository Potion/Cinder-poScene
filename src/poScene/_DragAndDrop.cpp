#include "DragAndDrop.h"

#include "poScene/ShapeView.h"

namespace po { namespace scene {

	// Drop Zone View
	DropZoneViewRef DropZoneView::create() {
		DropZoneViewRef ref(new DropZoneView());
		ref->setup();
		return ref;
	}

	DropZoneView::DropZoneView()
		: mBackgroundView(View::create())
		, mDraggableViewsHolder(View::create())
		, mIsHighlighted(false)
	{
	}

	void DropZoneView::setup() {
		setHighlighted(false);

		ShapeViewRef rect = ShapeView::createRect(50, 50);

		rect->setFillColor(ci::Color(0.5, 0.5, 1.0))
			.setAlignment(Alignment::CENTER_CENTER);

		mBackgroundView->addChild(rect);

		addChild(mBackgroundView);
		addChild(mDraggableViewsHolder);
	}

	void DropZoneView::setHighlighted(bool highlighted) {
		if(highlighted) {
			mBackgroundView->setAlpha(1.0f);
		} else {
			mBackgroundView->setAlpha(0.5f);
		}

		mIsHighlighted = highlighted;
	}

	bool DropZoneView::addDraggableView(DraggableViewRef view) {
		if(mDraggableViewsHolder->hasChildren()) {
			return false;
		}

		setHighlighted(false);
		mDraggableViewsHolder->addChild(view);

		view->setScale(0.75f, 0.75f);
		view->setPosition(0.0f, 0.0f);

		return true;
	}

	bool DropZoneView::removeDraggableView(DraggableViewRef view) {
		return mDraggableViewsHolder->removeChild(view) != nullptr;
	}

	// View Controller
	DragAndDropViewControllerRef DragAndDropViewController::create() {
		DragAndDropViewControllerRef ref(new DragAndDropViewController());
		ref->setup();
		return ref;
	}

	DragAndDropViewController::DragAndDropViewController() {
	}

	void DragAndDropViewController::setup() {
	}

	void DragAndDropViewController::trackDragAndDropView(DraggableViewRef view) {
		if (std::find(mDraggableViews.begin(), mDraggableViews.end(), view) != mDraggableViews.end()) {
			//Already exists, throw exception or something?
			return;
		}

		mDraggableViews.push_back(view);

		mConnections += view->getSignalDragBegan().connect(std::bind(&DragAndDropViewController::viewDragBeganHandler, this, std::placeholders::_1));
		mConnections += view->getSignalDragged().connect(std::bind(&DragAndDropViewController::viewDraggedHandler, this, std::placeholders::_1));
		mConnections += view->getSignalDragEnded().connect(std::bind(&DragAndDropViewController::viewDragEndedHandler, this, std::placeholders::_1));
	}

	void DragAndDropViewController::trackDropZoneView(DropZoneViewRef view) {
		if (std::find(mDropZoneViews.begin(), mDropZoneViews.end(), view) != mDropZoneViews.end()) {
			//Already exists, throw exception or something?
			return;
		}

		mDropZoneViews.push_back(view);
	}

	bool DragAndDropViewController::checkForIntersection(ViewRef view1, ViewRef view2) {
		if (view1->hasParent() && view2->hasParent()) {
			ci::vec2 tl = view1->getParent()->localToWindow(view1->getFrame().getUpperLeft());
			ci::vec2 br = view1->getParent()->localToWindow(view1->getFrame().getLowerRight());

			//Flip rect coords if they are rotated upside down
			ci::Rectf view1GlobalRect = tl.y > br.y ? ci::Rectf(br, tl) : ci::Rectf(tl, br);

			tl = view2->getParent()->localToWindow(view2->getFrame().getUpperLeft());
			br = view2->getParent()->localToWindow(view2->getFrame().getLowerRight());

			//Flip rect coords if they are rotated upside down
			ci::Rectf view2GlobalRect = tl.y > br.y ? ci::Rectf(br, tl) : ci::Rectf(tl, br);

			if (tl.y > br.y) {
				view2GlobalRect = ci::Rectf(br, tl);
			}
			else {
				view2GlobalRect = ci::Rectf(tl, br);
			}

			//Check for intersection
			ci::Rectf intersection = view1GlobalRect.getClipBy(view2GlobalRect);

			return (intersection.getWidth() > 0 && intersection.getHeight() > 0);
		}

		return false;
	}

	void DragAndDropViewController::viewRemovedFromDropZone(DraggableViewRef view) {
		view->setScale(1.0f);
		view->setPosition(mView->windowToLocal(view->getParent()->localToWindow(view->getPosition())));
		mView->addChild(view);
	}

	void DragAndDropViewController::viewDragBeganHandler(DraggableViewRef &view) {
		// TESTING, FIX ZONE/CONTROLLER REMOVAL RELATIONSHIP
		if (view->getParent() != mView) {
			viewRemovedFromDropZone(view);
		}
	}

	void DragAndDropViewController::viewDraggedHandler(DraggableViewRef &view) {
		for (auto &dropZone : mDropZoneViews) {
			if (checkForIntersection(view, dropZone)) {
				dropZone->setHighlighted(true);
			}
			else {
				dropZone->setHighlighted(false);
			}
		}
	}

	void DragAndDropViewController::viewDragEndedHandler(DraggableViewRef &view) {
		for (auto &dropZone : mDropZoneViews) {
			if (checkForIntersection(view, dropZone)) {
				dropZone->addDraggableView(view);
				return;
			}
		}

		// Snap it back if needed
		if (view->getSnapsBackToPosition()) {
			view->snapBackToPosition();
		}
	}
}}