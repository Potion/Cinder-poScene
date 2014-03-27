#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/Utilities.h";

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
    
    ci::Font mFont;
};

void BasicTestApp::setup()
{
    scene = po::Scene::create();
    
    TestNodeRef testNode = TestNode::create();
    testNode->setup();
    scene->getRootNode()->addChild(testNode);
    
    scene->getRootNode()->setDrawBoundsEnabled(true);
    
    mFont = Font( "Geneva", 18.0f );
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
    
    
    gl::drawString( "Framerate: " + ci::toString(getAverageFps()), Vec2f( 10.0f, 10.0f ), Color::white(), mFont );
}

void BasicTestApp::mouseDrag( MouseEvent event )
{
}

CINDER_APP_NATIVE( BasicTestApp, RendererGl )
