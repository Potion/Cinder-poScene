#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "poScene.h"
#include "MouseEventsSample.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class MouseEventsSampleApp
: public AppNative 
{
  public:
	void setup();
	void update();
	void draw();
    
    SceneRef scene;
};

void MouseEventsSampleApp::setup()
{
    scene = Scene::create(MouseEventsSample::create());
}

void MouseEventsSampleApp::update()
{
    scene->update();
}

void MouseEventsSampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    scene->draw();
}

CINDER_APP_NATIVE( MouseEventsSampleApp, RendererGl )
