#pragma once

#include "cinder/app/KeyEvent.h"

#include "poScene/View.h"
#include "poScene/ShapeView.h"
#include "poScene/TextView.h"

#include "Indicator.h"

class ShapeTextureSample;
typedef std::shared_ptr<ShapeTextureSample> ShapeTextureSampleRef;

class ShapeTextureSample
: public po::scene::View
{
public:
    static ShapeTextureSampleRef create();
    
    void setup();
    
protected:
	ShapeTextureSample();

private:
    po::scene::ShapeViewRef     mRectShape;     // rectangle for reference
    po::scene::ShapeViewRef     mEllipseShape;  // ellipse that will have texture
    po::scene::ShapeViewRef     mTriangleShape; // custom cinder shape
    ci::gl::TextureRef      mTexture;       // cinder texture
    
    //  Container to hold the indicators
    ViewRef        mFitIndicatorContainer;
    ViewRef        mAlignIndicatorContainer;
    
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
