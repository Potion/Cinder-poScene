#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "poScene.h"
#include "VideoPlayerSample.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class VideoPlayerSampleApp
: public AppNative 
{
  public:
	void setup();
	void update();
	void draw();
    
    SceneRef scene;
};

void VideoPlayerSampleApp::setup()
{
    setWindowSize(1024, 768);
    scene = Scene::create(VideoPlayerSample::create());
}

void VideoPlayerSampleApp::update()
{
    scene->update();
}

void VideoPlayerSampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    scene->draw();
}

CINDER_APP_NATIVE( VideoPlayerSampleApp, RendererGl )
