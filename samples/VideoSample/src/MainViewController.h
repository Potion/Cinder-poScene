#pragma once

#include "poScene/ViewController.h"
#include "poScene/VideoView.h"

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
			po::scene::VideoViewGlRef   mVideo;
			void                        spinPlayer();
			void                        resetPlayerRotation();
	};
}
