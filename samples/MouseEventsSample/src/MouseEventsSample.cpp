#include "MouseEventsSample.h"

MouseEventsSampleRef MouseEventsSample::create() 
{
    MouseEventsSampleRef node(new MouseEventsSample());
    node->setup();
    return node;
}


void MouseEventsSample::setup() 
{
	
}
