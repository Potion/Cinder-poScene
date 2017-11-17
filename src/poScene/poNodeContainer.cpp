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

#include "poNodeContainer.h"
#include "poScene.h"

namespace po { namespace scene {
	
    NodeContainerRef NodeContainer::create(std::string name)
    {
        return std::shared_ptr<NodeContainer>(new NodeContainer(name));
    }

    NodeContainer::NodeContainer(std::string name)
    : Node(name)
    {
    }
    
    NodeContainer::NodeContainer()
    : Node("")
    {
    }
    
    NodeContainer::~NodeContainer()
    {
        removeAllChildren();
    }
    
	
    //------------------------------------
    //  Scene
	//------------------------------------
    
    void NodeContainer::setScene(SceneRef scene)
    {
        Node::setScene(scene);
        for (NodeRef &childNode : mChildren) {
            childNode->setScene(scene);
        }
    }
    
    void NodeContainer::removeScene()
    {
        Node::removeScene();
        for (NodeRef &childNode : mChildren) {
            childNode->removeScene();
        }
    }
    
    
    //------------------------------------
    //  Children
	//------------------------------------
	
	//
    //  Add Children
	//
	
    NodeContainer &NodeContainer::addChild(NodeRef node)
    {
        setParentAndScene(node);
        mChildren.push_back(node);
        setAlignment(getAlignment());
        calculateMatrices();
        
        return *this;
    }
    
    NodeContainer &NodeContainer::addChildren(std::vector<NodeRef> nodes) {
        for (auto &node : nodes) {
            setParentAndScene(node);
            mChildren.push_back(node);
        }
        
        setAlignment(getAlignment());
        calculateMatrices();
        
        return *this;
    }
    
    NodeContainer &NodeContainer::addChildAt(int index, NodeRef node)
    {
        setParentAndScene(node);
        mChildren.insert(mChildren.begin() + index, node);
        setAlignment(getAlignment());
        calculateMatrices();
        
        return *this;
    }
    
    NodeContainer &NodeContainer::addChildBefore(NodeRef before, NodeRef node)
    {
        setParentAndScene(node);
        mChildren.insert(mChildren.begin() + getChildIndex(before), node);
        setAlignment(getAlignment());
        calculateMatrices();
        
        return *this;
    }
    
    NodeContainer &NodeContainer::addChildAfter(NodeRef after, NodeRef node)
    {
        setParentAndScene(node);
        mChildren.insert(mChildren.begin() + getChildIndex(after) + 1, node);
        setAlignment(getAlignment());
        calculateMatrices();
        
        return *this;
    }
	
	//
    //  Get Children
    //
	
    std::deque<NodeRef> NodeContainer::getChildren()
    {
        return mChildren;
    };
    
    std::deque<NodeRef> &NodeContainer::getChildrenByReference()
    {
        return mChildren;
    }
    
    bool NodeContainer::hasChildren()
    {
        return (mChildren.size() != 0);
    }
    
    int NodeContainer::getChildIndex(const NodeRef &child)
    {
        std::deque<NodeRef>::iterator iter = std::find(mChildren.begin(), mChildren.end(), child);
        if (iter != mChildren.end()) return (int)std::distance(mChildren.begin(), iter);
        return INVALID_INDEX;
    }
    
    NodeRef NodeContainer::getChildByIndex(int index)
    {
        if ( index < 0 || index >= mChildren.size() ) return NodeRef();
        return *(mChildren.begin() + index);
    }
    
    NodeRef NodeContainer::getChildByUID(uint32_t uid)
    {
        //	Go through our tree to find any node with UID
        for (NodeRef &node : mChildren) {
            NodeContainerRef container = std::dynamic_pointer_cast<NodeContainer>(node);
            if (container) {
                NodeRef foundNode = container->getChildByUID(uid);
                if (foundNode) return foundNode;
            } else {
                if (node->getUID() == uid) return node;
            }
        }
        
        //	See if it is us
        if (mUid == uid) return shared_from_this();
            
        //	Not found
        return NodeRef();
    }
    
    NodeRef NodeContainer::getChildByName(const std::string &name)
    {
        for (NodeRef& node : mChildren) {
            if (node->getName() == name) return node;
        }
        
        return NodeRef();
    }
    
    NodeRef NodeContainer::getFirstChild()
    {
        if (mChildren.empty()) return NodeRef();
        return mChildren.front();
    }
    
    NodeRef NodeContainer::getLastChild()
    {
        if (mChildren.empty()) return NodeRef();
        return mChildren.back();
    }
    
    //
    //  Remove Children
	//
    
    NodeContainer &NodeContainer::removeChild(NodeRef node)
    {
        std::deque<NodeRef>::iterator iter = std::find(mChildren.begin(), mChildren.end(), node);
        if (iter != mChildren.end()) {
            (*iter)->removeParent();
            (*iter)->removeScene();
            
            mChildren.erase(iter);
            
            setAlignment(getAlignment());
            
            return *this;
        } else {
            NodeContainerInvalidChildException exc;
            throw(exc);
        }
    }
    
    NodeContainer &NodeContainer::removeChildAt(int index)
    {
        if ( index <= 0 || index >= mChildren.size() ) {
            NodeContainerInvalidChildException exc;
            throw(exc);
        } else {
            mChildren[index]->removeParent();
            mChildren[index]->removeScene();
            
            mChildren.erase(mChildren.begin() + index);
            
            setAlignment(getAlignment());
            
            return *this;
        }
    }
    
    NodeContainer &NodeContainer::removeAllChildren()
    {
        for (NodeRef &node : mChildren) {
            node->removeParent();
            node->removeScene();
        }
        
        setAlignment(getAlignment());
        
        mChildren.clear();
        
        return *this;
    }
    
    //
    //  Move Children
	//
    
    NodeContainer &NodeContainer::moveChildToFront(NodeRef node)
    {
        auto nodeIter = std::find(mChildren.begin(), mChildren.end(), node);
        if (nodeIter != mChildren.end()) {
            mChildren.erase(nodeIter);
            mChildren.push_back(node);
        }
        
        return *this;
    }
    
    NodeContainer &NodeContainer::moveChildForward(NodeRef node)
    {
        auto nodeIter = std::find(mChildren.begin(), mChildren.end(), node);
        if ( nodeIter != mChildren.end() && *nodeIter != mChildren.back() ) {
            std::iter_swap(nodeIter, ++nodeIter);
        }
        
        return *this;
    }
    
    NodeContainer &NodeContainer::moveChildToBack(NodeRef node)
    {
        auto nodeIter = std::find(mChildren.begin(), mChildren.end(), node);
        if (nodeIter != mChildren.end()) {
            mChildren.erase(nodeIter);
            mChildren.push_front(node);
        }
        
        return *this;
    }
    
    NodeContainer &NodeContainer::moveChildBackward(NodeRef node)
    {
        auto nodeIter = std::find(mChildren.begin(), mChildren.end(), node);
        if ( nodeIter != mChildren.end() && *nodeIter != mChildren.front() ) {
            std::iter_swap(nodeIter, --nodeIter);
        }
        
        return *this;
    }
    
    void NodeContainer::setParentAndScene(NodeRef node)
    {
        //	See if the node is already a child of another node.
		if (node->getParent() != nullptr) {
			node->getParent()->removeChild(node);
		}
        
        //	Assign ourselves as the parent
        node->setParent(std::static_pointer_cast<NodeContainer>(shared_from_this()));
        node->setScene(mScene.lock());
    }
    
    
    //------------------------------------
    //  Update + Draw
	//------------------------------------

    void NodeContainer::updateTree()
    {
        Node::updateTree();
        
        //	We have to copy the children, because if any of the update loops remove children
        //	The vector is screwed (invalidated)
        std::deque<NodeRef> children(mChildren);
        
		for (NodeRef &childNode : children) {
            if (childNode->mVisible && childNode->hasParent()) childNode->updateTree();
		}
    }

    
    void NodeContainer::draw()
    {
        for (NodeRef &childNode : mChildren) {
            childNode->drawTree();
        }
    }

	
    //------------------------------------
    //  Dimensions
	//------------------------------------

    ci::Rectf NodeContainer::getBounds()
    {
        //	Reset Bounds
		ci::Rectf bounds = ci::Rectf(0, 0, 0, 0);
		
        for (NodeRef &childNode : mChildren) {
			if (childNode->mVisible && !childNode->getParentShouldIgnoreInBounds()) bounds.include(childNode->getFrame());
		}
            
        return bounds;
    }
	
    
    //------------------------------------
    //  Interaction
	//------------------------------------
	
    bool NodeContainer::pointInside(const ci::vec2 &windowPoint)
    {
        for (const NodeRef &node : mChildren) {
            if (!node->mVisible) continue;
            if (node->pointInside(windowPoint)) return true;
        }
        
        return false;
    }
	
    
    //------------------------------------
    //  Matrices/Dimensions
	//------------------------------------
    
    void NodeContainer::matrixTree() {
        beginDrawTree();
        
        for (NodeRef &childNode : mChildren) {
            childNode->matrixTree();
        }
        
        finishDrawTree();
    }
    
    void NodeContainer::calculateMatrices() {
        if (hasParent()) {
            getParent()->calculateMatrices();
        } else {
            matrixTree();
        }
    }
	
} } //  namespace po::scene