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

#include <deque>

#include "View.h"

namespace po { namespace scene {
    // ViewContainers are groups of Views. They do not render anything themselves,
    // but they should render all of their children, allow for grouped alpha,
    // and be hit testable. Hit testing on a View Container is true if any of it's
    // children return true to a pointInside query.
    //
    // View containers can also be use to generate a texture from a complex layout,
    // hide/show collections of objects, enable/disable interaction on a group level,
    // etc.
    //
    // Child Views are drawn in the order they are added, i.e. last View added will
    // appear on top. These positions are their indices.
    
	//	Create ViewContainerRef typedef
	class ViewContainer;
	typedef std::shared_ptr<ViewContainer> ViewContainerRef;
	
	class ViewContainer
	: public View
	{
		friend class Scene;
		
	public:
        //! Create a ViewContainer
		static ViewContainerRef create(std::string name = "");
		
		~ViewContainer();
		
		//  Overrides function in View.h so we can tell between ViewContainers and other types of View
		virtual bool isRenderable (void) const override { return false; }
		
		//	Children
		static const int INVALID_INDEX = -1;
        
        // Add Children
        
        //! Add a View to this ViewContainer
        ViewContainer &addChild(ViewRef View);
        //! Add multiple children to this ViewContainer
        /** This method should be preferred when adding a large amount of children at the same time. 
            The View container needs to recalculate it's matrices every time we add a child (to update bounds)
            so using this only causes that to happen once vs n times**/
		ViewContainer &addChildren(std::vector<ViewRef> Views);
        //! Add a child at an index
		ViewContainer &addChildAt(int index, ViewRef View);
        //! Add a child before (below) another View
		ViewContainer &addChildBefore(ViewRef before, ViewRef View);
        //! Add a child after (above) another View
        ViewContainer &addChildAfter(ViewRef after, ViewRef View);
        
        // Get Children
        
        //! Get the total number of children for this ViewContainer
        size_t getNumChildren() { return mChildren.size(); };
        //! Get all this ViewContainer's children
		std::deque<ViewRef> getChildren();
        //! Get a reference to all of this ViewContainer's children
		std::deque<ViewRef> &getChildrenByReference();
        //! Find if the ViewContainer has any children
		bool hasChildren();
        //! Get a child at an index. Returns INVALID_INDEX if not found
		int getChildIndex(const ViewRef &child);
        //! Get a child by an index. Returns null_ptr if not found.
		ViewRef getChildByIndex(int index);
        //! Get a child by it's UID. Returns null_ptr if not found.
		ViewRef getChildByUID(uint32_t uid);
        //! Get a child by name. Returns null_ptr if not found.
		ViewRef getChildByName(const std::string &name);
        //! Get the first (bottom) child
		ViewRef getFirstChild();
        //! Get the last (top) child
		ViewRef getLastChild();
        
        // Remove children
        
        //! Remove a child by View reference
		ViewContainer &removeChild(ViewRef View);
        //! Remove a child at a specific index
		ViewContainer &removeChildAt(int index);
        //! Remove all the children from this ViewContainer.
		ViewContainer &removeAllChildren();
        
        // Move Child Views
        
        //! Move a child to the front (top)
		ViewContainer &moveChildToFront(ViewRef View);
        //! Move a child forward one index
		ViewContainer &moveChildForward( ViewRef View);
        //! Move a child to after (in front of) a View
		ViewContainer &moveChildAfter(ViewRef after, ViewRef View) { return addChildAfter(after, View); };
        //! Move a child to back (bottom)
		ViewContainer &moveChildToBack(ViewRef View);
        //! Move a child backward one index
		ViewContainer &moveChildBackward(ViewRef View);
        //! Move a child to before (behind) another View
		ViewContainer &moveChildBefore(ViewRef before, ViewRef View) { return addChildBefore(before, View); };
		
		//	Bounds
        
        //! Get the bounds
		virtual ci::Rectf getBounds();

		//  Interaction
        //! Determine if a point is inside of any of this ViewContainer's children
		virtual bool pointInside(const ci::vec2 &windowPoint);
		
	protected:
		virtual void draw();
		
		ViewContainer();
		ViewContainer(std::string name);

		void setScene(SceneRef scene);
		void removeScene();
		
	private:
		//  Set the parent to this container and the scene to this container's scene
		void setParentAndScene(ViewRef View);
		
		//  Update and Draw trees, traverse child Views
		virtual void updateTree();
		void matrixTree();
		
		//  Children
		std::deque<ViewRef> mChildren;
		
		//  Matrices
		void calculateMatrices();
		
	};
	
	//------------------------------------
	//  Exceptions
	//------------------------------------
	
	class ViewContainerException
	: public ci::Exception
	{
	};
	
	class ViewContainerInvalidChildException
	: public ViewContainerException
	{
	};
		
} } //  namespace po::scene