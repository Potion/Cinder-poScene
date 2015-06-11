#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "poScene.h"
#include "TextureSample.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class TextureSampleApp
: public AppNative 
{
  public:
	void setup();
	void update();
	void draw();
    
    SceneRef scene;
};

void TextureSampleApp::setup()
{
    setWindowSize(1024, 768);
    scene = Scene::create(TextureSample::create());
}

void TextureSampleApp::update()
{
    scene->update();
}

void TextureSampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    scene->draw();
}

CINDER_APP_NATIVE( TextureSampleApp, RendererGl )
