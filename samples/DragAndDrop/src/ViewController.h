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

		po::scene::DragAndDropViewControllerRef mDragAndDropVC;
	};
}