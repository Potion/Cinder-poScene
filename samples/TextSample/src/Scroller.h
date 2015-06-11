#pragma once

/*	Created by bruce on 6/11/15.
 *	Copyright 2015 __MyCompanyName__. All rights reserved.
 */

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

protected:
	Scroller();

private:
	ShapeRef mThumb;
	
	bool mIsPressed;
	ci::Vec2f mStartPos, mEndPos;
	
	void onMouseDown(MouseEvent &event);
	void onMouseDrag(MouseEvent & event);
	void onMouseUp(MouseEvent &event);
	
};