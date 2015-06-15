#include "TextureSample.h"

//	photo credit: <a href="https://www.flickr.com/photos/dcoetzee/3565607295/">Black and white kitten with baseball toy


using namespace po::scene;

TextureSampleRef TextureSample::create() 
{
    TextureSampleRef node(new TextureSample());
    node->setup();
    return node;
}

TextureSample::TextureSample()
{
}

void TextureSample::setup() 
{
    //  Cinder method for key events
    ci::app::getWindow()->connectKeyDown(&TextureSample::keyDown, this);
    
    //  add text
    std::string legend = "TextureFit\n\n0: NONE\n1: EXACT\n2: WIDTH\n3: HEIGHT\n4: INSIDE";
    ci::TextBox ciTextbox = ci::TextBox();
    ciTextbox.text(legend);
    
    TextBoxRef poTextBox = TextBox::create(ciTextbox);
    poTextBox->setPosition(20, 20);
    addChild(poTextBox);

    std::string label = "Current TextureFit: NONE";
    ci::TextBox ciTextbox2 = ci::TextBox();
    ciTextbox2.text(label);

    mTexturefitLabel = TextBox::create(ciTextbox2);
    mTexturefitLabel->setPosition(20, 700);
    addChild(mTexturefitLabel);
    
    
    //  add shapes
    float shapeWidth = 400;
    float shapeHeight = 600;
    float centerX = (ci::app::getWindowWidth() - shapeWidth) / 2;
    float centerY = (ci::app::getWindowHeight() - shapeHeight) / 2;
    
    mPlainShape = Shape::createRect(400, 600);
    mPlainShape->setFillColor(ci::Color (122.f/255, 201.f/255, 67.f/255));
    mPlainShape->setPosition(ci::Vec2f(centerX, centerY));
    addChild(mPlainShape);

    
    mTexture = ci::gl::Texture::create(ci::loadImage(ci::app::loadAsset("kitten.jpg")));
    mTexture->setWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
    
    mTexShape = Shape::createEllipse(400, 600);
    mTexShape->setPosition(ci::Vec2f(centerX, centerY));
    mTexShape->setTexture(mTexture);
    addChild(mTexShape);
}

void TextureSample::keyDown(ci::app::KeyEvent &event)
{
    std::cout << "Key event: " << event.getChar() << std::endl;
    
    std::string labelText = "";
    
    switch (event.getChar()) {
        case '0':
            labelText = "NONE";
            mTexShape->setTexture(mTexture, TextureFit::Type::NONE, Alignment::NONE);
            break;
            
        case '1':
            labelText = "EXACT";
            mTexShape->setTexture(mTexture, TextureFit::Type::EXACT, Alignment::TOP_LEFT);
            break;
            
        case '2':
            labelText = "WIDTH";
            mTexShape->setTexture(mTexture, TextureFit::Type::WIDTH, Alignment::TOP_LEFT);
            break;
            
        case '3':
            labelText = "HEIGHT";
            mTexShape->setTexture(mTexture, TextureFit::Type::HEIGHT, Alignment::TOP_LEFT);
            break;
            
        case '4':
            labelText = "INSIDE";
            mTexShape->setTexture(mTexture, TextureFit::Type::INSIDE, Alignment::TOP_LEFT);
            break;
            
        default:
            break;
    }
    
    if (labelText != "") {
        ci::TextBox textBox = mTexturefitLabel->getCiTextBoxCopy();
        textBox.text("Current TextureFit: " + labelText);
        mTexturefitLabel->setCiTextBox(textBox);
    }
    
    
}
