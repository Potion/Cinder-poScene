#include "ViewController.h"

#include "cinder/app/App.h"

#include "Square.h"

using namespace po::scene;

namespace sample {

	ViewControllerRef ViewController::create()
	{
		ViewControllerRef ref(new ViewController());
		ref->setup();
		return ref;
	}


	void ViewController::setup()
	{
		createIndicators();
		createSquares();

		//	Connect to mouse events
		getView()->getSignal(MouseEvent::DOWN).connect(std::bind(&ViewController::onMouseEvent, this, std::placeholders::_1));
		getView()->getSignal(MouseEvent::MOVE).connect(std::bind(&ViewController::onMouseEvent, this, std::placeholders::_1));
		getView()->getSignal(MouseEvent::DRAG).connect(std::bind(&ViewController::onMouseEvent, this, std::placeholders::_1));
		getView()->getSignal(MouseEvent::UP).connect(std::bind(&ViewController::onMouseEvent, this, std::placeholders::_1));
		getView()->getSignal(MouseEvent::WHEEL).connect(std::bind(&ViewController::onMouseEvent, this, std::placeholders::_1));
	}

	void ViewController::createIndicators()
	{
		//	Indicator names same as event names
		mIndicatorNames = {
			"DOWN",
			"MOVE",
			"DRAG",
			"UP",
			"WHEEL",
			"DOWN_INSIDE",
			"MOVE_INSIDE",
			"DRAG_INSIDE",
			"UP_INSIDE"
		};

		//	Create a container to hold the indicators
		mIndicatorContainer = View::create();
		getView()->addSubview(mIndicatorContainer);
		mIndicatorContainer->setPosition(30, 30);

		//	Create and add indicators to the container
		//	Add them to a map to reference later
		for (int i = 0; i < mIndicatorNames.size(); i++) {
			IndicatorRef indicator = Indicator::create(mIndicatorNames[i]);
			mIndicatorContainer->addSubview(indicator);

			if (i < 5) {
				indicator->setPosition(0, i * (indicator->getHeight() + 5));
			}
			else {
				indicator->setPosition(indicator->getWidth() + 2, (i - 5) * (indicator->getHeight() + 5));
			}

			mIndicators[mIndicatorNames[i]] = indicator;
		}
	}

	void ViewController::createSquares()
	{
		// Create a container for the squares
		mSquareContainer = View::create();
		getView()->addSubview(mSquareContainer);

		//	Create and add the squares to the container
		for (int i = 0; i < 5; i++) {
			SquareRef square = Square::create();
			mSquareContainer->addSubview(square);
			square->setPosition(i * (square->getWidth() + 20), 0);

			//	Connect to mouse events on the square
			square->getSignal(MouseEvent::DOWN_INSIDE).connect(std::bind(&ViewController::onSquareMouseEvent, this, std::placeholders::_1, square));
			square->getSignal(MouseEvent::MOVE_INSIDE).connect(std::bind(&ViewController::onSquareMouseEvent, this, std::placeholders::_1, square));
			square->getSignal(MouseEvent::DRAG_INSIDE).connect(std::bind(&ViewController::onSquareMouseEvent, this, std::placeholders::_1, square));
			square->getSignal(MouseEvent::UP_INSIDE).connect(std::bind(&ViewController::onSquareMouseEvent, this, std::placeholders::_1, square));
		}

		mSquareContainer->setPosition(ci::app::getWindowWidth() / 2 - mSquareContainer->getWidth() / 2 + 50, ci::app::getWindowHeight() / 2 - mSquareContainer->getHeight() / 2 + 50);
	}

	void ViewController::onSquareMouseEvent(po::scene::MouseEvent &event, SquareRef square)
	{
		//	Show the indicator for the event
		switch (event.getType()) {
		case po::scene::MouseEvent::MOVE_INSIDE:
			showIndicator("MOVE_INSIDE");
			break;
		case po::scene::MouseEvent::DOWN_INSIDE:
			showIndicator("DOWN_INSIDE");
			break;
		case po::scene::MouseEvent::UP_INSIDE:
			showIndicator("UP_INSIDE");
			break;
		case po::scene::MouseEvent::DRAG_INSIDE:
			showIndicator("DRAG_INSIDE");
			break;
		default:
			break;
		}
	}

	void ViewController::onMouseEvent(po::scene::MouseEvent &event)
	{
		//	Show the indicator for the event
		switch (event.getType()) {
		case po::scene::MouseEvent::DOWN:
			showIndicator("DOWN");
			break;
		case po::scene::MouseEvent::MOVE:
			showIndicator("MOVE");
			break;
		case po::scene::MouseEvent::DRAG:
			showIndicator("DRAG");
			break;
		case po::scene::MouseEvent::UP:
			showIndicator("UP");
			break;
		case po::scene::MouseEvent::WHEEL:
			showIndicator("WHEEL");
			break;
		default:
			break;
		}
	}

	void ViewController::showIndicator(std::string name)
	{
		//	Look up the indicator by name and highlight it
		if (mIndicators.find(name) != mIndicators.end()) {
			mIndicators[name]->showHighlighted();
		}
	}
}