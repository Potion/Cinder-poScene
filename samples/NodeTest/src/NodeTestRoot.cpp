#include "poShape.h"

#include "NodeTestRoot.h"

NodeTestRootRef NodeTestRoot::create() {
    NodeTestRootRef root(new NodeTestRoot());
    root->setup();
    return root;
}


void NodeTestRoot::setup() {

}
