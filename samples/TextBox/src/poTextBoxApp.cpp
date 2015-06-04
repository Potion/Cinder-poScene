//
//  poTextBoxApp.cpp
//  TextBox
//
//  Created by Stephen Varga on 3/29/14.
//
//

#include "poTextBoxApp.h"

#include "poTextBox.h"

using namespace po::scene;

poTextBoxAppRef poTextBoxApp::create() {
    poTextBoxAppRef app(new poTextBoxApp());
    app->setup();
    return app;
}


void poTextBoxApp::setup() {
    std::string text = "Testing Testing Testing TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting Testing";
    
    std::shared_ptr<ci::TextBox> t(new ci::TextBox());
    t->font(ci::Font( "Helvetica", 12 ))
    .text(text)
    .size(ci::Vec2f(200,200));
    
    TextBoxRef tb = TextBox::create(t);
    

    
    tb->setDrawBounds(true);
    tb->setRotation(45);
	
    std::cout << tb->getWidth() << std::endl;
    
    addChild(tb);
}

