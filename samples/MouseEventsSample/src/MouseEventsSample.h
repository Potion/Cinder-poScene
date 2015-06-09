#pragma once

#include "poNodeContainer.h"

class MouseEventsSample;
typedef std::shared_ptr<MouseEventsSample> MouseEventsSampleRef;

class MouseEventsSample
: public po::scene::NodeContainer
{
public:
    static MouseEventsSampleRef create();
    
    void setup();
    
protected:
	
private:
	
	
};
