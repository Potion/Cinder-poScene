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
    
    ci::TextBox t;
    t.font(ci::Font("Helvetica", 24))
        .text(instructions)
        .size(ci::Vec2f(160, ci::TextBox::GROW));
    TextBoxRef tbTop = TextBox::create(t);
    tbTop->setPosition(20, 20);
    addChild(tbTop);
    
    //  create and add legend text box (using local variables)
    std::string legendText = "1: TOP_LEFT\t\t4: CENTER_LEFT\t\t7: BOTTOM_LEFT\n2: TOP_CENTER\t5: CENTER_CENTER\t8: BOTTOM_CENTER\n3: TOP_RIGHT\t\t6: CENTER_RIGHT\t\t9: BOTTOM_RIGHT\n0: NONE";
    ci::TextBox legend;
    legend.text(legendText)
    .size(ci::Vec2f(400, ci::TextBox::GROW));
    TextBoxRef tbLegend = TextBox::create(legend);
    tbLegend->setPosition(ci::Vec2f(200, 20));
    addChild(tbLegend);
    
    //  create and add text box that shows current alignment (using member variable)
    std::string currentAlignment = "Alignment: NONE";
    ci::TextBox u;
    u.font(ci::Font("Helvetica", 24))
        .text(currentAlignment)
        .size(ci::Vec2f(350, 25));
    mTextBottom = TextBox::create(u);
    mTextBottom->setAlignment(po::scene::Alignment::BOTTOM_LEFT)
                .setPosition(20, 460);
    addChild(mTextBottom);
    
    
    ci::Vec2f center(320, 240);
    
    //  create and add the shape
    mShapeNode = Shape::createRect(200, 100);
    mShapeNode->setAlignment(po::scene::Alignment::NONE)
                .setFillColor(122.f/255, 201.f/255, 67.4/255)
                .setPosition(center);
    addChild(mShapeNode);

    //  create and add a reference dot to indicate position of mShapeNode
    ShapeRef dot = Shape::createCircle(10);
    dot->setAlignment(po::scene::Alignment::CENTER_CENTER)
        .setFillColor(255.f/255, 123.f/255, 172.f/255)
        .setPosition(center);
    addChild(dot);
}

void AlignmentSample::keyDown(ci::app::KeyEvent &event)
{
    std::string labelText = "";
    switch (event.getChar()) {
        case '0':
            labelText = "NONE";
            mShapeNode->setAlignment(po::scene::Alignment::NONE);
            break;

        case '1':
            labelText = "TOP_LEFT";
            mShapeNode->setAlignment(po::scene::Alignment::TOP_LEFT);
            break;
            
        case '2':
            labelText = "TOP_CENTER";
            mShapeNode->setAlignment(po::scene::Alignment::TOP_CENTER);
            break;
            
        case '3':
            labelText = "TOP_RIGHT";
            mShapeNode->setAlignment(po::scene::Alignment::TOP_RIGHT);
            break;
            
        case '4':
            labelText = "CENTER_LEFT";
            mShapeNode->setAlignment(po::scene::Alignment::CENTER_LEFT);
            break;
            
        case '5':
            labelText = "CENTER_CENTER";
            mShapeNode->setAlignment(po::scene::Alignment::CENTER_CENTER);
            break;
            
        case '6':
            labelText = "CENTER_RIGHT";
            mShapeNode->setAlignment(po::scene::Alignment::CENTER_RIGHT);
            break;
            
        case '7':
            labelText = "BOTTOM_LEFT";
            mShapeNode->setAlignment(po::scene::Alignment::BOTTOM_LEFT);
            break;
            
        case '8':
            labelText = "BOTTOM_CENTER";
            mShapeNode->setAlignment(po::scene::Alignment::BOTTOM_CENTER);
            break;
            
        case '9':
            labelText = "BOTTOM_RIGHT";
            mShapeNode->setAlignment(po::scene::Alignment::BOTTOM_RIGHT);
            break;
            
        default:
            break;
    }
    
    if (labelText != "") {
        ci::TextBox textBox = mTextBottom->getCiTextBoxCopy();
        textBox.text("Alignment: " + labelText);
        mTextBottom->setCiTextBox(textBox);
    }
}