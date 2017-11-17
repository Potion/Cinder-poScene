#include "ViewController.h"

#include "cinder/app/App.h"
#include "cinder/ImageIo.h"

using namespace po::scene;

namespace sample {
	ViewControllerRef ViewController::create()
	{
		ViewControllerRef ref(new ViewController());
		ref->setup();
		return ref;
	}

	ViewController::ViewController()
	{
	}

	void ViewController::setup()
	{
		std::string text = "Have secret plans intrigued by the shower hack up furballs, for kick up litter so loves cheeseburgers, for poop on grasses scamper. Leave hair everywhere stand in front of the computer screen.";

		//	Create the ci TextView
		ci::TextBox ciTextBox = ci::TextBox();
		ciTextBox.size(200, 100);
		ciTextBox.color(ci::Color(1, 1, 1));
		ciTextBox.text(text);
		ciTextBox.alignment(ci::TextBox::Alignment::CENTER);
		ciTextBox.font(ci::Font("Arial", 13));

		//	Create a text box that rotates
		TextViewRef rotatingText = TextView::create(ciTextBox);
		rotatingText->setAlignment(po::scene::Alignment::CENTER_CENTER)
			.setPosition(150, 150);
		getView()->addSubview(rotatingText);
		ci::app::timeline().apply(&rotatingText->getRotationAnim(), ci::toRadians(360.f), 10.0f).loop();

		//	Create a text box that scales up and down
		mScalingText = TextView::create(ciTextBox);
		mScalingText->setAlignment(po::scene::Alignment::CENTER_CENTER)
			.setPosition(150, 350);
		getView()->addSubview(mScalingText);
		animateScale();

		//	Create a text box that scrolls across the screen
		TextViewRef movingText = TextView::create(ciTextBox);
		movingText->setPosition(400, ci::app::getWindowHeight());
		getView()->addSubview(movingText);
		ci::app::timeline().apply(&movingText->getPositionAnim(), ci::vec2(movingText->getPosition().x, -movingText->getHeight()), 10.f)
			.easeFn(ci::EaseInSine())
			.loop();

		//	Load in the mask texture and create a mask shape
		ci::gl::TextureRef texture = ci::gl::Texture::create(ci::loadImage(ci::app::loadAsset("mask.jpg")));
		ShapeViewRef mask = ShapeView::create(texture);

		//	Create a container to hold the text box
		ViewRef textNode = View::create();
		getView()->addSubview(textNode);
		textNode->setPosition(400, 100);

		//	Create a text box to be masked and add it to the container
		TextViewRef maskedText = TextView::create(ciTextBox);
		textNode->addSubview(maskedText);

		//	Apply a mask to the container
		textNode->setMask(mask);

		//	Animate the mask from left to right
		ci::app::timeline().apply(&mask->getPositionAnim(), ci::vec2(-textNode->getWidth(), -textNode->getHeight()), ci::vec2(textNode->getWidth(), 0), 3.0f).loop();
	}

	//	Scale down and up
	void ViewController::animateScale()
	{
		ci::app::timeline().apply(&mScalingText->getScaleAnim(), ci::vec2(0.6f, 0.6f), 1.f);
		ci::app::timeline().appendTo(&mScalingText->getScaleAnim(), ci::vec2(1.2f, 1.2f), 1.f)
			.easeFn(ci::EaseInSine())
			.finishFn(std::bind(&ViewController::animateScale, this));
	}
}