#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "poScene.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class BasicTestApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void BasicTestApp::setup()
{
    po::NodeRef myNode = po::Node::create();
    po::NodeRef myOtherNode = po::Node::create();
    
    myNode->addChild(myOtherNode);
    
    myOtherNode->addChild(myNode);
    
    std::cout << (myNode->getParent() == myOtherNode) << std::endl;
}

void BasicTestApp::mouseDown( MouseEvent event )
{
}

void BasicTestApp::update()
{
}

void BasicTestApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( BasicTestApp, RendererGl )
