#pragma once

#include "poNodeContainer.h"
#include "poShape.h"
#include "poTextBox.h"

#include "Indicator.h"

class ShapeTextureSample;
typedef std::shared_ptr<ShapeTextureSample> ShapeTextureSampleRef;

class ShapeTextureSample
: public po::scene::NodeContainer
{
public:
    static ShapeTextureSampleRef create();
    
    void setup();
    
protected:
	ShapeTextureSample();

private:
    po::scene::ShapeRef     mRectShape;     // rectangle for reference
    po::scene::ShapeRef     mEllipseShape;  // ellipse that will have texture
    po::scene::ShapeRef     mTriangleShape; // custom cinder shape
    ci::gl::TextureRef      mTexture;       // cinder texture
    
    //  Container to hold the indicators
    NodeContainerRef        mFitIndicatorContainer;
    NodeContainerRef        mAlignIndicatorContainer;
    
    //  TextureFit types mapped to indicators
    std::vector<std::string>                mFitIndicatorNames;
    std::vector<std::string>                mAlignIndicatorNames;
    std::map<std::string, IndicatorRef>     mFitIndicators;
    std::map<std::string, IndicatorRef>     mAlignIndicators;
    
    TextureFit::Type                        mCurrentFitType;
    Alignment                               mCurrentAlignment;
    
    void keyDown(ci::app::KeyEvent &event);
    
    void createFitIndicators();
    void createAlignmentIndicators();
    
    void activateFitIndicator(int num);
    void activateAlignmentIndicator(int num);

};
