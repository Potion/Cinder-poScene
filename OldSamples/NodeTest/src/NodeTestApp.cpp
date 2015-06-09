#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "poScene.h"
#include "NodeTestRoot.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class NodeTestApp : public AppNative {
  public:
	void setup();
	void update();
	void draw();
    
    po::SceneRef scene;
};

void NodeTestApp::setup()
{
    scene = po::Scene::create(NodeTestRoot::create());
    ci::gl::enableAlphaBlending();
}

void NodeTestApp::update()
{
    scene->update();
}

void NodeTestApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    scene->draw();
}

CINDER_APP_NATIVE( NodeTestApp, RendererGl )
