#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/app/RendererGl.h"

#include "poScene/Scene.h"

#include "TextSampleAdvanced.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace po::scene;

class TextSampleAdvancedApp
: public App 
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

CINDER_APP( TextSampleAdvancedApp, RendererGl )
