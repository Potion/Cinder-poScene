#pragma once

#include "poNodeContainer.h"
#include "poShape.h"
#include "poTextBox.h"

#include "Indicator.h"


class TextureSample;
typedef std::shared_ptr<TextureSample> TextureSampleRef;

class TextureSample
: public po::scene::NodeContainer
{
public:
    static TextureSampleRef create();
    
    void setup();
    
protected:
	TextureSample();

private:
    po::scene::ShapeRef     mRectShape;     // rectangle for reference
    po::scene::ShapeRef     mEllipseShape;  // ellipse that will have texture
    po::scene::ShapeRef     mTriangleShape; // custom cinder shape
    ci::gl::TextureRef          mTexture;       // cinder texture
    
    //  Container to hold the indicators
    NodeContainerRef        mIndicatorContainer;
    
    //  Events mapped to indicators
    std::vector<std::string>                mIndicatorNames;
    std::map<std::string, IndicatorRef>     mIndicators;
    
    po::scene::TextBoxRef   mTexturefitLabel;
    
    void keyDown(ci::app::KeyEvent &event);
    
    void createIndicators();
    void activateIndicator(int fit);
};
