#pragma once

#include "poScene/ViewController.h"
#include "poScene/ShapeView.h"
#include "poScene/ImageView.h"


namespace sample {
	class ViewController;
	typedef std::shared_ptr<ViewController> ViewControllerRef;

	class ViewController
		: public po::scene::ViewController
	{
	public:
		static ViewControllerRef create();

		void viewDidLoad() override;

	protected:
		ViewController();

	private:
		//	Image
		po::scene::ImageViewRef mImage;

		//	Mask shape
		po::scene::ShapeViewRef mMask;

		//	Mouse event handlers
		void onMouseMove(po::scene::MouseEvent &event);

	};
}
