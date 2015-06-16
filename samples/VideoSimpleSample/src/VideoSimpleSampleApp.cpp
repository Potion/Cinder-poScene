#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "poScene.h"
#include "VideoSimpleSample.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class VideoSimpleSampleApp
: public AppNative 
{
  public:
	void setup();
	void update();
	void draw();
    
    SceneRef scene;
};

void VideoSimpleSampleApp::setup()
{
    setWindowSize(1024, 768);
    scene = Scene::create(VideoSimpleSample::create());
}

void VideoSimpleSampleApp::update()
{
    scene->update();
}

void VideoSimpleSampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    scene->draw();
}

CINDER_APP_NATIVE( VideoSimpleSampleApp, RendererGl )
