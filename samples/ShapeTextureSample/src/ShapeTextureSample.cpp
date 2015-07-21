#include "ShapeTextureSample.h"
#include "cinder/imageIo.h"

ShapeTextureSampleRef ShapeTextureSample::create() 
{
    ShapeTextureSampleRef node(new ShapeTextureSample());
    node->setup();
    return node;
}

ShapeTextureSample::ShapeTextureSample()
: mCurrentFitType(TextureFit::Type::NONE)
, mCurrentAlignment(Alignment::NONE)
{
}

void ShapeTextureSample::setup() 
{
    //  Cinder method for key events
    ci::app::getWindow()->getSignalKeyDown().connect(std::bind(&ShapeTextureSample::keyDown, this, std::placeholders::_1));
    
    createFitIndicators();
    activateFitIndicator(0);
    
    createAlignmentIndicators();
    activateAlignmentIndicator(0);
    
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
    mEllipseShape->setPosition(ci::vec2(xInterval + xOffset, yOffset));
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
    mTriangleShape->setPosition(ci::vec2((xInterval * 2) + xOffset, yOffset));
    mTriangleShape->setTexture(mTexture);
    mTriangleShape->setDrawBounds(true);
    addChild(mTriangleShape);
}

void ShapeTextureSample::keyDown(ci::app::KeyEvent &event)
{
    switch (event.getChar()) {
        case '0':
            mCurrentFitType = TextureFit::Type::NONE;
            activateFitIndicator(0);
            break;
            
        case '1':
            mCurrentFitType = TextureFit::Type::EXACT;
            activateFitIndicator(1);
            break;
            
        case '2':
            mCurrentFitType = TextureFit::Type::WIDTH;
            activateFitIndicator(2);
            break;
            
        case '3':
            mCurrentFitType = TextureFit::Type::HEIGHT;
            activateFitIndicator(3);
            break;
            
        case '4':
            mCurrentFitType = TextureFit::Type::INSIDE;
            activateFitIndicator(4);
            break;
            
        case 'O':
        case 'o':
            mCurrentAlignment = Alignment::NONE;
            activateAlignmentIndicator(0);
            break;
            
        case 'Q':
        case 'q':
            mCurrentAlignment = Alignment::TOP_LEFT;
            activateAlignmentIndicator(1);
            break;
            
        case 'A':
        case 'a':
            mCurrentAlignment = Alignment::CENTER_LEFT;
            activateAlignmentIndicator(2);
            break;
            
        case 'Z':
        case 'z':
            mCurrentAlignment = Alignment::BOTTOM_LEFT;
            activateAlignmentIndicator(3);
            break;
            
        case 'W':
        case 'w':
            mCurrentAlignment = Alignment::TOP_CENTER;
            activateAlignmentIndicator(4);
            break;
            
        case 'S':
        case 's':
            mCurrentAlignment = Alignment::CENTER_CENTER;
            activateAlignmentIndicator(5);
            break;
            
        case 'X':
        case 'x':
            mCurrentAlignment = Alignment::BOTTOM_CENTER;
            activateAlignmentIndicator(6);
            break;
            
        case 'E':
        case 'e':
            mCurrentAlignment = Alignment::TOP_RIGHT;
            activateAlignmentIndicator(7);
            break;
            
        case 'D':
        case 'd':
            mCurrentAlignment = Alignment::CENTER_RIGHT;
            activateAlignmentIndicator(8);
            break;
            
        case 'C':
        case 'c':
            mCurrentAlignment = Alignment::BOTTOM_RIGHT;
            activateAlignmentIndicator(9);
            break;
            
        default:
            break;
    }
    
    //  reset all the shapes
    mRectShape->setTexture(mTexture, mCurrentFitType, mCurrentAlignment);
    mEllipseShape->setTexture(mTexture, mCurrentFitType, mCurrentAlignment);
    mTriangleShape->setTexture(mTexture, mCurrentFitType, mCurrentAlignment);
    
}

void ShapeTextureSample::createFitIndicators()
{
    ci::TextBox labelText;
    labelText.text("Press 0-4 to change TextureFit:");
    labelText.color(ci::Color(1, 1, 1));
    labelText.size(85, ci::TextBox::GROW);
    
    TextBoxRef fitLabel = TextBox::create(labelText);
    fitLabel->setPosition(10, 15);
    addChild(fitLabel);
    
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
    mFitIndicatorContainer->setPosition(95, 10);
    
    //  Create and add indicators to the container
    //  Add them to a map to reference later
    for (int i = 0; i < mFitIndicatorNames.size(); i++) {
        std::string indicatorText = std::to_string(i) + ": " + mFitIndicatorNames[i];
        IndicatorRef indicator = Indicator::create(indicatorText);
        mFitIndicatorContainer->addChild(indicator);
        indicator->setPosition(0, i * (indicator->getHeight() + 5));
        mFitIndicators[mFitIndicatorNames[i]] = indicator;
    }
}

void ShapeTextureSample::createAlignmentIndicators()
{
    ci::TextBox labelText;
    labelText.text("Press letter grid to change Alignment:");
    labelText.color(ci::Color(1, 1, 1));
    labelText.size(85, ci::TextBox::GROW);
    
    TextBoxRef alignLabel = TextBox::create(labelText);
    alignLabel->setPosition(300, 15);
    addChild(alignLabel);
    
    mAlignIndicatorNames = {
        "O: NONE",
        "Q: TOP_LEFT",
        "A: CENTER_LEFT",
        "Z: BOTTOM_LEFT",
        "W: TOP_CENTER",
        "S: CENTER_CENTER",
        "X: BOTTOM_CENTER",
        "E: TOP_RIGHT",
        "D: CENTER_RIGHT",
        "C: BOTTOM_RIGHT"
    };
    
    //  Create a container to hold the indicators
    mAlignIndicatorContainer = NodeContainer::create();
    addChild(mAlignIndicatorContainer);
    mAlignIndicatorContainer->setPosition(390, 10);
    
    //  Create and add indicators to the container
    //  Add them to a map to reference later
    for (int i = 0; i < mAlignIndicatorNames.size(); i++) {
        std::string indicatorText = mAlignIndicatorNames[i];
        IndicatorRef indicator = Indicator::create(indicatorText);
        mAlignIndicatorContainer->addChild(indicator);
        if (i == 0) {
            indicator->setPosition((indicator->getWidth() * 3) + 20, i * (indicator->getHeight() + 5));
        } else if (i < 4) {
            indicator->setPosition(0, (i-1) * (indicator->getHeight() + 5));
        } else if (i < 7) {
            indicator->setPosition(indicator->getWidth(), (i-4) * (indicator->getHeight() + 5));
        } else {
            indicator->setPosition((indicator->getWidth() * 2) + 20, (i-7) * (indicator->getHeight() + 5));
        }
        
        mAlignIndicators[mAlignIndicatorNames[i]] = indicator;
    }
}

void ShapeTextureSample::activateFitIndicator(int num)
{
    for (int i = 0; i < mFitIndicatorNames.size(); i++) {
        if (i == num) {
            mFitIndicators[mFitIndicatorNames[i]]->showHighlighted();
        } else {
            mFitIndicators[mFitIndicatorNames[i]]->hideHighlighted();
        }
    }
}

void ShapeTextureSample::activateAlignmentIndicator(int num)
{
    for (int i = 0; i < mAlignIndicatorNames.size(); i++) {
        if (i == num) {
            mAlignIndicators[mAlignIndicatorNames[i]]->showHighlighted();
        } else {
            mAlignIndicators[mAlignIndicatorNames[i]]->hideHighlighted();
        }
    }
}