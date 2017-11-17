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

#include "cinder/gl/Texture.h"

namespace po
{
	namespace scene
	{
		namespace TextureFit
		{

			enum class Type {
				NONE = 0,
				EXACT,
				WIDTH,
				HEIGHT,
				INSIDE
			};

			static ci::vec2 alignInRect( ci::vec2 max, ci::Rectf rect, Alignment align )
			{
				ci::vec2 offset;

				switch( align ) {
					case Alignment::BOTTOM_LEFT:
						offset = ci::vec2( 0.f, max.y - 1.f );
						break;

					case Alignment::BOTTOM_CENTER:
						offset = ci::vec2( ( max.x - 1.f ) / 2.f, max.y - 1.f );
						break;

					case Alignment::BOTTOM_RIGHT:
						offset = ci::vec2( max.x - 1.f, max.y - 1.f );
						break;

					case Alignment::CENTER_LEFT:
						offset = ci::vec2( 0.f, ( max.y - 1.f ) / 2.f );
						break;

					case Alignment::CENTER_CENTER:
						offset = ci::vec2( ( max.x - 1.f ) / 2.f, ( max.y - 1.f ) / 2.f );
						break;

					case Alignment::CENTER_RIGHT:
						offset = ci::vec2( max.x - 1.f, ( max.y - 1.f ) / 2.f );
						break;

					case Alignment::TOP_LEFT:
						offset = ci::vec2( 0.f, 0.f );
						break;

					case Alignment::TOP_CENTER:
						offset = ci::vec2( ( max.x - 1.f ) / 2.f, 0.f );
						break;

					case Alignment::TOP_RIGHT:
						offset = ci::vec2( max.x - 1.f, 0.f );
						break;

					case Alignment::NONE:
						break;
				}

				return offset;
			}

			static void textureFitExact( ci::Rectf rect, ci::gl::TextureRef tex, Alignment align, const std::vector<ci::vec2>& points, std::vector<ci::vec2>& coords )
			{
				for( uint32_t i = 0; i < points.size(); i++ ) {
					float s = ( points[i].x - rect.getX1() ) / rect.getWidth();
					float t = ( points[i].y - rect.getY1() ) / rect.getHeight();
					coords[i] = ci::vec2( s, t );
				}
			}

			static void textureFitNone( ci::Rectf rect, ci::gl::TextureRef tex, Alignment align, const std::vector<ci::vec2>& points, std::vector<ci::vec2>& coords )
			{
				ci::vec2 max( FLT_MIN, FLT_MIN );

				for( uint32_t i = 0; i < points.size(); i++ ) {
					float s = ( points[i].x - rect.getX1() ) / tex->getWidth();
					float t = ( points[i].y - rect.getY1() ) / tex->getHeight();
					max.x = std::max( s, max.x );
					max.y = std::max( t, max.y );
					coords[i] = ci::vec2( s, t );
				}

				ci::vec2 offset = alignInRect( max, ci::Rectf( 0, 0, 1, 1 ), align );

				for( uint32_t i = 0; i < coords.size(); i++ ) {
					coords[i] -= offset;
				}
			}

			static void textureFitHorizontal( ci::Rectf rect, ci::gl::TextureRef tex, Alignment align, const std::vector<ci::vec2>& points, std::vector<ci::vec2>& coords )
			{
				float new_w = rect.getWidth();
				float new_h = new_w / ( tex->getWidth() / ( float )tex->getHeight() );

				ci::vec2 max( FLT_MIN, FLT_MIN );

				for( uint32_t i = 0; i < points.size(); i++ ) {
					float s = ( points[i].x - rect.getX1() ) / rect.getWidth();
					float t = ( points[i].y - rect.getY1() ) / new_h;
					max.x = std::max( s, max.x );
					max.y = std::max( t, max.y );
					coords[i] = ci::vec2( s, t );
				}

				ci::vec2 offset = alignInRect( max, ci::Rectf( 0, 0, 1, 1 ), align );

				for( uint32_t i = 0; i < coords.size(); i++ ) {
					coords[i] -= offset;
				}
			}

			static void textureFitVertical( ci::Rectf rect, ci::gl::TextureRef tex, Alignment align, const std::vector<ci::vec2>& points, std::vector<ci::vec2>& coords )
			{
				float new_h = rect.getHeight();
				float new_w = new_h / ( tex->getHeight() / ( float )tex->getWidth() );

				ci::vec2 max( FLT_MIN, FLT_MIN );

				for( uint32_t i = 0; i < points.size(); i++ ) {
					float s = ( points[i].x - rect.getX1() ) / new_w;
					float t = ( points[i].y - rect.getY1() ) / rect.getHeight();
					max.x = std::max( s, max.x );
					max.y = std::max( t, max.y );
					coords[i] = ci::vec2( s, t );
				}

				ci::vec2 offset = alignInRect( max, ci::Rectf( 0, 0, 1, 1 ), align );

				for( uint32_t i = 0; i < coords.size(); i++ ) {
					coords[i] -= offset;
				}
			}

			static void fitTexture( ci::Rectf rect, ci::gl::TextureRef tex, Type fit, Alignment align, const std::vector<ci::vec2>& points, std::vector<ci::vec2>& coords )
			{
				switch( fit ) {
					case Type::NONE:
						textureFitNone( rect, tex, align, points, coords );
						break;

					case Type::EXACT:
						textureFitExact( rect, tex, align, points, coords );
						break;

					case Type::WIDTH:
						textureFitHorizontal( rect, tex, align, points, coords );
						break;

					case Type::HEIGHT:
						textureFitVertical( rect, tex, align, points, coords );
						break;

					case Type::INSIDE: {
						float new_h = ( ( float )rect.getWidth() ) * ( ( float )tex->getHeight() ) / ( ( float )tex->getWidth() );

						if( new_h > rect.getHeight() ) {
							textureFitVertical( rect, tex, align, points, coords );
						}
						else {
							textureFitHorizontal( rect, tex, align, points, coords );
						}

						break;
					}

					default:
						break;
				}
			}

			static std::vector<ci::vec2> fitTexture( ci::Rectf rect, ci::gl::TextureRef tex, Type fit, Alignment align )
			{
				std::vector<ci::vec2> coords( 4 );
				std::vector<ci::vec2> points;
				points.push_back( rect.getUpperLeft() );
				points.push_back( rect.getUpperRight() );
				points.push_back( rect.getLowerRight() );
				points.push_back( rect.getLowerLeft() );

				fitTexture( rect, tex, fit, align, coords, points );
				return coords;
			}

		}
	}
} //  namespace po::scene::TextureFit