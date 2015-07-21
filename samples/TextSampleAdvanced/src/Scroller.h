#pragma once

#include "poNodeContainer.h"
#include "poShape.h"

using namespace po::scene;

class Scroller;
typedef std::shared_ptr<Scroller> ScrollerRef;

class Scroller
: public po::scene::NodeContainer
{
public:
	static ScrollerRef create();
	virtual ~Scroller();

	virtual void setup();
	
	//	Get the thumb position for text positioning
	ci::vec2 getThumbPosition() { return mThumb->getPosition(); }

protected:
	Scroller();

private:
	//	Scroll thumb
	ShapeRef mThumb;
	
	//	Scroll track
	ShapeRef mTrack;
	
	//	Keep track of mouse position for dragging
	bool mIsPressed;
	ci::vec2 mStartPos, mEndPos;
	ci::vec2 mInitialPos;
	
	//	Mouse event handlers
	void onMouseDown(MouseEvent &event);
	void onMouseDrag(MouseEvent & event);
	void onMouseUp(MouseEvent &event);
	
};