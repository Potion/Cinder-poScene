#pragma once

#include "poScene/ViewController.h"
#include "poScene/ShapeView.h"
#include "poScene/DragAndDrop.h"

namespace sample {
	class ViewController;
	typedef std::shared_ptr<ViewController> ViewControllerRef;

	class ViewController : public po::scene::ViewController {
	public:
		static ViewControllerRef create();
		void onViewAddedToDragZone( po::scene::DropZoneViewRef dropRef, po::scene::DraggableViewRef dragRef );

	private:
		
		ViewController();
		void viewDidLoad() override;
		po::scene::DragAndDropViewControllerRef mDragAndDropVC;
	};
}
