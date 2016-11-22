#pragma once

#include "cinder/app/KeyEvent.h"

#include "poScene/ViewContainer.h"
#include "poScene/ShapeView.h"
#include "poScene/TextView.h"

#include "Indicator.h"

//  create a shared pointer type for the class
class AlignmentSample;
typedef std::shared_ptr<AlignmentSample> AlignmentSampleRef;

//  define the class
class AlignmentSample
: public po::scene::ViewContainer
{
public:
    static AlignmentSampleRef create();
    
    void setup();
    
protected:
	
private:
    po::scene::ShapeViewRef  mShapeView;
    po::scene::TextViewRef   mTextBottom;
    
    //  Container to hold the indicators
    po::scene::ViewContainerRef             mIndicatorContainer;
    
    //  Alignment types mapped to indicators
    std::vector<std::string>                mIndicatorNames;
    std::map<std::string, IndicatorRef>     mIndicators;
    

    void keyDown(ci::app::KeyEvent &event);
    
    void createIndicators();
    void activateIndicator(int num);

    
};
