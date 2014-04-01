#pragma once

#include "poNodeContainer.h"

class _TBOX_PREFIX_Root;
typedef std::shared_ptr<_TBOX_PREFIX_Root> _TBOX_PREFIX_RootRef;

class _TBOX_PREFIX_Root
: public po::NodeContainer
{
public:
    static _TBOX_PREFIX_RootRef create();
    
    void setup();
    
    #pragma message "Should we declare all events for this template?"
protected:
};