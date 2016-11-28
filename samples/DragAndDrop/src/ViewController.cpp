#include "ViewController.h"

namespace sample {
	ViewControllerRef ViewController::create() {
		ViewControllerRef ref(new ViewController());
		ref->setup();
		return ref;
	}

	ViewController::ViewController()
	: mDragAndDropVC(po::scene::DragAndDropViewController::create()) {
	}

	void ViewController::setup() {
		mView->addChild(mDragAndDropVC->getView());
	}
}