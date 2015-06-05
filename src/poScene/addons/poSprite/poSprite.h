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

#pragma once

#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/Json.h"
#include "cinder/Xml.h"
#include "Resources.h"
#include "poNode.h"
#include "poSpritesheet.h"
#include "poSpritesheetAnimation.h"

namespace po { namespace scene {
	
	class Sprite;
	typedef std::shared_ptr<Sprite> SpriteRef;
	
	class Sprite
	: public Node
	{
	public:
		struct SpriteDataJson {
			std::vector<ci::gl::TextureRef> textures;
			std::vector<ci::JsonTree>       jsonFiles;
		};
		
		struct SpriteDataXml {
			std::vector<ci::gl::TextureRef> textures;
			std::vector<ci::XmlTree>        xmlFiles;
		};
		
		//	using single-texture spritesheets
		static SpriteRef create(ci::gl::TextureRef texture, ci::JsonTree json);
		static SpriteRef create(ci::gl::TextureRef texture, ci::XmlTree xml);
		
		//	using Multipacked texture spritesheets
		static SpriteRef create(std::vector<ci::gl::TextureRef> &textures, std::vector<ci::JsonTree> &data);
		static SpriteRef create(std::vector<ci::gl::TextureRef> &textures, std::vector<ci::XmlTree> &data);
		
		//	using a spritesheet
		static SpriteRef create(po::SpritesheetRef spriteSheet);
		
		//  using spritedata object
		static SpriteRef create(SpriteDataJson &spriteData);
		static SpriteRef create(SpriteDataXml &spriteData);
		
		po::SpritesheetRef &getSpritesheet() { return mSpritesheet; }
		po::SpritesheetAnimationRef &getAnimation() { return mSpritesheetAnimation; }
		ci::Rectf getBounds() { return mSpritesheet->getOriginalBounds(); }
		void setReplacementColor(ci::Color color);
		
		bool pointInside(const ci::Vec2f &point, bool localize = true);
		
	protected:
		Sprite();
		
		void setup(ci::gl::TextureRef texture, ci::JsonTree json);
		void setup(ci::gl::TextureRef texture, ci::XmlTree xml);
		
		void setup(std::vector<ci::gl::TextureRef> &textures, std::vector<ci::JsonTree> &data);
		void setup(std::vector<ci::gl::TextureRef> &textures, std::vector<ci::XmlTree> &data);
		void setup(po::SpritesheetRef spriteSheet);
		void update();
		void draw();
		
	private:
		ci::gl::GlslProgRef mShader;
		bool mIsKeyShaderEnabled;
		ci::Color mReplacementColor;
		
		po::SpritesheetRef mSpritesheet;
		po::SpritesheetAnimationRef mSpritesheetAnimation;
		
		void setupAnimation();
		
	};
	
} } //	namespace po::scene
