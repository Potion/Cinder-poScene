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

#include "poNode.h"

namespace po { namespace scene {
    // NodeContainers are groups of Nodes. They do not render anything themselves,
    // but they should render all of their children, allow for grouped alpha,
    // and be hit testable. Hit testing on a Node Container is true if any of it's
    // children return true to a pointInside query.
    //
    // Node containers can also be use to generate a texture from a complex layout,
    // hide/show collections of objects, enable/disable interaction on a group level,
    // etc.
    //
    // Child nodes are drawn in the order they are added, i.e. last node added will
    // appear on top. These positions are their indices.
    
	//	Create NodeContainerRef typedef
	class NodeContainer;
	typedef std::shared_ptr<NodeContainer> NodeContainerRef;
	
	class NodeContainer
	: public Node
	{
		friend class Scene;
		
	public:
        //! Create a NodeContainer
		static NodeContainerRef create(std::string name = "");
		
		~NodeContainer();
		
		//  Overrides function in Node.h so we can tell between NodeContainers and other types of Node
		virtual bool isRenderable (void) const override { return false; }
		
		//	Children
		static const int INVALID_INDEX = -1;
        
        // Add Children
        
        //! Add a Node to this NodeContainer
        NodeContainer &addChild(NodeRef node);
        //! Add multiple children to this NodeContainer
        /** This method should be preferred when adding a large amount of children at the same time. 
            The node container needs to recalculate it's matrices every time we add a child (to update bounds)
            so using this only causes that to happen once vs n times**/
		NodeContainer &addChildren(std::vector<NodeRef> nodes);
        //! Add a child at an index
		NodeContainer &addChildAt(int index, NodeRef node);
        //! Add a child before (below) another node
		NodeContainer &addChildBefore(NodeRef before, NodeRef node);
        //! Add a child after (above) another node
        NodeContainer &addChildAfter(NodeRef after, NodeRef node);
        
        // Get Children
        
        //! Get the total number of children for this NodeContainer
        size_t getNumChildren() { return mChildren.size(); };
        //! Get all this NodeContainer's children
		std::deque<NodeRef> getChildren();
        //! Get a reference to all of this NodeContainer's children
		std::deque<NodeRef> &getChildrenByReference();
        //! Find if the NodeContainer has any children
		bool hasChildren();
        //! Get a child at an index. Returns INVALID_INDEX if not found
		int getChildIndex(const NodeRef &child);
        //! Get a child by an index. Returns null_ptr if not found.
		NodeRef getChildByIndex(int index);
        //! Get a child by it's UID. Returns null_ptr if not found.
		NodeRef getChildByUID(uint32_t uid);
        //! Get a child by name. Returns null_ptr if not found.
		NodeRef getChildByName(const std::string &name);
        //! Get the first (bottom) child
		NodeRef getFirstChild();
        //! Get the last (top) child
		NodeRef getLastChild();
        
        // Remove children
        
        //! Remove a child by node reference
		NodeContainer &removeChild(NodeRef node);
        //! Remove a child at a specific index
		NodeContainer &removeChildAt(int index);
        //! Remove all the children from this NodeContainer.
		NodeContainer &removeAllChildren();
        
        // Move Child nodes
        
        //! Move a child to the front (top)
		NodeContainer &moveChildToFront(NodeRef node);
        //! Move a child forward one index
		NodeContainer &moveChildForward( NodeRef node);
        //! Move a child to after (in front of) a node
		NodeContainer &moveChildAfter(NodeRef after, NodeRef node) { return addChildAfter(after, node); };
        //! Move a child to back (bottom)
		NodeContainer &moveChildToBack(NodeRef node);
        //! Move a child backward one index
		NodeContainer &moveChildBackward(NodeRef node);
        //! Move a child to before (behind) another node
		NodeContainer &moveChildBefore(NodeRef before, NodeRef node) { return addChildBefore(before, node); };
		
		//	Bounds
        
        //! Get the bounds
		virtual ci::Rectf getBounds();

		//  Interaction
        //! Determine if a point is inside of any of this NodeContainer's children
		virtual bool pointInside(const ci::vec2 &windowPoint);
		
	protected:
		virtual void draw();
		
		NodeContainer();
		NodeContainer(std::string name);

		void setScene(SceneRef scene);
		void removeScene();
		
	private:
		//  Set the parent to this container and the scene to this container's scene
		void setParentAndScene(NodeRef node);
		
		//  Update and Draw trees, traverse child nodes
		virtual void updateTree();
		void matrixTree();
		
		//  Children
		std::deque<NodeRef> mChildren;
		
		//  Matrices
		void calculateMatrices();
		
	};
	
	//------------------------------------
	//  Exceptions
	//------------------------------------
	
	class NodeContainerException
	: public ci::Exception
	{
	};
	
	class NodeContainerInvalidChildException
	: public NodeContainerException
	{
	};
		
} } //  namespace po::scene