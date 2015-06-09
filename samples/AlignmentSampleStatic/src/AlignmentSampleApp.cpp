#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "poScene.h"
#include "AlignmentSample.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class AlignmentSampleApp
: public AppNative 
{
  public:
	void setup();
	void update();
	void draw();
    
    SceneRef scene;
};

void AlignmentSampleApp::setup()
{
    setWindowSize(550, 550);
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

CINDER_APP_NATIVE( AlignmentSampleApp, RendererGl )
