#include "TextureSample.h"

//	photo credit: <a href="https://www.flickr.com/photos/iyoupapa/4878707809/">cat</a> via <a href="https://www.flickr.com/photos/iyoupapa/">iyoupapa</a> on Flickr <a href="https://creativecommons.org/licenses/by-sa/2.0/">(license)</a>


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
    
    mTexture = ci::gl::Texture::create(ci::loadImage(ci::app::loadAsset("cat.jpg")));
    mTexture->setWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
    
    float xInterval = ci::app::getWindowWidth() / 3.f;
    float shapeWidth = 340.f;
    float shapeHeight = 500.f;
    float xOffset = (xInterval - shapeWidth) / 2;
    float yOffset = ((ci::app::getWindowHeight() - shapeHeight) / 2);
    
    //  create and add shapes
    mRectShape = Shape::createRect(shapeWidth, shapeHeight);
    mRectShape->setPosition(xOffset, yOffset);
    mRectShape->setTexture(mTexture);
    mRectShape->setDrawBounds(true);
    addChild(mRectShape);

    mEllipseShape = Shape::createEllipse(shapeWidth, shapeHeight);
    mEllipseShape->setPosition(ci::Vec2f(xInterval + xOffset, yOffset));
    mEllipseShape->setTexture(mTexture);
    mEllipseShape->setDrawBounds(true);
    addChild(mEllipseShape);
    
    ci::Shape2d customShape;
    customShape.moveTo (shapeWidth / 2.f, 0);
    customShape.lineTo(shapeWidth, shapeHeight);
    customShape.lineTo(0, shapeHeight);
    customShape.close();
    
    mTriangleShape = Shape::create();
    mTriangleShape->setCiShape2d(customShape);
    mTriangleShape->setPosition(ci::Vec2f((xInterval * 2) + xOffset, yOffset));
    mTriangleShape->setTexture(mTexture);
    mTriangleShape->setDrawBounds(true);
    addChild(mTriangleShape);
}

void TextureSample::keyDown(ci::app::KeyEvent &event)
{
    //  convert char to int
    int selectedInt = event.getChar() - '0';
    if (selectedInt < 0 || selectedInt > mIndicatorNames.size()-1)
    {
        return;
    }
    
    TextureFit::Type selectedFit = static_cast<TextureFit::Type>(selectedInt);
    mRectShape->setTexture(mTexture, selectedFit, Alignment::TOP_LEFT);
    mEllipseShape->setTexture(mTexture, selectedFit, Alignment::TOP_LEFT);
    mTriangleShape->setTexture(mTexture, selectedFit, Alignment::TOP_LEFT);
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
    mIndicatorContainer->setPosition(10, 10);
    
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