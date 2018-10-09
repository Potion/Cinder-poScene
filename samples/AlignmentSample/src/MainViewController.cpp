#include "MainViewController.h"

#include "cinder/app/App.h"

using namespace po::scene;

namespace sample {
	ViewControllerRef ViewController::create()
	{
		ViewControllerRef ref(new ViewController());
		return ref;
	}

	void ViewController::viewDidLoad()
	{
		//  Cinder method for key events
		ci::app::getWindow()->getSignalKeyDown().connect(std::bind(&ViewController::keyDown, this, std::placeholders::_1));

		createIndicators();

		ci::vec2 center(320, 240);

		//  create and add the shape
		mShapeView = ShapeView::createRect(200, 100);
		mShapeView->setAlignment(po::scene::Alignment::NONE)
			.setFillColor(122.f / 255, 201.f / 255, 67.4 / 255)
			.setPosition(center);
		getView()->addSubview(mShapeView);

		//  create and add a reference dot to indicate position of mShapeView
		ShapeViewRef dot = ShapeView::createCircle(10);
		dot->setAlignment(po::scene::Alignment::CENTER_CENTER)
			.setFillColor(255.f / 255, 123.f / 255, 172.f / 255)
			.setPosition(center);
		getView()->addSubview(dot);

		activateIndicator(0);
	}

	void ViewController::keyDown(ci::app::KeyEvent &event)
	{
		//  convert char to int
		int selectedInt = event.getChar() - '0';
		if (selectedInt < 0 || selectedInt > mIndicatorNames.size() - 1)
		{
			return;
		}

		activateIndicator(selectedInt);

		switch (event.getChar()) {
		case '0':
			mShapeView->setAlignment(po::scene::Alignment::NONE);
			break;

		case '1':
			mShapeView->setAlignment(po::scene::Alignment::TOP_LEFT);
			break;

		case '2':
			mShapeView->setAlignment(po::scene::Alignment::TOP_CENTER);
			break;

		case '3':
			mShapeView->setAlignment(po::scene::Alignment::TOP_RIGHT);
			break;

		case '4':
			mShapeView->setAlignment(po::scene::Alignment::CENTER_LEFT);
			break;

		case '5':
			mShapeView->setAlignment(po::scene::Alignment::CENTER_CENTER);
			break;

		case '6':
			mShapeView->setAlignment(po::scene::Alignment::CENTER_RIGHT);
			break;

		case '7':
			mShapeView->setAlignment(po::scene::Alignment::BOTTOM_LEFT);
			break;

		case '8':
			mShapeView->setAlignment(po::scene::Alignment::BOTTOM_CENTER);
			break;

		case '9':
			mShapeView->setAlignment(po::scene::Alignment::BOTTOM_RIGHT);
			break;

		default:
			break;
		}
	}

	void ViewController::createIndicators()
	{
		//  Indicator names same as Alignment type names
		mIndicatorNames = {
			"NONE",
			"TOP_LEFT",
			"TOP_CENTER",
			"TOP_RIGHT",
			"CENTER_LEFT",
			"CENTER_CENTER",
			"CENTER_RIGHT",
			"BOTTOM_LEFT",
			"BOTTOM_CENTER",
			"BOTTOM_RIGHT"
		};

		//  Create a container to hold the indicators
		mIndicatorContainer = View::create();
		getView()->addSubview(mIndicatorContainer);
		mIndicatorContainer->setPosition(10, 10);

		//  Create and add indicators to the container
		//  Add them to a map to reference later
		for (int i = 0; i < mIndicatorNames.size(); i++) {
			std::string indicatorText = std::to_string(i) + ": " + mIndicatorNames[i];
			IndicatorRef indicator = Indicator::create(indicatorText);
			mIndicatorContainer->addSubview(indicator);
			if (i == 0) {
				indicator->setPosition(indicator->getWidth() * 3, i * (indicator->getHeight() + 5));
			}
			else if (i < 4) {
				indicator->setPosition(0, (i - 1) * (indicator->getHeight() + 5));
			}
			else if (i < 7) {
				indicator->setPosition(indicator->getWidth(), (i - 4) * (indicator->getHeight() + 5));
			}
			else {
				indicator->setPosition(indicator->getWidth() * 2, (i - 7) * (indicator->getHeight() + 5));
			}

			mIndicators[mIndicatorNames[i]] = indicator;
		}
	}

	void ViewController::activateIndicator(int num)
	{
		for (int i = 0; i < mIndicatorNames.size(); i++) {
			if (i == num) {
				mIndicators[mIndicatorNames[i]]->showHighlighted();
			}
			else {
				mIndicators[mIndicatorNames[i]]->hideHighlighted();
			}
		}
	}
}
