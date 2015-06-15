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
    
    createFitIndicators();
    activateFitIndicator(0);
    
    createAlignmentIndicators();
    //activateAlignmentIndicator(0);
    
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
    if (selectedInt < 0 || selectedInt > mFitIndicatorNames.size()-1)
    {
        return;
    }
    
    TextureFit::Type selectedFit = static_cast<TextureFit::Type>(selectedInt);
    mRectShape->setTexture(mTexture, selectedFit, Alignment::TOP_LEFT);
    mEllipseShape->setTexture(mTexture, selectedFit, Alignment::TOP_LEFT);
    mTriangleShape->setTexture(mTexture, selectedFit, Alignment::TOP_LEFT);
    activateFitIndicator(selectedInt);
}

void TextureSample::createFitIndicators()
{
    //  Indicator names same as TextureFit type names
    mFitIndicatorNames = {
        "NONE",
        "EXACT",
        "WIDTH",
        "HEIGHT",
        "INSIDE"
    };
    
    //  Create a container to hold the indicators
    mFitIndicatorContainer = NodeContainer::create();
    addChild(mFitIndicatorContainer);
    mFitIndicatorContainer->setPosition(10, 10);
    
    //  Create and add indicators to the container
    //  Add them to a map to reference later
    for (int i = 0; i < mFitIndicatorNames.size(); i++) {
        std::string indicatorText = std::to_string(i) + ": " + mFitIndicatorNames[i];
        IndicatorRef indicator = Indicator::create(indicatorText);
        mFitIndicatorContainer->addChild(indicator);
        indicator->setPosition(0, i * (indicator->getHeight() + 5));
        mIndicators[mFitIndicatorNames[i]] = indicator;
    }
}

void TextureSample::createAlignmentIndicators()
{
    mAlignIndicatorNames = {
        "O: NONE",
        "Q: TOP_LEFT",
        "A: CENTER_LEFT",
        "Z: BOTTOM_LEFT",
        "W: TOP_CENTER",
        "S: CENTER_CENTER",
        "X: BOTTOM_CENTER",
        "E: TOP_RIGHT",
        "F: CENTER_RIGHT",
        "C: BOTTOM_RIGHT"
    };

    //  Create a container to hold the indicators
    mAlignIndicatorContainer = NodeContainer::create();
    addChild(mAlignIndicatorContainer);
    mAlignIndicatorContainer->setPosition(200, 10);
    
    //  Create and add indicators to the container
    //  Add them to a map to reference later
    for (int i = 0; i < mAlignIndicatorNames.size(); i++) {
        std::string indicatorText = mAlignIndicatorNames[i];
        IndicatorRef indicator = Indicator::create(indicatorText);
        mAlignIndicatorContainer->addChild(indicator);
        if (i == 0) {
            indicator->setPosition(indicator->getWidth() * 3, i * (indicator->getHeight() + 5));
        } else if (i < 4) {
            indicator->setPosition(0, (i-1) * (indicator->getHeight() + 5));
        } else if (i < 7) {
            indicator->setPosition(indicator->getWidth(), (i-4) * (indicator->getHeight() + 5));
        } else {
            indicator->setPosition(indicator->getWidth() * 2, (i-7) * (indicator->getHeight() + 5));
        }
        
        mAlignIndicators[mAlignIndicatorNames[i]] = indicator;
    }
}

void TextureSample::activateFitIndicator(int num)
{
    for (int i = 0; i < mFitIndicatorNames.size(); i++) {
        if (i == num) {
            mIndicators[mFitIndicatorNames[i]]->showHighlighted();
        } else {
            mIndicators[mFitIndicatorNames[i]]->hideHighlighted();
        }
    }
}

void TextureSample::setFitAllImages()
{
    
}

void TextureSample::setAlignmentAllImages()
{
    
}