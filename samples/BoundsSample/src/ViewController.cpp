#include "ViewController.h"

#include "cinder/app/App.h"

using namespace po::scene;

namespace sample {
	float hueIncrement = 0.05f;

	float hue = 0.5f;
	float nodeContainerHue = hue + hueIncrement;
	float nodeHue = nodeContainerHue + hueIncrement;

	float brightness = 0.65f;
	float selectedBrightness = 1.0f;

	float saturation = 0.5f;

	ci::Color boundsColor(1.0f, 1.0f, 1.0f);

	float scaleIncrease = 0.1f;

	ViewControllerRef ViewController::create()
	{
		ViewControllerRef ref(new ViewController());
		ref->setup();
		return ref;
	}

	void ViewController::setup()
	{
		// Set up our info text
		mInfoText.size(200, ci::TextBox::GROW)
			.color(ci::Color(1, 1, 1))
			.alignment(ci::TextBox::Alignment::RIGHT)
			.font(ci::Font("Arial", 11));

		// Scene BG
		mBG = ShapeView::createRect(1, 1);
		mBG->setFillColor(ci::Color(ci::CM_HSV, hue, saturation, brightness))
			.setParentShouldIgnoreInBounds(true);
		getView()->addSubview(mBG);

		// Scene text box + window text box
		mWindowTextBox = TextView::create();
		mWindowTextBox->setPosition(-250, 0)
			.setParentShouldIgnoreInBounds(true);
		getView()->addSubview(mWindowTextBox);

		mTextView = TextView::create();
		mTextView->setPosition(5, 5)
			.setParentShouldIgnoreInBounds(true);
		getView()->addSubview(mTextView);

		// Node Container
		mContainerView = View::create("Node Container");
		mContainerView->setPosition(100, 150)
			.setDrawBounds(true)
			.setBoundsColor(boundsColor)
			.setName("Node Container");

		mContainerViewBG = ShapeView::createRect(1, 1);
		mContainerViewBG->setFillColor(ci::Color(ci::CM_HSV, nodeContainerHue, saturation, brightness))
			.setParentShouldIgnoreInBounds(true);

		mContainerTextView = TextView::create();

		mContainerTextView->setPosition(5, 5)
			.setParentShouldIgnoreInBounds(true);

		mContainerView->addSubview(mContainerViewBG)
			.addSubview(mContainerTextView)
			.getSignal(MouseEvent::Type::DOWN_INSIDE).connect(std::bind(&ViewController::nodeMouseOver, this, std::placeholders::_1));


		// Add some shapes
		int yPos = 175;
		for (int i = 0; i < 3; i++) {
			ShapeViewRef s = ShapeView::createRect(100, 100);
			s->setPosition(i * 125, yPos)
				.setName("Selected Node")
				.setDrawBounds(true)
				.setBoundsColor(boundsColor)
				.setFillColor(ci::Color(ci::CM_HSV, nodeHue, saturation, brightness))
				.getSignal(MouseEvent::Type::DOWN_INSIDE).connect(std::bind(&ViewController::nodeMouseOver, this, std::placeholders::_1));
			mContainerView->addSubview(s);
		}

		getView()->addSubview(mContainerView);

		// Setup Scene (this class is the root node)
		getView()->setName("Scene (Scene Root Node)")
			.setPosition(ci::vec2(250, 50))
			.setDrawBounds(true)
			.setBoundsColor(boundsColor);

		ci::app::getWindow()->getSignalKeyDown().connect(std::bind(&ViewController::keyPressed, this, std::placeholders::_1));

		//    ShapeRef rect = Shape::createRect(400,400);
		//    setMask(rect);
	}

	void ViewController::update()
	{
		// Set the BGs
		mBG->setPosition(getView()->getBounds().getUpperLeft())
			.setScale(getView()->getSize());

		mContainerViewBG->setPosition(mContainerView->getBounds().getUpperLeft())
			.setScale(mContainerView->getSize());

		// Update the scene and node container text
		mInfoText.setText(getViewInfo(getView()));
		mTextView->setCiTextBox(mInfoText);


		mInfoText.setText(getViewInfo(mContainerView));
		mContainerTextView->setCiTextBox(mInfoText);

		// Update window text
		std::stringstream ss;
		ss << "Window Mouse Position: " << ci::app::App::get()->getMousePos() - ci::app::getWindow()->getPos();

		if (mSelectedView) {
			ss << "\n\n" << getViewInfo(mSelectedView)
				<< "\n---------------------------------"
				<< "\n To rotate press 'r'"
				<< "\n To adjust alignment press 'a'"
				<< "\n To adjust scale press +/-";
		}
		else {
			ss << "\n\n Click a node to select\nand view/change attributes...";
		}

		mInfoText.text(ss.str())
			.alignment(ci::TextBox::Alignment::RIGHT);
		mWindowTextBox->setCiTextBox(mInfoText);

		mInfoText.alignment(ci::TextBox::Alignment::LEFT);
	}

	void ViewController::nodeMouseOver(po::scene::MouseEvent &event)
	{
		if (mSelectedView)
		{
			if (mSelectedView == mContainerView) {
				mContainerViewBG->setFillColor(ci::Color(ci::CM_HSV, nodeContainerHue, saturation, brightness));
			}
			else {
				mSelectedView->setFillColor(ci::Color(ci::CM_HSV, nodeHue, saturation, brightness));
			}
		}

		mSelectedView = event.getSource();
		if (mSelectedView == mContainerView) {
			mContainerViewBG->setFillColor(ci::Color(ci::CM_HSV, nodeContainerHue, saturation, selectedBrightness));
		}
		else {
			mSelectedView->setFillColor(ci::Color(ci::CM_HSV, nodeHue, saturation, selectedBrightness));
		}
	}

	void ViewController::keyPressed(ci::app::KeyEvent &key)
	{
		if (!mSelectedView)
			return;

		switch (key.getChar()) {
		case 'r':
			mSelectedView->setRotation(mSelectedView->getRotation() + M_PI / 4.0f);
			break;
		case 'a': {
			int curAlignment = static_cast<int>(mSelectedView->getAlignment());
			curAlignment += 1;
			if (curAlignment > static_cast<int>(po::scene::Alignment::NONE)) {
				curAlignment = 0;
			}

			mSelectedView->setAlignment(static_cast<po::scene::Alignment>(curAlignment));
		}

		case '+':
			mSelectedView->setScale(mSelectedView->getScale() + ci::vec2(scaleIncrease, scaleIncrease));
			break;

		case '-':
			mSelectedView->setScale(mSelectedView->getScale() - ci::vec2(scaleIncrease, scaleIncrease));
			break;


		default:
			break;
		}
	}


	std::string ViewController::getViewInfo(po::scene::ViewRef node)
	{
		std::stringstream ss;
		ss << node->getName()
			<< "\n---------------------------------"
			<< "\nWindow Pos " << node->localToWindow(ci::vec2(0, 0))
			<< "\nScene Pos: " << node->localToScene(ci::vec2(0, 0))
			<< "\nPosition: " << node->getPosition()
			<< "\nLocal Mouse Position: " << node->windowToLocal(ci::app::App::get()->getMousePos() - ci::app::getWindow()->getPos())
			<< "\nBounds: " << node->getBounds()
			<< "\nFrame: " << node->getFrame();

		return ss.str();
	}
}