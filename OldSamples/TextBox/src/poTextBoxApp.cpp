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
    std::string text = "this is a simple paragraph that is meant to be nice and easy to type which is why there will be mommas no periods or any capital letters so i guess this means that it cannot really be considered a paragraph but just a series of run on sentences this should help you get faster at typing as im trying not to use too many difficult words in it although i think that i might start making it hard by including some more difficult letters I'm typing pretty quickly so forgive me for any mistakes i think that i will not just tell you a story about the time i went to the zoo and found a monkey and a fox playing together they were so cute and i think that they were not supposed to be in the same cage but they somehow were and i loved watching them horse around forgive the pun well i hope that it has been highly enjoyable typing this paragraph and i wish you the best of luck getting the best score that you possibly can";
    
    std::shared_ptr<ci::TextBox> t(new ci::TextBox());
    t->font(ci::Font( "Helvetica", 24 ))
    .text(text)
    .size(ci::Vec2f(300,ci::TextBox::GROW));
    
    TextBoxRef tb = TextBox::create(t);
    

    tb->setPosition(ci::Vec2f(50,50));
    tb->setDrawBounds(true);
    //tb->setRotation(45);
	
    std::cout << tb->getWidth() << std::endl;
    
    addChild(tb);
}

