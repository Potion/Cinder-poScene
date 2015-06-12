#pragma once

#include "poNodeContainer.h"
#include "poShape.h"
#include "poTextBox.h"

class BoundsSample;
typedef std::shared_ptr<BoundsSample> BoundsSampleRef;

class BoundsSample
: public po::scene::NodeContainer
{
public:
    static BoundsSampleRef create();
    
    void setup();
    
protected:
    void update();
	
private:
    ci::TextBox mInfoText;
    po::scene::TextBoxRef mWindowTextBox;
    
    po::scene::ShapeRef mBG;
    po::scene::TextBoxRef mTextBox;
    
    po::scene::NodeContainerRef mNodeContainer;
    po::scene::ShapeRef mNodeContainerBG;
    po::scene::TextBoxRef mNodeContainerTextBox;
    
    
    void nodeMouseOver(po::scene::MouseEvent &event);
    po::scene::NodeRef mSelectedNode;
    
    void keyPressed(ci::app::KeyEvent &key);
    
    std::string getNodeInfo(po::scene::NodeRef);

};
