#include "poShape.h"

#include "_TBOX_PREFIX_Root.h"

_TBOX_PREFIX_RootRef _TBOX_PREFIX_Root::create() {
    _TBOX_PREFIX_RootRef root(new _TBOX_PREFIX_Root());
    root->setup();
    return root;
}


void _TBOX_PREFIX_Root::setup() {

}