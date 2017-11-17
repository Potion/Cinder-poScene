#include "ViewController.h"

#include "cinder/app/App.h"

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
		//	Create the text component
		mTextComponent = TextComponent::create();
		getView()->addSubview(mTextComponent);
		mTextComponent->setPosition(20, 20);

		//	Create the scroller and align it to the right of the text component
		mScroller = Scroller::create();
		getView()->addSubview(mScroller);
		mScroller->setPosition(mTextComponent->getWidth() + mTextComponent->getPosition().x + 20, mTextComponent->getPosition().y);
	}

	void ViewController::update()
	{
		//	Calculate the position of the text based on the scroll thumb postion
		float textPos = ci::lmap<float>(mScroller->getThumbPosition().y, 0, 340, -20, mTextComponent->getHeight() - ci::app::getWindowHeight() + 20);

		//	Animate text component to the new position
		ci::app::timeline().apply(&mTextComponent->getPositionAnim(), ci::vec2(mTextComponent->getPosition().x, -textPos), 0.2f);
	}
}