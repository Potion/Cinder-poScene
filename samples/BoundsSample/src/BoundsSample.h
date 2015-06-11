#pragma once

#include "BGNodeContainer.h"

class BoundsSample;
typedef std::shared_ptr<BoundsSample> BoundsSampleRef;

class BoundsSample
: public BGNodeContainer
{
public:
    static BoundsSampleRef create();
    
    void setup();
    
protected:
    void update();
	
private:
    void nodeMouseOver(po::scene::MouseEvent &event);
    po::scene::NodeRef mSelectedNode;
    
    po::scene::TextBoxRef mTextBox;
    ci::TextBox mText;

};
