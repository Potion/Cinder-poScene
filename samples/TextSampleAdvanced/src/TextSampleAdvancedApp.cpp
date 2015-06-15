#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "poScene.h"
#include "TextSampleAdvanced.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class TextSampleAdvancedApp
: public AppNative 
{
  public:
	void setup();
	void update();
	void draw();
    
    SceneRef scene;
};

void TextSampleAdvancedApp::setup()
{
    scene = Scene::create(TextSampleAdvanced::create());
}

void TextSampleAdvancedApp::update()
{
    scene->update();
}

void TextSampleAdvancedApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    scene->draw();
}

CINDER_APP_NATIVE( TextSampleAdvancedApp, RendererGl )
