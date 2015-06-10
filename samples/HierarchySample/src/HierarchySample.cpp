#include "HierarchySample.h"

#include "poShape.h"
#include "SpiralGenerator.h"

HierarchySampleRef HierarchySample::create() 
{
    HierarchySampleRef node(new HierarchySample());
    node->setup();
    return node;
}


void HierarchySample::setup() 
{
	int numSquares = 60;
	float maxSize = 60.f;
	
	NodeContainerRef rootNode = NodeContainer::create();
	addChild(rootNode);
	
	NodeContainerRef previousNode = rootNode;
	NodeContainerRef currentNode = rootNode;
	
	SpiralGenerator::SpiralProperties properties;
	properties.maxItems = numSquares;
	properties.slope = 1;
	properties.expansion = 30;
	properties.r_min = 0.3f;
	properties.r_max = 6.f;
	properties.spiral_type = 9;
	std::vector<SpiralGenerator::SpiralItemProperties> points = SpiralGenerator::getSpiralItemProperties(properties);
	
	for (int i = 5; i < numSquares; i++) {
		previousNode = currentNode;
		currentNode = NodeContainer::create();
		
		int size = (maxSize * (1 - points[i].scale.x));
		float speed = (maxSize - size) * 2;
		
		ci::app::console() << points[i].scale << ", " << size << ", " << speed << std::endl;
		
		ShapeRef shape = Shape::createRect(size, size);
		shape->setAlignment(po::scene::Alignment::CENTER_CENTER);
		shape->setRotation(ci::toDegrees(points[i].angle));
		shape->setPosition(points[i].position);
		shape->setFillColor(ci::Color(150, 0, 0));
		shape->setDrawBounds(true);
		shape->setAlpha(0.5f);
		
		currentNode->addChild(shape);
		
		ci::app::timeline().apply(&shape->getRotationAnim(), 360.f, speed).loop();
		
		previousNode->addChild(currentNode);
	}
	
	rootNode->setPosition(ci::app::getWindowWidth() / 2, ci::app::getWindowHeight() / 2);
	
	ci::app::timeline().apply(&rootNode->getRotationAnim(), 360.f, 60.0f).loop();
}
