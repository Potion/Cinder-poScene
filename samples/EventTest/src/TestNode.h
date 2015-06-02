//
//  TestNode.h
//  BasicScene
//
//  Created by Stephen Varga on 3/18/14.
//
//

#pragma once

#include "poNodeContainer.h"
#include "poShape.h"

class TestNode;
typedef std::shared_ptr<TestNode> TestNodeRef;



class TestNode
:   public po::scene::NodeContainer
{
public:
    static TestNodeRef create();
    
    void setup();
    
    void mouseDown(po::scene::MouseEvent &event);
    void mouseMove(po::scene::MouseEvent &event);
    void mouseDrag(po::scene::MouseEvent &event);
    void mouseUp(po::scene::MouseEvent &event);
    void mouseDownInside(po::scene::MouseEvent &event);
    void mouseMoveInside(po::scene::MouseEvent &event);
    void mouseDragInside(po::scene::MouseEvent &event);
    void mouseUpInside(po::scene::MouseEvent &event);
    
    void touchesBeganInside(po::scene::TouchEvent &event);
    void touchesMovedInside(po::scene::TouchEvent &event);
    void touchesEndedInside(po::scene::TouchEvent &event);
    
    void myMouseHandler(po::scene::MouseEvent &event);
    
protected:
    
private:
	static const int NUM_ROWS;
	static const int NUM_COLS;
	static const int SIZE;
	static const int SPACING;

    float p;
    
    void squareFinishedTweening(po::scene::ShapeRef square);
};
