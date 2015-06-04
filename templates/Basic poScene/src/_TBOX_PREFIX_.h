#pragma once

#include "poNodeContainer.h"

class _TBOX_PREFIX_;
typedef std::shared_ptr<_TBOX_PREFIX_> _TBOX_PREFIX_Ref;

class _TBOX_PREFIX_
: public po::scene::NodeContainer
{
public:
    static _TBOX_PREFIX_Ref create();
    
    void setup();
    
protected:
	
private:

};