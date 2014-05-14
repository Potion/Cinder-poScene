#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"

#include "poScene.h"
#include "Masking.h"

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
    
    //scene = po::Scene::create(Masking::create());
    tex     = gl::Texture::create(loadImage(loadAsset("zach.jpg")));
    mask    = gl::Texture::create(loadImage(loadAsset("mask.png")));
    
    //Load the shader
    try {
        shader = gl::GlslProg ( loadAsset("poMask_vert.glsl"), loadAsset( "poMask_frag.glsl"));
    } catch (gl::GlslProgCompileExc e) {
        console() << "Could not load shader: " << e.what() << std::endl;
        exit(1);
    }
    
    MaskingRef m = Masking::create();
    m->setMask(mask);
    scene = po::Scene::create(m);
}

void MaskingApp::update()
{
    scene->update();
}

void MaskingApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    scene->draw();
    
//    gl::color(255, 0, 255);
//    tex->bind(0);
//    //mask->bind(1);
//    
//    shader.bind();
//    shader.uniform("tex", 0);
//    shader.uniform("mask", 1);
//    shader.uniform ( "contentScale", Vec2f((float)tex->getWidth() / (float)mask->getWidth(), (float)tex->getHeight() / (float)mask->getHeight() ) );
//    shader.uniform ( "scrollOffset", Vec2f(0,0) );
//    
//    gl::pushMatrices();
//    gl::scale((float)getWindowWidth()/(float)tex->getWidth(), (float)getWindowHeight()/(float)tex->getHeight());
//    gl::drawSolidRect(tex->getBounds());
//    gl::popMatrices();
//    
//    tex->unbind();
//    mask->unbind();
//    shader.unbind();
}

CINDER_APP_NATIVE( MaskingApp, RendererGl )
