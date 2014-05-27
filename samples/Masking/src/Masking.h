#pragma once

#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Fbo.h"

#include "poNodeContainer.h"

class Masking;
typedef std::shared_ptr<Masking> MaskingRef;

class Masking
: public po::NodeContainer
{
public:
    static MaskingRef create();
    
    void setup();
    void _drawTree();
    void _drawFbo();
    
    void setMask(po::ShapeRef mask) { mMask = mask; };
    void keyDown(po::KeyEvent &event);
    
    void mouseMove(po::MouseEvent &event);
    
protected:
    //Masking
    po::ShapeRef mMask;
    ci::gl::GlslProg    mShader;
    ci::gl::Fbo         mFbo;
    
    ci::Vec2f maskPos;
    
    po::ShapeRef mZach;
};
