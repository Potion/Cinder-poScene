#pragma once

#include "cinder/app/KeyEvent.h"
#include "poScene/ViewController.h"
#include "poScene/ShapeView.h"
#include "poScene/TextView.h"

class BoundsSampleViewController;
typedef std::shared_ptr<BoundsSampleViewController> BoundsSampleViewControllerRef;

class BoundsSampleViewController
: public po::scene::ViewController
{
public:
    static BoundsSampleViewControllerRef create();
    
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
