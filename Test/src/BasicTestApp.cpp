#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "poScene.h"

//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
//#include "catch.hpp"


using namespace ci;
using namespace ci::app;
using namespace std;

class BasicTestApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    
    po::NodeRef myNode;
    po::NodeRef myOtherNode;
};

void BasicTestApp::setup()
{
    myNode = po::Node::create();
    myOtherNode = po::Node::create();
    
    std::cout << "Has Parent: " << myNode->hasParent() << std::endl;
    
    myNode->addChild(myOtherNode);
    
    myOtherNode->addChild(myNode);
    
    std::cout << (myNode->getParent() == myOtherNode) << std::endl;
    std::cout << myNode->hasParent() << std::endl;
    
    std::cout << "Remove Child Successful: " << myOtherNode->removeChild(myNode) << std::endl;;
    
    std::cout << "Has Parent: " << myNode->hasParent() << std::endl;
    

}

//TEST_CASE("Parent reset on removeChild()", "[myNode->hasParent()]" )
//{
//    REQUIRE("myNode->hasParent() == 0");
//}

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
