#pragma once

#include "poScene/ViewController.h"
#include "Square.h"

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
			//	Container to hold all the squares
			po::scene::ViewRef mContainer;

			//	Reference to previous square
			SquareRef mPreviousSquare;

	};
}
