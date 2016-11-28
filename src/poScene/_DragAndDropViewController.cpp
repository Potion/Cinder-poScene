#include "DragAndDropViewController.h"

namespace po { namespace scene{

	DragAndDropViewControllerRef DragAndDropViewController::create() {
		DragAndDropViewControllerRef ref(new DragAndDropViewController());
		ref->setup();
		return ref;
	}

	DragAndDropViewController::DragAndDropViewController() {
	}

	void DragAndDropViewController::setup() {
		mView->addChild(DraggableView::create());
	}
}}