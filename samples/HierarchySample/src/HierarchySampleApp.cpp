#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/app/RendererGl.h"

#include "poScene/Scene.h"

#include "HierarchySample.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class HierarchySampleApp
: public App
{
  public:
	void setup();
	void update();
	void draw();
    
    SceneRef scene;
};

void HierarchySampleApp::setup()
{
    scene = Scene::create(HierarchySample::create());
}

void HierarchySampleApp::update()
{
    scene->update();
}

void HierarchySampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    scene->draw();
}

CINDER_APP( HierarchySampleApp, RendererGl )
