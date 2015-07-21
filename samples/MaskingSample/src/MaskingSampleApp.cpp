#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/app/RendererGl.h"
#include "poScene.h"
#include "MaskingSample.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class MaskingSampleApp
: public App
{
  public:
	void setup();
	void update();
	void draw();
    
    SceneRef scene;
};

void MaskingSampleApp::setup()
{
	setWindowSize(683, 1024);
    scene = Scene::create(MaskingSample::create());
}

void MaskingSampleApp::update()
{
    scene->update();
}

void MaskingSampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 1, 0, 0 ) );
    scene->draw();
}

CINDER_APP( MaskingSampleApp, RendererGl )
