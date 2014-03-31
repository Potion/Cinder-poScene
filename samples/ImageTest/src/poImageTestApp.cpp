//
//  poImageTestApp.cpp
//  ImageTest
//
//  Created by Stephen Varga on 3/30/14.
//
//

#include "cinder/gl/Texture.h"

#include "poImageTestApp.h"

#include "poShape.h"

poImageTestAppRef poImageTestApp::create() {
    poImageTestAppRef app(new poImageTestApp());
    app->setup();
    return app;
}


void poImageTestApp::setup() {
    ci::gl::TextureRef texture = ci::gl::Texture::create(ci::loadImage("../../../resources/test.jpg"));
    
    po::ShapeRef imageShape = po::Shape::create(texture);
    imageShape->setScale(0.25f, 0.25f);
    addChild(imageShape);
}


