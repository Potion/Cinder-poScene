//
//  poTextBoxApp.cpp
//  TextBox
//
//  Created by Stephen Varga on 3/29/14.
//
//

#include "poTextBoxApp.h"

#include "poTextBox.h"

poTextBoxAppRef poTextBoxApp::create() {
    poTextBoxAppRef app(new poTextBoxApp());
    app->setup();
    return app;
}


void poTextBoxApp::setup() {
    std::string text = "Testing Testing Testing TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting TestingTesting Testing";
    
    po::TextBoxRef t = po::TextBox::create();
    t->font(ci::Font( "Helvetica", 12 ))
        .text(text)
        .size(ci::Vec2f(200,200))
        .render();
    
    t->setDrawBounds(true);
    t->setRotation(45);
    
    std::cout << t->getWidth() << std::endl;
    
    addChild(t);
}

