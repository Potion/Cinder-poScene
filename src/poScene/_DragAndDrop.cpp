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
		, mDraggableViews(View::create())
		, mIsHighlighted(false)
	{
	}

	void DropZoneView::setup() {
		setHighlighted(false);

		po::scene::ShapeViewRef rect = po::scene::ShapeView::createRect(50, 50);

		rect->setFillColor(ci::Color(0.5, 0.5, 1.0))
			.setAlignment(po::scene::Alignment::CENTER_CENTER);

		mBackgroundView->addChild(rect);

		addChild(mBackgroundView);
		addChild(mDraggableViews);
	}

	void DropZoneView::setHighlighted(bool highlighted) {
		if(highlighted) {
			mBackgroundView->setAlpha(1.0f);
		} else {
			mBackgroundView->setAlpha(0.5f);
		}

		mIsHighlighted = highlighted;
	}

	bool DropZoneView::addView(DraggableViewRef view) {
		if(mDraggableViews->hasChildren()) {
			return false;
		}

		setHighlighted(false);
		mDraggableViews->addChild(view);

		view->setScale(0.75f, 0.75f);
		view->setPosition(0.0f, 0.0f);

		return true;
	}
}}

//
//namespace po { namespace scene{
//
//	DragAndDropViewControllerRef DragAndDropViewController::create() {
//		DragAndDropViewControllerRef ref(new DragAndDropViewController());
//		ref->setup();
//		return ref;
//	}
//
//	DragAndDropViewController::DragAndDropViewController() {
//	}
//
//	void DragAndDropViewController::setup() {
//	}
//
//	void DragAndDropViewController::reset() {
//		mSets.clear();
//	}
//
//	void DragAndDropViewController::trackDraggableView(DraggableViewRef view, int setID) {
//		if(mSets.count(setID) != 0) {
//			if(std::find(mSets[setID].draggableViews.begin(), mSets[setID].draggableViews.end(), view) != mSets[setID].draggableViews.end()) {
//				//Already exists, throw exception or something?
//				return;
//			}
//		}
//		
//		mSets[setID].draggableViews.push_back(view);
//		mSets[setID].connections[view].push_back(view->getSignalDragged().connect(std::bind(&DragAndDropViewController::viewBeganDraggingHandler, this, std::placeholders::_1)));
//	}
//
//	void DragAndDropViewController::trackDropZoneView(DropZoneViewRef view, int setID) {
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
//	void DragAndDropViewController::viewBeganDraggingHandler(DraggableViewRef &view) {
//
//	}
//
//	void DragAndDropViewController::viewDraggedHandler(DraggableViewRef &view) {
//
//	}
//
//	void DragAndDropViewController::viewFinishedDraggingHandler(DraggableViewRef &view) {
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