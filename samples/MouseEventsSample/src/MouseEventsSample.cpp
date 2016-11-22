#include "MouseEventsSample.h"

#include "cinder/app/App.h"

#include "Square.h"

MouseEventsSampleRef MouseEventsSample::create() 
{
    MouseEventsSampleRef node(new MouseEventsSample());
    node->setup();
    return node;
}


void MouseEventsSample::setup() 
{
	createIndicators();
	createSquares();
	
	//	Connect to mouse events
	getSignal(MouseEvent::DOWN).connect(std::bind(&MouseEventsSample::onMouseEvent, this, std::placeholders::_1));
	getSignal(MouseEvent::MOVE).connect(std::bind(&MouseEventsSample::onMouseEvent, this, std::placeholders::_1));
	getSignal(MouseEvent::DRAG).connect(std::bind(&MouseEventsSample::onMouseEvent, this, std::placeholders::_1));
	getSignal(MouseEvent::UP).connect(std::bind(&MouseEventsSample::onMouseEvent, this, std::placeholders::_1));
	getSignal(MouseEvent::WHEEL).connect(std::bind(&MouseEventsSample::onMouseEvent, this, std::placeholders::_1));
}

void MouseEventsSample::createIndicators()
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
	addChild(mIndicatorContainer);
	mIndicatorContainer->setPosition(30, 30);
	
	//	Create and add indicators to the container
	//	Add them to a map to reference later
	for (int i = 0; i < mIndicatorNames.size(); i++) {
		IndicatorRef indicator = Indicator::create(mIndicatorNames[i]);
		mIndicatorContainer->addChild(indicator);
		
		if (i < 5) {
			indicator->setPosition(0, i * (indicator->getHeight() + 5));
		} else {
			indicator->setPosition(indicator->getWidth() + 2, (i - 5) * (indicator->getHeight() + 5));
		}
		
		mIndicators[mIndicatorNames[i]] = indicator;
	}
}

void MouseEventsSample::createSquares()
{
	// Create a container for the squares
	mSquareContainer = View::create();
	addChild(mSquareContainer);
	
	//	Create and add the squares to the container
	for (int i = 0; i < 5; i++) {
		SquareRef square = Square::create();
		mSquareContainer->addChild(square);
		square->setPosition(i * (square->getWidth() + 20), 0);
		
		//	Connect to mouse events on the square
		square->getSignal(MouseEvent::DOWN_INSIDE).connect(std::bind(&MouseEventsSample::onSquareMouseEvent, this, std::placeholders::_1, square));
		square->getSignal(MouseEvent::MOVE_INSIDE).connect(std::bind(&MouseEventsSample::onSquareMouseEvent, this, std::placeholders::_1, square));
		square->getSignal(MouseEvent::DRAG_INSIDE).connect(std::bind(&MouseEventsSample::onSquareMouseEvent, this, std::placeholders::_1, square));
		square->getSignal(MouseEvent::UP_INSIDE).connect(std::bind(&MouseEventsSample::onSquareMouseEvent, this, std::placeholders::_1, square));
	}
	
	mSquareContainer->setPosition(ci::app::getWindowWidth() / 2 - mSquareContainer->getWidth() / 2 + 50, ci::app::getWindowHeight() / 2 - mSquareContainer->getHeight() / 2 + 50);
}

void MouseEventsSample::onSquareMouseEvent(po::scene::MouseEvent &event, SquareRef square)
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

void MouseEventsSample::onMouseEvent(po::scene::MouseEvent &event)
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

void MouseEventsSample::showIndicator(std::string name)
{
	//	Look up the indicator by name and highlight it
	if (mIndicators.find(name) != mIndicators.end()) {
		mIndicators[name]->showHighlighted();
	}
}