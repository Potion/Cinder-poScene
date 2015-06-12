#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "poScene.h"
#include "BoundsSample.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class BoundsSampleApp
: public AppNative 
{
  public:
	void setup();
	void update();
	void draw();
    
    SceneRef scene;
};

void BoundsSampleApp::setup()
{
    setWindowSize(800, 600);
    //ci::gl::enableAlphaBlending();
    scene = Scene::create(BoundsSample::create());
}

void BoundsSampleApp::update()
{
    scene->update();
}

void BoundsSampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0.0, 0.0f, 0.0 ) );
    scene->draw();
}

CINDER_APP_NATIVE( BoundsSampleApp, RendererGl )
