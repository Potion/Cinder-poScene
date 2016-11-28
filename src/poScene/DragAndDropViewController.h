#pragma once

#include "poScene/ViewController.h"
#include "poScene/DraggableView.h"

namespace po { namespace scene {
	typedef std::shared_ptr<class DragAndDropViewController> DragAndDropViewControllerRef;

	class DragAndDropViewController : public ViewController {
	public:
		static DragAndDropViewControllerRef create();

		void setup();

		void addDraggableView(DraggableViewRef view);

	private:
		DragAndDropViewController();

		std::vector<DraggableViewRef> mDraggableViews;
	};
}}