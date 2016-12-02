#include "ViewController.h"

#include "cinder/app/App.h"

namespace sample {
	ViewControllerRef ViewController::create() {
		ViewControllerRef ref(new ViewController());
		ref->setup();
		return ref;
	}

	ViewController::ViewController()
	{
	}

	void ViewController::setup() {
		mDragAndDropVC = po::scene::DragAndDropViewController::create();

		for(int i=0; i<5; i++) {
			po::scene::DropZoneViewRef dropZoneView = po::scene::DropZoneView::create();
			dropZoneView->setPosition(ci::app::getWindowWidth() * 0.75, 75 + i * 75);
			mDragAndDropVC->trackDropZoneView(dropZoneView);

			ci::vec2 pos(ci::app::getWindowWidth() * 0.25, 75 + i * 75);
			po::scene::DragAndDropViewRef dragView = po::scene::DragAndDropView::create(pos);
			mDragAndDropVC->trackDragAndDropView(dragView);
		}

		for(auto &view : mDragAndDropVC->getDropZoneViews()) {
			mDragAndDropVC->getView()->addChild(view);
		}

		for(auto &view : mDragAndDropVC->getDragAndDropViews()) {
			mDragAndDropVC->getView()->addChild(view);
		}

		mView->addChild(mDragAndDropVC->getView());
	}
}