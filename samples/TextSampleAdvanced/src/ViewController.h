#pragma once

#include "poScene/ViewController.h"

#include "TextComponent.h"
#include "Scroller.h"

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

		void update() override;

	private:
		//	Reference to the text component
		TextComponentRef mTextComponent;

		//	Reference to the scroller
		ScrollerRef mScroller;
	};
}
