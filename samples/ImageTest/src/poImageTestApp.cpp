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
    
    po::ShapeRef imageShape = po::Shape::createRect(100, 200);
    imageShape->setTexture(texture, po::TextureFit::HEIGHT);
    //imageShape->setScale(0.25f, 0.25f);
    addChild(imageShape);
    
//    imageShape->alignment(po::Alignment::CENTER_CENTER)
//                .rotation(45);
}


