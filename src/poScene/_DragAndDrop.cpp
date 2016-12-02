#include "DragAndDrop.h"

#include "poScene/ShapeView.h"

namespace po { namespace scene {
	// Drag And Drop View
	DragAndDropViewRef DragAndDropView::create() {
		DragAndDropViewRef ref(new DragAndDropView());
		ref->setup();
		return ref;
	}

	DragAndDropViewRef DragAndDropView::create(ci::vec2 snapBackPosition) {
		DragAndDropViewRef ref(new DragAndDropView(snapBackPosition));
		ref->setup();
		return ref;
	}

	DragAndDropView::DragAndDropView()
		: mSnapsBack(false)
	{
	}

	DragAndDropView::DragAndDropView(ci::vec2 snapBackPosition)
	: mSnapsBack(true)
	, mSnapPosition(snapBackPosition) {
		setPosition(snapBackPosition);
	}

	void DragAndDropView::snapBackToPosition() {
		if(mSnapsBack) {
			setPosition(mSnapPosition);
		}
	}

	// Drop Zone View
	DropZoneViewRef DropZoneView::create() {
		DropZoneViewRef ref(new DropZoneView());
		ref->setup();
		return ref;
	}

	DropZoneView::DropZoneView()
		: mBackgroundView(View::create())
		, mDragAndDropViewsHolder(View::create())
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
		addChild(mDragAndDropViewsHolder);
	}

	void DropZoneView::setHighlighted(bool highlighted) {
		if(highlighted) {
			mBackgroundView->setAlpha(1.0f);
		} else {
			mBackgroundView->setAlpha(0.5f);
		}

		mIsHighlighted = highlighted;
	}

	bool DropZoneView::addDragAndDropView(DragAndDropViewRef view) {
		if(mDragAndDropViewsHolder->hasChildren()) {
			return false;
		}

		setHighlighted(false);
		mDragAndDropViewsHolder->addChild(view);

		view->setScale(0.75f, 0.75f);
		view->setPosition(0.0f, 0.0f);

		return true;
	}

	bool DropZoneView::removeDragAndDropView(DragAndDropViewRef view) {
		return mDragAndDropViewsHolder->removeChild(view) != nullptr;
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

	void DragAndDropViewController::trackDragAndDropView(DragAndDropViewRef view) {
		if (std::find(mDragAndDropViews.begin(), mDragAndDropViews.end(), view) != mDragAndDropViews.end()) {
			//Already exists, throw exception or something?
			return;
		}

		mDragAndDropViews.push_back(view);

		view->getSignalDragBegan().connect(std::bind(&DragAndDropViewController::viewDragBeganHandler, this, std::placeholders::_1));
		view->getSignalDragged().connect(std::bind(&DragAndDropViewController::viewDraggedHandler, this, std::placeholders::_1));
		view->getSignalDragEnded().connect(std::bind(&DragAndDropViewController::viewDragEndedHandler, this, std::placeholders::_1));
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

	void DragAndDropViewController::viewRemovedFromDropZone(DragAndDropViewRef view) {
		view->setScale(1.0f);
		view->setPosition(mView->windowToLocal(view->getParent()->localToWindow(view->getPosition())));
		mView->addChild(view);
	}

	void DragAndDropViewController::viewDragBeganHandler(DraggableViewRef &view) {
		DragAndDropViewRef dragAndDropView = std::static_pointer_cast<DragAndDropView>(view);
		//DropZoneViewRef dropZone = std::dynamic_pointer_cast<DropZoneView>(view->getParent());

		//if ( dropZone != nullptr) {
		//	dropZone->removeDragAndDropView(dragAndDropView);
		//	viewRemovedFromDropZone(dragAndDropView);
		//}

		// TESTING, FIX ZONE/CONTROLLER REMOVAL RELATIONSHIP
		if (view->getParent() != mView) {
			viewRemovedFromDropZone(dragAndDropView);
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
				DragAndDropViewRef dragAndDropView = std::static_pointer_cast<DragAndDropView>(view);
				dropZone->addDragAndDropView(dragAndDropView);
				return;
			}
		}

		std::static_pointer_cast<DragAndDropView>(view)->snapBackToPosition();
	}
}}

//
//namespace po { namespace scene{
//
//	DragAndDropViewControllerRef DragAndDropDragAndDropViewController::create() {
//		DragAndDropViewControllerRef ref(new DragAndDropViewController());
//		ref->setup();
//		return ref;
//	}
//
//	DragAndDropDragAndDropViewController::DragAndDropViewController() {
//	}
//
//	void DragAndDropDragAndDropViewController::setup() {
//	}
//
//	void DragAndDropDragAndDropViewController::reset() {
//		mSets.clear();
//	}
//
//	void DragAndDropDragAndDropViewController::trackDraggableView(DraggableViewRef view, int setID) {
//		if(mSets.count(setID) != 0) {
//			if(std::find(mSets[setID].draggableViews.begin(), mSets[setID].draggableViews.end(), view) != mSets[setID].draggableViews.end()) {
//				//Already exists, throw exception or something?
//				return;
//			}
//		}
//		
//		mSets[setID].draggableViews.push_back(view);
//		mSets[setID].connections[view].push_back(view->getSignalDragged().connect(std::bind(&DragAndDropDragAndDropViewController::viewBeganDraggingHandler, this, std::placeholders::_1)));
//	}
//
//	void DragAndDropDragAndDropViewController::trackDropZoneView(DropZoneViewRef view, int setID) {
//		if(mSets.count(setID) != 0) {
//			if(std::find(mSets[setID].dropZoneViews.begin(), mSets[setID].dropZoneViews.end(), view) != mSets[setID].dropZoneViews.end()) {
//				//Already exists, throw exception or something?
//				return;
//			}
//		}
//
//		mSets[setID].dropZoneViews.push_back(view);
//	}
//
//	void DragAndDropDragAndDropViewController::viewBeganDraggingHandler(DraggableViewRef &view) {
//
//	}
//
//	void DragAndDropDragAndDropViewController::viewDraggedHandler(DraggableViewRef &view) {
//
//	}
//
//	void DragAndDropDragAndDropViewController::viewFinishedDraggingHandler(DraggableViewRef &view) {
//
//	}
//
//	DropZoneView::DropZoneView() {
//		
//	}
//
//	void DropZoneView::setup() {
//		
//	}
//}}