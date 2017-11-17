#pragma once

#include "poScene/ViewController.h"
#include "AnimationSquare.h"

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

	private:
		AnimationSquareRef mPositionSquare;
		AnimationSquareRef mScaleSquare;
		AnimationSquareRef mRotationSquare;
		AnimationSquareRef mAlphaSquare;
		AnimationSquareRef mOffsetSquare;
		AnimationSquareRef mColorSquare;
	};
}
