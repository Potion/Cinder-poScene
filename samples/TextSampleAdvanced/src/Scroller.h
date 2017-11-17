#pragma once

#include "poScene/View.h"
#include "poScene/ShapeView.h"

using namespace po::scene;

class Scroller;
typedef std::shared_ptr<Scroller> ScrollerRef;

class Scroller
: public po::scene::View
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
	ShapeViewRef mThumb;
	
	//	Scroll track
	ShapeViewRef mTrack;
	
	//	Keep track of mouse position for dragging
	bool mIsPressed;
	ci::vec2 mStartPos, mEndPos;
	ci::vec2 mInitialPos;
	
	//	Mouse event handlers
	void onMouseDown(po::scene::MouseEvent &event);
	void onMouseDrag(po::scene::MouseEvent & event);
	void onMouseUp(po::scene::MouseEvent &event);
	
};