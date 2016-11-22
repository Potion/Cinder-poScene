#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/app/RendererGl.h"

#include "poScene/Scene.h"

#include "ShapeTextureSample.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class ShapeTextureSampleApp
: public App
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

CINDER_APP( ShapeTextureSampleApp, RendererGl )
