#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

#include "poScene.h"
#include "TestNode.h"

#include "poShape.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class BasicTestApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );
	void mouseDrag( MouseEvent event );
	void update();
	void draw();
    
    po::SceneRef scene, scene2;
    po::ShapeRef r,e;
};

void BasicTestApp::setup()
{
    scene = po::Scene::create();
    
//    //Test Parents
//    po::NodeRef myNode = po::Node::create();
//    po::NodeRef myOtherNode = po::Node::create();
//    
//    std::cout << "Has Parent: " << myNode->hasParent() << std::endl;
//    
//    myNode->addChild(myOtherNode);
//    
//    myOtherNode->addChild(myNode);
//    
//    std::cout << (myNode->getParent() == myOtherNode) << std::endl;
//    std::cout << myNode->hasParent() << std::endl;
//    std::cout << "Remove Child Successful: " << myOtherNode->removeChild(myNode) << std::endl;;
//    std::cout << "Has Parent: " << myNode->hasParent() << std::endl;
//    
        TestNodeRef testNode    = TestNode::create();
//    TestNodeRef testNode2   = TestNode::create();
    
    r = po::Shape::createEllipse(200,100);
    r->setDrawBoundsEnabled(true);
    r->position.set(25,100);
    r->fillColor.set(255,255,0);
    r->setStrokeEnabled(true);
    r->strokeColor.set(255, 0, 255);
    r->scale.set(1, 2);
    
    e = po::Shape::createRect(50, 100);
    e->position.set(200,200);
    e->fillColor.set(0,255,255);
    e->rotation.set(0, 0, 45);
    e->setDrawBoundsEnabled(true);
    e->setInteractionEnabled(true);
    
    //Test scene graph
    scene->getRootNode()->addChild(r);
    scene->getRootNode()->addChild(e);
    scene->getRootNode()->addChild(testNode);
    
    scene->getRootNode()->setDrawBoundsEnabled(true);

}

void BasicTestApp::mouseDown( MouseEvent event )
{
    scene->getRootNode()->removeChild(e);
}

void BasicTestApp::update()
{
    scene->update();
}

void BasicTestApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    scene->draw();
}

void BasicTestApp::mouseDrag( MouseEvent event )
{
	r->position.set(event.getPos());
}

CINDER_APP_NATIVE( BasicTestApp, RendererGl )
