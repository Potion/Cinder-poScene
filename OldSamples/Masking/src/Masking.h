#pragma once

#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Fbo.h"

#include "poNodeContainer.h"

class Masking;
typedef std::shared_ptr<Masking> MaskingRef;

class Masking
: public po::scene::NodeContainer
{
public:
    static MaskingRef create();
    
    void setup();
    void update();
//    void _drawTree();
//    void _drawFbo();
    
    void setMask(po::scene::ShapeRef mask) { mMask = mask; };
    void keyDown(ci::app::KeyEvent &event);
    
    void mouseMove(po::scene::MouseEvent &event);
    
protected:
    //Masking
    po::scene::ShapeRef mMask;
    ci::gl::GlslProg    mShader;
    ci::gl::Fbo         mFbo;
    
    ci::Vec2f maskPos;
    float targetPos;
    
    po::scene::ShapeRef mZach;
};