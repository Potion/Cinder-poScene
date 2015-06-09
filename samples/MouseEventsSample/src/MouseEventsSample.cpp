#include "MouseEventsSample.h"
#include "Square.h"

MouseEventsSampleRef MouseEventsSample::create() 
{
    MouseEventsSampleRef node(new MouseEventsSample());
    node->setup();
    return node;
}


void MouseEventsSample::setup() 
{
	mIndicatorNames = {
		"DOWN",
		"DOWN_INSIDE",
		"MOVE",
		"MOVE_INSIDE",
		"DRAG",
		"DRAG_INSIDE",
		"UP",
		"UP_INSIDE",
		"WHEEL"
	};
	
	mIndicatorContainer = NodeContainer::create();
	addChild(mIndicatorContainer);
	
	for (int i = 0; i < mIndicatorNames.size() - 1; i++) {
		IndicatorRef indicator = Indicator::create(mIndicatorNames[i]);
		mIndicatorContainer->addChild(indicator);
		indicator->setPosition(0, i * indicator->getHeight());
		mIndicators[mIndicatorNames[i]] = indicator;
	}
	
	mSquareContainer = NodeContainer::create();
	addChild(mSquareContainer);
	
	//	Add the squares to the canvas
	for (int i = 0; i < 5; i++) {
		SquareRef square = Square::create();
		mSquareContainer->addChild(square);
		square->setPosition(i * (square->getWidth() + 20), 0);
		
		square->getSignal(MouseEvent::DOWN_INSIDE).connect(std::bind(&MouseEventsSample::onSquareMouseEvent, this, std::placeholders::_1, square));
		square->getSignal(MouseEvent::MOVE_INSIDE).connect(std::bind(&MouseEventsSample::onSquareMouseEvent, this, std::placeholders::_1, square));
		square->getSignal(MouseEvent::DRAG_INSIDE).connect(std::bind(&MouseEventsSample::onSquareMouseEvent, this, std::placeholders::_1, square));
		square->getSignal(MouseEvent::UP_INSIDE).connect(std::bind(&MouseEventsSample::onSquareMouseEvent, this, std::placeholders::_1, square));
	}
	
	mSquareContainer->setPosition(ci::app::getWindowWidth() / 2 - mSquareContainer->getWidth() / 2, ci::app::getWindowHeight() / 2 - mSquareContainer->getHeight() / 2);
	
	getSignal(MouseEvent::DOWN).connect(std::bind(&MouseEventsSample::onMouseEvent, this, std::placeholders::_1));
	getSignal(MouseEvent::MOVE).connect(std::bind(&MouseEventsSample::onMouseEvent, this, std::placeholders::_1));
	getSignal(MouseEvent::DRAG).connect(std::bind(&MouseEventsSample::onMouseEvent, this, std::placeholders::_1));
	getSignal(MouseEvent::UP).connect(std::bind(&MouseEventsSample::onMouseEvent, this, std::placeholders::_1));
}

void MouseEventsSample::onSquareMouseEvent(po::scene::MouseEvent &event, SquareRef square)
{
	switch (event.getType()) {
		case po::scene::MouseEvent::MOVE_INSIDE:
			showIndicator("MOVE_INSIDE");
			break;
		case po::scene::MouseEvent::DOWN_INSIDE:
			showIndicator("DOWN_INSIDE");
			square->setIsDown(true);
			break;
		case po::scene::MouseEvent::UP_INSIDE:
			showIndicator("UP_INSIDE");
			square->setIsDown(false);
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
	switch (event.getType()) {
		case po::scene::MouseEvent::DOWN:
			break;
		default:
			break;
	}
}

void MouseEventsSample::showIndicator(std::string name)
{
	for (auto &item : mIndicators) {
		item.second->setIsHighlighted(false);
	}
	
	if (mIndicators.find(name) != mIndicators.end()) {
		mIndicators[name]->setIsHighlighted(true);
	}
}