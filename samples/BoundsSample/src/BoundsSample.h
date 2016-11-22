#pragma once

#include "cinder/app/KeyEvent.h"

#include "poScene/ViewContainer.h"
#include "poScene/ShapeView.h"
#include "poScene/TextView.h"

class BoundsSample;
typedef std::shared_ptr<BoundsSample> BoundsSampleRef;

class BoundsSample
: public po::scene::View
{
public:
    static BoundsSampleRef create();
    
    void setup();
    
protected:
    void update();
	
private:
    ci::TextBox mInfoText;
    po::scene::TextViewRef mWindowTextBox;
    
    po::scene::ShapeViewRef mBG;
    po::scene::TextViewRef mTextView;
    
    po::scene::ViewRef mContainerView;
    po::scene::ShapeViewRef mContainerViewBG;
    po::scene::TextViewRef mContainerTextView;
    
    
    void nodeMouseOver(po::scene::MouseEvent &event);
    po::scene::ViewRef mSelectedView;
    
    void keyPressed(ci::app::KeyEvent &key);
    
    std::string getViewInfo(po::scene::ViewRef);

};
