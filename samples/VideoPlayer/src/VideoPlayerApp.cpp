#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "poScene.h"
#include "VideoPlayer.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class VideoPlayerApp : public AppNative {
  public:
	void setup();
	void update();
	void draw();
    
    po::SceneRef scene;
};

void VideoPlayerApp::setup()
{
    scene = po::Scene::create(VideoPlayer::create());
}

void VideoPlayerApp::update()
{
    scene->update();
}

void VideoPlayerApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    scene->draw();
}

CINDER_APP_NATIVE( VideoPlayerApp, RendererGl )
