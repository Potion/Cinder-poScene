#pragma once

#include "poScene/ViewController.h"
#include "poScene/TextView.h"
#include "poScene/ShapeView.h"

namespace sample
{
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
			//	Scaling text box
			po::scene::TextViewRef mScalingText;

			//	Scale text up and down in a loop
			void animateScale();
	};
}
