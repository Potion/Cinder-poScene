//
//  poTextBoxApp.h
//  TextBox
//
//  Created by Stephen Varga on 3/29/14.
//
//

#pragma once

#include "poNodeContainer.h"

class poTextBoxApp;
typedef std::shared_ptr<poTextBoxApp> poTextBoxAppRef;

class poTextBoxApp
: public po::NodeContainer
{
public:
    static poTextBoxAppRef create();
    
    void setup();
    
    
protected:
};
