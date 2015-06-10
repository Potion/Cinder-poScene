#pragma once

#include "poNodeContainer.h"

using namespace po::scene;

class HierarchySample;
typedef std::shared_ptr<HierarchySample> HierarchySampleRef;

class HierarchySample
: public po::scene::NodeContainer
{
public:
    static HierarchySampleRef create();
    
    void setup();
    
protected:
	
private:

};
