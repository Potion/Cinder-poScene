//
//  Sprite.cpp
//  ForestFriends
//
//  Created by bruce on 9/11/14.
//
//

#include "poSprite.h"

namespace po {
    namespace scene {
        //-----------------------------------
        //	Single texture
        //-----------------------------------
        SpriteRef Sprite::create(ci::gl::TextureRef texture, ci::JsonTree json)
        {
            SpriteRef ref(new Sprite());
            ref->setup(texture, json);
            return ref;
        }
        
        SpriteRef Sprite::create(ci::gl::TextureRef texture, ci::XmlTree xml)
        {
            SpriteRef ref(new Sprite());
            ref->setup(texture, xml);
            return ref;
        }
        
        void Sprite::setup(ci::gl::TextureRef texture, ci::JsonTree json)
        {
            mSpritesheet = po::Spritesheet::create(texture, json);
            setupAnimation();
        }
        
        void Sprite::setup(ci::gl::TextureRef texture, ci::XmlTree xml)
        {
            mSpritesheet = po::Spritesheet::create(texture, xml);
            setupAnimation();
        }
        
        //-----------------------------------
        // Multipacked textures
        //-----------------------------------
        SpriteRef Sprite::create(std::vector<ci::gl::TextureRef> &textures, std::vector<ci::JsonTree> &data)
        {
            SpriteRef ref(new Sprite());
            ref->setup(textures, data);
            return ref;
        }
        
        SpriteRef Sprite::create(std::vector<ci::gl::TextureRef> &textures, std::vector<ci::XmlTree> &data)
        {
            SpriteRef ref(new Sprite());
            ref->setup(textures, data);
            return ref;
        }
        
        void Sprite::setup(std::vector<ci::gl::TextureRef> &textures, std::vector<ci::JsonTree> &data)
        {
            mSpritesheet = po::Spritesheet::create(textures, data);
            setupAnimation();
        }
        
        void Sprite::setup(std::vector<ci::gl::TextureRef> &textures, std::vector<ci::XmlTree> &data)
        {
            mSpritesheet = po::Spritesheet::create(textures, data);
            setupAnimation();
        }
        
        //-----------------------------------
        //	Spritesheet
        //-----------------------------------
        SpriteRef Sprite::create(po::SpritesheetRef spriteSheet)
        {
            SpriteRef ref(new Sprite());
            ref->setup(spriteSheet);
            return ref;
        }
        
        void Sprite::setup(po::SpritesheetRef spriteSheet)
        {
            mSpritesheet = spriteSheet;
            setupAnimation();
        }
        
        //-----------------------------------
        //	Sprite Data
        //-----------------------------------
        SpriteRef Sprite::create(SpriteDataJson &spriteData)
        {
            return create(spriteData.textures, spriteData.jsonFiles);
        }
        
        SpriteRef Sprite::create(SpriteDataXml &spriteData)
        {
            return create(spriteData.textures, spriteData.xmlFiles);
        }
        
        //-----------------------------------
        //	Sprite
        //-----------------------------------
        
        Sprite::Sprite()
        : mIsKeyShaderEnabled(false)
        {
        }
        
        //
        //	Create the spritesheet animation
        //
        void Sprite::setupAnimation()
        {
            mSpritesheetAnimation = po::SpritesheetAnimation::create(mSpritesheet);
        }
        
        //
        //	Update the spritesheet animation
        //
        void Sprite::update()
        {
            Node::update();
            mSpritesheetAnimation->update();
        }
        
        //
        //	Draw the spritesheet animation
        //
        void Sprite::draw()
        {		
            if (mIsKeyShaderEnabled) {
                mShader->bind();
    //			mShader->uniform("keyColor", Colors::KEY_COLOR);
                mShader->uniform("replacementColor", mReplacementColor);
                mShader->uniform("alpha", getAppliedAlpha());

            } else {
                ci::gl::color(ci::ColorA(getFillColor(), getAppliedAlpha()));
            }
            
            mSpritesheetAnimation->draw();
            
            if (mIsKeyShaderEnabled) {
                mShader->unbind();
            }
        }
        
        //
        //	Set the replacement color for the shader
        //
        void Sprite::setReplacementColor(ci::Color color)
        {
            mIsKeyShaderEnabled = true;
            mReplacementColor = color;
        }
        
        bool Sprite::pointInside(const ci::Vec2f &point, bool localize)
        {
            ci::Vec2f pos = localize ? globalToLocal(point) : point;
            return getSpritesheet()->getFrameBounds().contains(pos);
        }
    }
}