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

#include "cinder/Area.h"
#include "cinder/Matrix.h"
#include "cinder/Camera.h"

namespace po { namespace scene {
	// MatrixSet
    // Used to record the current Model View Projection Matrices
    // for nodes, then perform translations
    
	class MatrixSet
	{
	public:
        //! Set the matrix set
        void set(glm::mat4x4 modelview, glm::mat4x4 projection, ci::Area viewport);
		
        //! Convert a point from window space to local space
		ci::vec2 globalToLocal(const ci::vec2 &point);
        //! Convert a point from local space back to window space
		ci::vec2 localToGlobal(const ci::vec2 &point);
		
	private:
		glm::mat4x4 mModelview, mProjection;
		ci::Area mViewport;
		
		ci::vec3 project(const ci::vec3 &point);
		ci::vec3 unproject(const ci::vec3 &point);
		
	};
		
} } //  namespace po::scene
