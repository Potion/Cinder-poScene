#include "_TBOX_PREFIX_.h"

_TBOX_PREFIX_Ref _TBOX_PREFIX_::create() 
{
    _TBOX_PREFIX_Ref view(new _TBOX_PREFIX_());
    view->setup();
    return view;
}

_TBOX_PREFIX_::_TBOX_PREFIX_()
{
}

void _TBOX_PREFIX_::setup() 
{

}