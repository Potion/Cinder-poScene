#pragma once

#include "poScene/View.h"

class _TBOX_PREFIX_;
typedef std::shared_ptr<_TBOX_PREFIX_> _TBOX_PREFIX_Ref;

class _TBOX_PREFIX_
: public po::scene::Viewcontroller
{
public:
    static _TBOX_PREFIX_Ref create();
    
    void viewDidLoad() override;
    
protected:
	_TBOX_PREFIX_();

private:

};