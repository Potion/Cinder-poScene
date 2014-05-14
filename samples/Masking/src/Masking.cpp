#include "poShape.h"

#include "Masking.h"
#include "poScene.h"

using namespace ci;
using namespace ci::app;

bool bUseFbo = false;
bool bShowMask = true;

MaskingRef Masking::create() {
    MaskingRef node(new Masking());
    node->setup();
    return node;
}


void Masking::setup() {
    //setPosition(200,50);
    po::ShapeRef s = po::Shape::create();
    addChild(s);
    
    ci::gl::TextureRef texture = ci::gl::Texture::create(ci::loadImage(ci::app::loadAsset(("beer.jpg"))));
    po::ShapeRef image = po::Shape::createRect(texture->getWidth()/10, texture->getHeight()/10);
    image->setTexture(texture, po::TextureFit::EXACT);
    image->setAlignment(po::Alignment::CENTER_CENTER);
    image->setPosition(ci::app::getWindowWidth()/2,
                       ci::app::getWindowHeight()/2);
    image->setRotation(45);
    addChild(image);
    
    po::ShapeRef mask = po::Shape::createRect(200,200);
    mask->setFillColor(255,0,255);
    mask->setAlignment(po::Alignment::CENTER_CENTER);
    mask->setPosition(image->getPosition().x, image->getPosition().y);
    addChild(mask);
    
    //Load the shader
    try {
        mShader = gl::GlslProg ( loadAsset("poMask_vert.glsl"), loadAsset( "poMask_frag.glsl"));
    } catch (gl::GlslProgCompileExc e) {
        console() << "Could not load shader: " << e.what() << std::endl;
        exit(1);
    }
    
    mFbo = gl::Fbo(getWidth(), getHeight());
    
    //setCacheToFboEnabled(true);
}

void Masking::drawTree() {
    if(!bUseFbo) {
        po::Node::drawTree();
        return;
    }
    
    //Save the window buffer
    gl::SaveFramebufferBinding *binding = new gl::SaveFramebufferBinding();
    
    //Setup the FBO
    gl::setViewport(mFbo.getBounds());
    mFbo.bindFramebuffer();
    
    ci::CameraOrtho cam;
    cam.setOrtho( 0,getBounds().getWidth(), getBounds().getHeight(), 0, -1, 1 );
    gl::setMatrices(cam);
    
    //Draw into the FBO
    gl::pushMatrices();
    gl::translate(-getFrame().getUpperLeft());
    
    po::Node::drawTree();
    
    gl::popMatrices();
    
    delete binding;
    
    gl::setViewport(app::getWindowBounds());
    gl::setMatrices(getScene()->getCamera());
    
    drawFbo();
    
}

void Masking::drawFbo() {
    if(!bUseFbo) {
        po::NodeContainer::draw();
        return;
    }
    
    gl::Texture tex = mFbo.getTexture();
    tex.setFlipped(true);
    
    if(bShowMask) {
        tex.bind(0);
        mMaskTex->bind(1);
        
        mShader.bind();
        mShader.uniform("tex", 0);
        mShader.uniform("mask", 1);
        mShader.uniform ( "contentScale", Vec2f((float)tex.getWidth() / (float)mMaskTex->getWidth(), (float)tex.getHeight() / (float)mMaskTex->getHeight() ) );
        mShader.uniform ( "maskPosition", Vec2f(0.5f,0.5f) );
    }

    
    beginDrawTree();
    
    gl::color(255,255,255);
    if(bShowMask) {
        //gl::scale(1,-1,1);
        gl::drawSolidRect(mFbo.getBounds());
    } else {
        gl::draw(tex);
        //gl::draw(mMaskTex);
    }

    finishDrawTree();

    if(bShowMask) {
        tex.unbind();
        mMaskTex->unbind();
        mShader.unbind();
    }
}


void Masking::keyDown(po::KeyEvent &event)
{
    if(event.getChar() == 'm') {
        bShowMask = !bShowMask;
    }
    
    else if(event.getChar() == 'f') {
        bUseFbo = !bUseFbo;
    }
}
