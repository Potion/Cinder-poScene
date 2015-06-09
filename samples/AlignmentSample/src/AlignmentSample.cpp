#include "AlignmentSample.h"


using namespace po::scene;

AlignmentSampleRef AlignmentSample::create() 
{
    AlignmentSampleRef node(new AlignmentSample());
    node->setup();
    return node;
}


void AlignmentSample::setup() 
{
    //  Cinder method for key events
    ci::app::getWindow()->connectKeyDown(&AlignmentSample::keyDown, this);
    
    //  create and add instructions text box (using local variable)
    std::string instructions = "Press a number key to change alignment";
    std::shared_ptr<ci::TextBox> t(new ci::TextBox());
    t->font(ci::Font("Helvetica", 24))
        .text(instructions)
        .size(ci::Vec2f(250, ci::TextBox::GROW));
    TextBoxRef tbTop = TextBox::create(t);
    tbTop->setPosition(20, 20);
    addChild(tbTop);
    
    //  create and add text box that shows current alignment (using member variable)
    std::string currentAlignment = "NONE";
    std::shared_ptr<ci::TextBox> u(new ci::TextBox());
    u->font(ci::Font("Helvetica", 24))
        .text(currentAlignment)
        .size(ci::Vec2f(250, ci::TextBox::GROW));
    mText = TextBox::create(u);
    mText->setPosition(20, 220);
    addChild(mText);
    
    //  create and add the shape
    mShapeNode = Shape::createRect(200, 100);
    
    mShapeNode->setAlignment(po::scene::Alignment::NONE)
                .setFillColor(122.f/255, 201.f/255, 67.4/255)
                .setPosition(250, 250);
    addChild(mShapeNode);

    //  add a reference dot to indicate position of mShapeNode
    ShapeRef dot = Shape::createCircle(10);
    dot->setAlignment(po::scene::Alignment::CENTER_CENTER)
        .setFillColor(255.f/255, 123.f/255, 172.f/255)
        .setPosition(250, 250);
    addChild(dot);
}

void AlignmentSample::keyDown(ci::app::KeyEvent &event)
{
    std::cout << "Key down: " << event.getChar() << std::endl;
    //std::shared_ptr<ci::TextBox> internalTextBox = mText->getCiTextBox();
    
    std::stringstream ss;
    std::string s;
    ss<< event.getChar();
    ss>> s;
    //internalTextBox->setText(s);
    //mText->getCiTextBox()->text(s);
    
    std::cout << s << std::endl;
    
}