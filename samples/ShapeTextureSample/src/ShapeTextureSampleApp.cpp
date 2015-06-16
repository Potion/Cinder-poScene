#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "poScene.h"
#include "ShapeTextureSample.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class ShapeTextureSampleApp
: public AppNative 
{
  public:
	void setup();
	void update();
	void draw();
    
    SceneRef scene;
};

void ShapeTextureSampleApp::setup()
{
    setWindowSize(1024, 768);
    scene = Scene::create(ShapeTextureSample::create());
}

void ShapeTextureSampleApp::update()
{
    scene->update();
}

void ShapeTextureSampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    scene->draw();
}

CINDER_APP_NATIVE( ShapeTextureSampleApp, RendererGl )
