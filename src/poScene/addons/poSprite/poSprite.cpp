/*
 Copyright (c) 2015, Potion Design LLC
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 * Neither the name of copyright holder nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "poSprite.h"
#include "poMatrixSet.h"

namespace po { namespace scene {
	
	//-----------------------------------
	//	Single texture
		#pragma mark - Single texture
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


	//---------------------------------------
	//	Multipacked textures
		#pragma mark - Multipacked textures
	//---------------------------------------

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
		#pragma mark - Spritesheet
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
		#pragma mark - Sprite Data
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
		#pragma mark - Sprite
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
		
		if (mIsKeyShaderEnabled) mShader->unbind();
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
		ci::Vec2f pos = localize ? windowToLocal(point) : point;
		return getSpritesheet()->getFrameBounds().contains(pos);
	}
	
} } //	namespace po::scene