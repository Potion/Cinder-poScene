//
//  poImageTestApp.cpp
//  ImageTest
//
//  Created by Stephen Varga on 3/30/14.
//
//

#include "cinder/gl/Texture.h"

#include "poImageTestApp.h"
#include "poImage.h"

using namespace po::scene;

poImageTestAppRef poImageTestApp::create() {
    poImageTestAppRef app(new poImageTestApp());
    app->setup();
    return app;
}


void poImageTestApp::setup() {
    ci::gl::TextureRef texture = ci::gl::Texture::create(ci::loadImage("../../../resources/bubbles.jpg"));

    
    ImageRef image = Image::create(texture);
    addChild(image);
    
    image->setScale(ci::Vec2f(0.5f, 2.0f));
}


