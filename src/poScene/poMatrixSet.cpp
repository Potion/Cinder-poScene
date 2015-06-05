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

#include "cinder/Area.h"
#include "poMatrixSet.h"

#if defined( CINDER_MSW )
	#include <windows.h>
	#undef min
	#undef max
	#include <gl/gl.h>
#elif defined( CINDER_COCOA_TOUCH )
	#include <OpenGLES/ES1/gl.h>
	#include <OpenGLES/ES1/glext.h>
#elif defined( CINDER_MAC )
	#include <OpenGL/gl.h>
#endif

namespace po { namespace scene {
		
	void MatrixSet::set(ci::Matrix44f modelview, ci::Matrix44f projection, ci::Area viewport)
	{
		mModelview     = modelview;
		mProjection    = projection;
		mViewport      = viewport;
	}
	
	ci::Vec2f MatrixSet::globalToLocal(const ci::Vec2f &point)
	{
		ci::Vec3f p(point.x, mViewport.getHeight() - point.y, 0.f);
		ci::Vec3f r = unproject(p);
		return ci::Vec2f(r.x, r.y);
	}
	
	ci::Vec2f MatrixSet::localToGlobal(const ci::Vec2f &point)
	{
		ci::Matrix44f a = mProjection * mModelview;
		a.invert();
		ci::Vec3f p = project(ci::Vec3f(point.x, point.y, 0.f));
		return ci::Vec2f(p.x, p.y);
	}
	
	//Reimplementation of glm::Project
	//Based on original function at https://github.com/g-truc/glm/blob/0.9.5/glm/gtc/matrix_transform.inl
	ci::Vec3f MatrixSet::project(const ci::Vec3f &pt)
	{
		ci::Matrix44f a = mProjection * mModelview;
		ci::Vec4f p(pt.x, pt.y, 0, 1.f);
		p = mModelview * p;
		p = mProjection * p;
		p /= p.w;
		p = p * 0.5 + ci::Vec4f(0.5, 0.5f, 0.f, 1.f);
		p.x = p.x * mViewport.getWidth();
		p.y = mViewport.getHeight() - (p.y * mViewport.getHeight());
		
		return ci::Vec3f(p.x, p.y, p.z);
	}
	
	//Adapted from code by Paul Houx https://forum.libcinder.org/topic/glu-s-gluunproject-substitute
	ci::Vec3f MatrixSet::unproject(const ci::Vec3f &pt)
	{
		/* find the inverse modelview-projection-matrix */
		ci::Matrix44f a = mProjection * mModelview;
		a.invert(0.0f);
		
		/* transform to normalized coordinates in the range [-1, 1] */
		ci::Vec4f in;
		in.x = (pt.x - mViewport.getX1()) / mViewport.getWidth() * 2.0f - 1.0f;
		in.y = (pt.y - mViewport.getY1()) / mViewport.getHeight() * 2.0f - 1.0f;
		in.z = 2.0f * pt.z - 1.0f;
		in.w = 1.0f;
		
		/* find the object's coordinates */
		ci::Vec4f out = a * in;
		if(out.w != 0.0f) out.w = 1.0f / out.w;
		
		/* calculate output */
		ci::Vec3f result;
		result.x = out.x * out.w;
		result.y = out.y * out.w;
		result.z = out.z * out.w;
		
		return result;
	}
		
} } //  namespace po::scene