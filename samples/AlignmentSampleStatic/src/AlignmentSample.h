#pragma once

#include "poNodeContainer.h"


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

};
