#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"

#include "poScene.h"
#include "Masking.h"

#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class MaskingApp : public AppNative {
  public:
	void setup();
	void update();
	void draw();
    
    po::SceneRef scene;
    
    gl::TextureRef tex;
    gl::TextureRef mask;
    
    gl::GlslProg shader;
};

void MaskingApp::setup()
{
    gl::enableAlphaBlending();
    MaskingRef m = Masking::create();
    scene = po::Scene::create(m);
}

void MaskingApp::update()
{
    scene->update();
}

void MaskingApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 1, 0, 0 ) );
    scene->draw();
}

CINDER_APP_NATIVE( MaskingApp, RendererGl )
