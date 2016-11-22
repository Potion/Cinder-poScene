#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/app/RendererGl.h"

#include "poScene/Scene.h"
#include "AlignmentSample.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class AlignmentSampleApp
: public App
{
  public:
	void setup();
	void update();
	void draw();
    
    SceneRef scene;
};

void AlignmentSampleApp::setup()
{
    scene = Scene::create(AlignmentSample::create());
}

void AlignmentSampleApp::update()
{
    scene->update();
}

void AlignmentSampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    scene->draw();
}

CINDER_APP( AlignmentSampleApp, RendererGl )
