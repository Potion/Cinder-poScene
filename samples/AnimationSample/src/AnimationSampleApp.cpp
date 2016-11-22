#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/app/RendererGl.h"

#include "poScene/Scene.h"
#include "AnimationSample.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class AnimationSampleApp
: public App
{
  public:
	void setup();
	void update();
	void draw();
    
    SceneRef scene;
};

void AnimationSampleApp::setup()
{
    scene = Scene::create(AnimationSample::create());
}

void AnimationSampleApp::update()
{
    scene->update();
}

void AnimationSampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    scene->draw();
}

CINDER_APP( AnimationSampleApp, RendererGl )
