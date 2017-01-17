#pragma once

#include "cinder/app/KeyEvent.h"

#include "poScene/ViewController.h"
#include "poScene/ShapeView.h"
#include "poScene/TextView.h"

#include "Indicator.h"

//  create a shared pointer type for the class
typedef std::shared_ptr<class AlignmentSampleViewController> AlignmentSampleViewControllerRef;

//  define the class
class AlignmentSampleViewController
: public po::scene::ViewController
{
public:
    static AlignmentSampleViewControllerRef create();
    void setup() override;
    
protected:
	
private:
	AlignmentSampleViewController() {};

    po::scene::ShapeViewRef  mShapeView;
    po::scene::TextViewRef   mTextBottom;
    
    //  Container to hold the indicators
    po::scene::ViewRef mIndicatorContainer;
    
    //  Alignment types mapped to indicators
    std::vector<std::string>                mIndicatorNames;
    std::map<std::string, IndicatorRef>     mIndicators;
    

    void keyDown(ci::app::KeyEvent &event);
    
    void createIndicators();
    void activateIndicator(int num); 
};
