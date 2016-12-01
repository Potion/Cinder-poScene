#pragma once

#include "poScene/ViewController.h"

#include "poScene/DragAndDrop.h"

namespace sample {
	class ViewController;
	typedef std::shared_ptr<ViewController> ViewControllerRef;

	class ViewController : public po::scene::ViewController {
	public:
		static ViewControllerRef create();

	private:
		ViewController();

		void setup();

		bool checkForIntersection(po::scene::ViewRef view1, po::scene::ViewRef view2);

		void trackDragAndDropView(po::scene::DragAndDropViewRef view);
		void trackDropZoneView(po::scene::DropZoneViewRef view);

		std::vector<po::scene::DragAndDropViewRef> mDragAndDropViews;
		std::vector<po::scene::DropZoneViewRef> mDropZoneViews;

		void storeConnection(po::scene::ViewRef view, ci::signals::Connection &connection);

		// Event Handlers
		void viewDragBeganHandler(po::scene::DraggableViewRef &view);
		void viewDraggedHandler(po::scene::DraggableViewRef &view);
		void viewDragEndedHandler(po::scene::DraggableViewRef &view);


	};
}