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
		
	void MatrixSet::set(glm::mat4x4 modelview, glm::mat4x4 projection, ci::Area viewport)
	{
		mModelview = modelview;
		mProjection = projection;
		mViewport = viewport;
	}
	
	ci::vec2 MatrixSet::globalToLocal(const ci::vec2 &point)
	{
		ci::vec3 p(point.x, mViewport.getHeight() - point.y, 0.f);
		ci::vec3 r = unproject(p);
		return ci::vec2(r.x, r.y);
	}
	
	ci::vec2 MatrixSet::localToGlobal(const ci::vec2 &point)
	{
		glm::mat4x4 a = mProjection * mModelview;
        a = glm::inverse(a);
		ci::vec3 p = project(ci::vec3(point.x, point.y, 0.f));
		return ci::vec2(p.x, p.y);
	}
	
	//
	//	Reimplementation of glm::Project
	//	Based on original function at https://github.com/g-truc/glm/blob/0.9.5/glm/gtc/matrix_transform.inl
	//
	ci::vec3 MatrixSet::project(const ci::vec3 &pt)
	{
        glm::vec4 p(pt.x, pt.y, 0, 1.f);
		p = mModelview * p;
		p = mProjection * p;
		p /= p.w;
        
        p = p * glm::vec4(0.5, 0.5, 0.5, 0.5) + glm::vec4(0.5, 0.5f, 0.f, 1.f);
		p.x = p.x * mViewport.getWidth();
		p.y = mViewport.getHeight() - (p.y * mViewport.getHeight());
		
		return ci::vec3(p.x, p.y, p.z);
	}
	
	//
	//	Adapted from code by Paul Houx https://forum.libcinder.org/topic/glu-s-gluunproject-substitute
	//
	ci::vec3 MatrixSet::unproject(const ci::vec3 &pt)
	{
		//	find the inverse modelview-projection-matrix
		glm::mat4x4 a = mProjection * mModelview;
        a = glm::inverse(a);
		
		//	transform to normalized coordinates in the range [-1, 1]
        glm::vec4 in;
		in.x = (pt.x - mViewport.getX1()) / mViewport.getWidth() * 2.0f - 1.0f;
		in.y = (pt.y - mViewport.getY1()) / mViewport.getHeight() * 2.0f - 1.0f;
		in.z = 2.0f * pt.z - 1.0f;
		in.w = 1.0f;
		
		//	find the object's coordinates
        glm::vec4 out = a * in;
		if (out.w != 0.0f) out.w = 1.0f / out.w;
		
		//	calculate output
		ci::vec3 result;
		result.x = out.x * out.w;
		result.y = out.y * out.w;
		result.z = out.z * out.w;
		
		return result;
	}
		
} } //  namespace po::scene