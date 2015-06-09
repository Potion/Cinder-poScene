#include "AlignmentSample.h"
#include "poShape.h"

using namespace po::scene;

AlignmentSampleRef AlignmentSample::create() 
{
    AlignmentSampleRef node(new AlignmentSample());
    node->setup();
    return node;
}


void AlignmentSample::setup() 
{
    //  draw debugging bounds around the entire node
    //  these default to red
    setDrawBounds(true);
    
    //  create and add the blue shape
    ShapeRef shapeBlue = Shape::createRect(100, 100);
    shapeBlue->setAlignment(Alignment::NONE);
    shapeBlue->setFillColor(63.f/255, 169.f/255, 245.f/255);
    // default position is (0, 0)
    addChild(shapeBlue);
    
    //  create and add the orange shape
    ShapeRef shapeOrange = Shape::createRect(150, 150);
    shapeOrange->setAlignment(Alignment::TOP_LEFT);
    shapeOrange->setFillColor(255.f/255, 147.f/255, 30.f/255);
    shapeOrange->setPosition(100, 100);
    addChild(shapeOrange);
    
    //  create and add the green shape
    ShapeRef shapeGreen = Shape::createRect(100, 100);
    shapeGreen->setAlignment(Alignment::CENTER_CENTER);
    shapeGreen->setFillColor(122.f/255, 201.f/255, 67.f/255);
    shapeGreen->setPosition(175, 175);
    addChild(shapeGreen);
    
    //  create and add the gray shape
    ShapeRef shapeGray = Shape::createRect(100, 100);
    shapeGray->setAlignment(Alignment::TOP_CENTER);
    shapeGray->setFillColor(189.f/255, 204.f/255, 212.f/255);
    shapeGray->setPosition(175, 250);
    addChild(shapeGray);
    
    //  create and add the pink shape
    ShapeRef shapePink = Shape::createRect(100, 100);
    shapePink->setAlignment(Alignment::BOTTOM_LEFT);
    shapePink->setFillColor(255.f/255, 123.f/255, 172.f/255);
    shapePink->setPosition(250, 200);
    addChild(shapePink);
    
    
    //  Add a reference dot at the position of each shape node
    
    //  grab all children added so far
    std::deque<NodeRef> children = getChildren();
    
    //  add a dot at the same position of each child
    //  each dot will be the same color, but darker, as its reference shape
    for (auto &child : children) {
        ShapeRef referenceDot = Shape::createCircle(10);
        referenceDot->setAlignment(Alignment::CENTER_CENTER);
        referenceDot->setPosition(child->getPosition());
        ci::Color dotColor = child->getFillColor();
        dotColor *= 0.7f;
        referenceDot->setFillColor(dotColor);
        addChild(referenceDot);
    }
    
    //  move the entire node to the right and down
    setPosition(100, 100);
    
    std::cout << "size of main node: " << getWidth() << ", " << getHeight() << std::endl;
}
