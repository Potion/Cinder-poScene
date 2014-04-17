#include "poShape.h"

#include "_TBOX_PREFIX_.h"

_TBOX_PREFIX_Ref _TBOX_PREFIX_::create() {
    _TBOX_PREFIX_Ref node(new _TBOX_PREFIX_());
    node->setup();
    return node;
}


void _TBOX_PREFIX_::setup() {

}