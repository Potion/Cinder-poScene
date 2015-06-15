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
    
    createIndicators();
    activateIndicator(0);
    
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

    mTexShape = Shape::createEllipse(400, 600);
    mTexShape->setPosition(ci::Vec2f(centerX, centerY));
    mTexShape->setTexture(mTexture);
    addChild(mTexShape);
}

void TextureSample::keyDown(ci::app::KeyEvent &event)
{
    //  convert char to int
    int selectedInt = event.getChar() - '0';
    if (selectedInt < 0 || selectedInt > mIndicatorNames.size()-1)
    {
        return;
    }
    
    switch (selectedInt) {
        case 0:
            mTexShape->setTexture(mTexture, TextureFit::Type::NONE, Alignment::NONE);
            break;
            
        case 1:
            mTexShape->setTexture(mTexture, TextureFit::Type::EXACT, Alignment::TOP_LEFT);
            break;
            
        case 2:
            mTexShape->setTexture(mTexture, TextureFit::Type::WIDTH, Alignment::TOP_LEFT);
            break;
            
        case 3:
            mTexShape->setTexture(mTexture, TextureFit::Type::HEIGHT, Alignment::TOP_LEFT);
            break;
            
        case 4:
            mTexShape->setTexture(mTexture, TextureFit::Type::INSIDE, Alignment::TOP_LEFT);
            break;
            
        default:
            break;
    }

    activateIndicator(selectedInt);
}

void TextureSample::createIndicators()
{
    //  Indicator names same as event names
    mIndicatorNames = {
        "NONE",
        "EXACT",
        "WIDTH",
        "HEIGHT",
        "INSIDE"
    };
    
    //  Create a container to hold the indicators
    mIndicatorContainer = NodeContainer::create();
    addChild(mIndicatorContainer);
    mIndicatorContainer->setPosition(20, 20);
    
    //  Create and add indicators to the container
    //  Add them to a map to reference later
    for (int i = 0; i < mIndicatorNames.size(); i++) {
        std::string indicatorText = std::to_string(i) + ": " + mIndicatorNames[i];
        IndicatorRef indicator = Indicator::create(indicatorText);
        mIndicatorContainer->addChild(indicator);
        indicator->setPosition(0, i * (indicator->getHeight() + 5));
        mIndicators[mIndicatorNames[i]] = indicator;
    }
}

void TextureSample::activateIndicator(int fit)
{
    for (int i = 0; i < mIndicatorNames.size(); i++) {
        if (i == fit) {
            mIndicators[mIndicatorNames[i]]->showHighlighted();
        } else {
            mIndicators[mIndicatorNames[i]]->hideHighlighted();
        }
    }
}