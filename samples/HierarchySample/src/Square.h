#pragma once

#include "poNodeContainer.h"
#include "poShape.h"

using namespace po::scene;

class Square;
typedef std::shared_ptr<Square> SquareRef;

class Square
: public po::scene::NodeContainer
{
public:
	static SquareRef create(int size);
	virtual ~Square();

	virtual void setup(int size);

protected:
	Square();

private:
	//	Size of the square
	int mSize;
	
	//	Square shape
	ShapeRef mShape;
	
	//	Shape color and highlight color
	ci::Color mColor;
	ci::Color mHighlightColor;
	
	//	Keep track on pressed for dragging
	bool mIsPressed;
	
	//	Keep track of the start and end position to use for dragging
	ci::Vec2f mStartPos, mEndPos;
	
	//	Mouse down event handler
	void onMouseDown(MouseEvent &event);
	
	//	Mouse dragged event handler
	void onMouseDragged(MouseEvent &event);
	
	//	Mouse up event handler
	void onMouseUp(MouseEvent &event);
	
	//	Highlight the square
	void highlight(bool isHighlighted);
	
	//	Highlight child nodes
	void highlightChildren(bool isHighlighted);
	
	//	Highlight the parent node
	void highlightParent();
 
};