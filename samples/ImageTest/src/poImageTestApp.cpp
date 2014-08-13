//
//  poImageTestApp.cpp
//  ImageTest
//
//  Created by Stephen Varga on 3/30/14.
//
//

#include "cinder/gl/Texture.h"

#include "poImageTestApp.h"

#include "poTextureFit.h"
#include "poShape.h"

poImageTestAppRef poImageTestApp::create() {
    poImageTestAppRef app(new poImageTestApp());
    app->setup();
    return app;
}


void poImageTestApp::setup() {
    ci::gl::TextureRef texture = ci::gl::Texture::create(ci::loadImage("../../../resources/bubbles.jpg"));

    std::vector<po::NodeRef> images;
    float curTime = ci::app::getElapsedSeconds();
    for(int i=0; i<1; i++) {
        po::ShapeRef imageShape = po::Shape::create(texture);
        imageShape->setTexture(texture);
        imageShape->setTextureOffset(ci::Vec2f(-imageShape->getWidth()/2, 0.f));
        //imageShape->setScale(0.25f, 0.25f);
        images.push_back(imageShape);
    }
    
    addChildren(images);
    
    std::cout << ci::app::getElapsedSeconds() - curTime;
    
//    imageShape->alignment(po::Alignment::CENTER_CENTER)
//                .rotation(45);
}


