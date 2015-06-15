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
    po::scene::ShapeRef     mPlainShape;    // plain rectangle for reference
    po::scene::ShapeRef     mTexShape;      // ellipse that will have texture
    ci::gl::TextureRef      mTexture;       // image texture
    
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
