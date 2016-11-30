#include "ViewController.h"

#include "cinder/app/App.h"

namespace sample {
	ViewControllerRef ViewController::create() {
		ViewControllerRef ref(new ViewController());
		ref->setup();
		return ref;
	}

	ViewController::ViewController() {
	}

	void ViewController::setup() {
		for(int i=0; i<5; i++) {
			po::scene::DropZoneViewRef dropZoneView = po::scene::DropZoneView::create();
			dropZoneView->setPosition(ci::app::getWindowWidth() * 0.75, 75 + i * 75);
			trackDropZoneView(dropZoneView);

			ci::vec2 pos(ci::app::getWindowWidth() * 0.25, 75 + i * 75);
			po::scene::DragAndDropViewRef dragView = po::scene::DragAndDropView::create(pos);
			trackDragAndDropView(dragView);
		}

		for(auto &view : mDropZoneViews) {
			mView->addChild(view);
		}

		for(auto &view : mDragAndDropViews) {
			mView->addChild(view);
		}
	}

	void ViewController::trackDragAndDropView(po::scene::DragAndDropViewRef view) {
		if(std::find(mDragAndDropViews.begin(), mDragAndDropViews.end(), view) != mDragAndDropViews.end()) {
			//Already exists, throw exception or something?
			return;
		}

		mDragAndDropViews.push_back(view);

		view->getSignalDragBegan().connect(std::bind(&ViewController::viewDragBeganHandler,	this, std::placeholders::_1));
		view->getSignalDragged().connect(std::bind(&ViewController::viewDraggedHandler,		this, std::placeholders::_1));
		view->getSignalDragEnded().connect(std::bind(&ViewController::viewDragEndedHandler,	this, std::placeholders::_1));
	}

	void ViewController::trackDropZoneView(po::scene::DropZoneViewRef view) {
		if(std::find(mDropZoneViews.begin(), mDropZoneViews.end(), view) != mDropZoneViews.end()) {
			//Already exists, throw exception or something?
			return;
		}

		mDropZoneViews.push_back(view);
	}

	bool ViewController::checkForIntersection(po::scene::ViewRef view1, po::scene::ViewRef view2) {
		if(view1->hasParent() && view2->hasParent()) {
			ci::vec2 tl = view1->getParent()->localToWindow(view1->getFrame().getUpperLeft());
			ci::vec2 br = view1->getParent()->localToWindow(view1->getFrame().getLowerRight());

			//Flip rect coords if they are rotated upside down
			ci::Rectf view1GlobalRect = tl.y > br.y ? ci::Rectf(br, tl) : ci::Rectf(tl, br);

			tl = view2->getParent()->localToWindow(view2->getFrame().getUpperLeft());
			br = view2->getParent()->localToWindow(view2->getFrame().getLowerRight());

			//Flip rect coords if they are rotated upside down
			ci::Rectf view2GlobalRect = tl.y > br.y ? ci::Rectf(br, tl) : ci::Rectf(tl, br);
	
			if(tl.y > br.y) {
				view2GlobalRect = ci::Rectf(br, tl);
			} else {
				view2GlobalRect = ci::Rectf(tl, br);
			}

			//Check for intersection
			ci::Rectf intersection = view1GlobalRect.getClipBy(view2GlobalRect);

			return (intersection.getWidth() > 0 && intersection.getHeight() > 0);
		}

		return false;
	}

	void ViewController::viewDragBeganHandler(po::scene::DraggableViewRef &view) {
		if(view->getParent() != mView) {
			view->setScale(1.0f);
			view->setPosition(mView->windowToLocal(view->getParent()->localToWindow(view->getPosition())));
			mView->addChild(view);
		}
	}

	void ViewController::viewDraggedHandler(po::scene::DraggableViewRef &view) {
		for(auto &dropZone : mDropZoneViews) {
			if(checkForIntersection(view, dropZone)) {
				dropZone->setHighlighted(true);
			} else {
				dropZone->setHighlighted(false);
			}
		}
	}

	void ViewController::viewDragEndedHandler(po::scene::DraggableViewRef &view) {
		for(auto &dropZone : mDropZoneViews) {
			if(checkForIntersection(view, dropZone)) {
				dropZone->addView(view);
				return;
			}
		}

		std::static_pointer_cast<po::scene::DragAndDropView>(view)->snapBackToPosition();
	}
}