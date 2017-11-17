#pragma once

#include "poScene/ViewController.h"

class DragAndDrop;
typedef std::shared_ptr<DragAndDrop> DragAndDropRef;

class DragAndDrop
: public po::scene::ViewController
{
public:
    static DragAndDropRef create();
    
    void setup();
    
protected:
	DragAndDrop();

private:

};
