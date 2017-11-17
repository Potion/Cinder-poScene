#pragma once

#include "cinder/app/KeyEvent.h"

#include "poScene/ViewController.h"
#include "poScene/ShapeView.h"
#include "poScene/TextView.h"

#include "Indicator.h"

namespace sample {
	//  create a shared pointer type for the class
	typedef std::shared_ptr<class ViewController> ViewControllerRef;

	//  define the class
	class ViewController
		: public po::scene::ViewController
	{
	public:
		static ViewControllerRef create();
		void viewDidLoad() override;

	protected:

	private:
		ViewController() {};

		po::scene::ShapeViewRef  mShapeView;
		po::scene::TextViewRef   mTextBottom;

		//  Container to hold the indicators
		po::scene::ViewRef mIndicatorContainer;

		//  Alignment types mapped to indicators
		std::vector<std::string>                mIndicatorNames;
		std::map<std::string, IndicatorRef>     mIndicators;


		void keyDown(ci::app::KeyEvent &event);

		void createIndicators();
		void activateIndicator(int num);
	};
}
