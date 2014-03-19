#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "poScene.h"
#include "TestNode.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class BasicTestApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    
    po::SceneRef scene;
};

void BasicTestApp::setup()
{
    scene = po::Scene::create();
    
    po::NodeRef myNode = po::Node::create();
    po::NodeRef myOtherNode = po::Node::create();
    
    std::cout << "Has Parent: " << myNode->hasParent() << std::endl;
    
    myNode->addChild(myOtherNode);
    
    myOtherNode->addChild(myNode);
    
    std::cout << (myNode->getParent() == myOtherNode) << std::endl;
    std::cout << myNode->hasParent() << std::endl;
    
    //std::cout << "Remove Child Successful: " << myOtherNode->removeChild(myNode) << std::endl;;
    
    std::cout << "Has Parent: " << myNode->hasParent() << std::endl;
    
    TestNodeRef testNode = TestNode::create();
    scene->getRootNode()->addChild(testNode);
    testNode->position.set(50, 50);
    testNode->rotation.set(0, 0, 45);
}

void BasicTestApp::mouseDown( MouseEvent event )
{
}

void BasicTestApp::update()
{
    scene->update();
}

void BasicTestApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    gl::color(255, 0, 0);
    scene->draw();
}

CINDER_APP_NATIVE( BasicTestApp, RendererGl )
