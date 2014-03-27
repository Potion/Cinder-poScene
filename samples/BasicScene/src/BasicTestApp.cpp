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
	void update();
	void draw();
    
    po::SceneRef scene;
    
    ci::Font mFont;
};

void BasicTestApp::setup()
{
    //This
//    scene = po::Scene::create();
//    scene->setRootNode(TestNode::create());
    
    //Or this
    scene = po::Scene::create(TestNode::create());
    
    mFont = Font( "Geneva", 18.0f );
}

void BasicTestApp::update()
{
    //Update our scene
    scene->update();
}

void BasicTestApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    //Draw our scene
    scene->draw();
    
    gl::drawString( "Framerate: " + ci::toString(getAverageFps()), Vec2f( 10.0f, 10.0f ), Color::white(), mFont );
}

CINDER_APP_NATIVE( BasicTestApp, RendererGl )
