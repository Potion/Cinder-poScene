#pragma once

#include "poNodeContainer.h"
#include "poShape.h"
#include "poTextBox.h"

#include "Indicator.h"

//  create a shared pointer type for the class
class AlignmentSample;
typedef std::shared_ptr<AlignmentSample> AlignmentSampleRef;

//  define the class
class AlignmentSample
: public po::scene::NodeContainer
{
public:
    static AlignmentSampleRef create();
    
    void setup();
    
protected:
	
private:
    po::scene::ShapeRef     mShapeNode;
    po::scene::TextBoxRef   mTextBottom;
    
    //  Container to hold the indicators
    po::scene::NodeContainerRef             mIndicatorContainer;
    
    //  TextureFit types mapped to indicators
    std::vector<std::string>                mIndicatorNames;
    std::map<std::string, IndicatorRef>     mIndicators;
    

    void keyDown(ci::app::KeyEvent &event);
    
    void createIndicators();
    void activateIndicator(int num);

    
};
