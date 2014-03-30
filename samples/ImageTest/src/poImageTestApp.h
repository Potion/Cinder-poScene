//
//  poImageTestApp.h
//  ImageTest
//
//  Created by Stephen Varga on 3/30/14.
//
//

#pragma once

#include "poNodeContainer.h"

class poImageTestApp;
typedef std::shared_ptr<poImageTestApp> poImageTestAppRef;

class poImageTestApp
: public po::NodeContainer
{
public:
    static poImageTestAppRef create();
    
    void setup();
    
protected:
};