#pragma once

#include "poNodeContainer.h"

class _TBOX_PREFIX_;
typedef std::shared_ptr<_TBOX_PREFIX_> _TBOX_PREFIX_Ref;

class _TBOX_PREFIX_
: public po::NodeContainer
{
public:
    static _TBOX_PREFIX_Ref create();
    
    void setup();
    
    #pragma message "Should we declare all events for this template?"
protected:
};