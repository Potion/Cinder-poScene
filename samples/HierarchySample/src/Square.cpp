#include "Square.h"

SquareRef Square::create(int size)
{
	SquareRef ref = std::shared_ptr<Square>(new Square());
	ref->setup(size);
	return ref;
}

Square::Square()
: mSize(50)
, mColor(ci::Color(ci::CM_HSV, 1, 1, 0.8))
, mHighlightColor(ci::Color(ci::CM_HSV, 1, 1, 1))
, mIsPressed(false)
, mStartPos(ci::vec2())
, mEndPos(ci::vec2())
, mInitialPos(ci::vec2())
, mBoundsColor(ci::Color(1.0f, 1.0f, 1.0f))
{
}

Square::~Square() 
{
}

void Square::setup(int size)
{
	mSize = size;
	
	//	The square shape
	mShape = Shape::createRect(mSize, mSize);
	mShape->setAlignment(po::scene::Alignment::CENTER_CENTER);
	
	//	Change hue based on size
	float hue = ci::lmap<float>(mSize, 0, 300, 0.1, 0.3);
	mColor = ci::Color(ci::CM_HSV, 1 - hue, 1, 0.8);
	mHighlightColor = ci::Color(ci::CM_HSV, 1 - hue, 1, 1);
	
	mShape->setFillColor(mColor);
	addChild(mShape);
	
	//	Connect to mouse events
	getSignal(po::scene::MouseEvent::DOWN_INSIDE).connect(std::bind(&Square::onMouseDown, this, std::placeholders::_1));
	getSignal(po::scene::MouseEvent::DRAG).connect(std::bind(&Square::onMouseDragged, this, std::placeholders::_1));
	getSignal(po::scene::MouseEvent::UP_INSIDE).connect(std::bind(&Square::onMouseUp, this, std::placeholders::_1));
	getSignal(po::scene::MouseEvent::UP).connect(std::bind(&Square::onMouseUp, this, std::placeholders::_1));
}

void Square::onMouseDown(po::scene::MouseEvent &event)
{
	if (!mIsPressed) {
		mIsPressed = true;
		
		//	Set the initial, start and end positions to the window position in the parent
		mInitialPos = getPosition();
		mStartPos = getParent()->windowToLocal(event.getWindowPos());
		mEndPos = getParent()->windowToLocal(event.getWindowPos());
		
		highlight(true);
		highlightChildren(true);
	}
}

void Square::onMouseDragged(po::scene::MouseEvent &event)
{
	if (mIsPressed) {
		//	Update the end position
		mEndPos = getParent()->windowToLocal(event.getWindowPos());
		
		//	Set the nodes position
		ci::vec2 newPosition = mInitialPos + (mEndPos - mStartPos);
		setPosition(newPosition);
	}
}

void Square::onMouseUp(po::scene::MouseEvent &event)
{
	if (mIsPressed) {
		mIsPressed = false;
		
		highlight(false);
		highlightChildren(false);
	}
}

void Square::highlight(bool isHighlighted)
{
	if (isHighlighted) {
		mShape->setFillColor(mHighlightColor);
	} else {
		mShape->setFillColor(mColor);
	}
}

void Square::highlightChildren(bool isHighlighted)
{
	//	Get all the nodes children
	std::deque<NodeRef> children = getChildren();
	
	if (isHighlighted) {
		for (auto &child : children) {
			child->setBoundsColor(mBoundsColor);
			child->setDrawBounds(true);
		}
	} else {
		for (auto &child : children) {
			child->setDrawBounds(false);
		}
	}
}