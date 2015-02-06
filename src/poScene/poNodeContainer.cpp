//
//  poNodeContainer.cpp
//  BasicScene
//
//  Created by Stephen Varga on 3/22/14.
//
//

#include "poNodeContainer.h"
#include "poScene.h"

namespace po {
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
    
    
    //------------------------------------------------------
    #pragma mark - Scene -
    
    void NodeContainer::setScene(SceneRef scene)
    {
        Node::setScene(scene);
        for(NodeRef &childNode : mChildren) {
            childNode->setScene(scene);
        }
    }
    
    void NodeContainer::removeScene()
    {
        Node::removeScene();
        for(NodeRef &childNode : mChildren) {
            childNode->removeScene();
        }
    }
    
    
    //------------------------------------------------------
    #pragma mark - Add Children -

    void NodeContainer::addChild(NodeRef node)
    {
        setParentAndScene(node);
        mChildren.push_back(node);
        setAlignment(getAlignment());
        calculateMatrices();
    }
    
    void NodeContainer::addChildren(std::vector<NodeRef> nodes) {
        for (auto &node : nodes) {
            setParentAndScene(node);
            mChildren.push_back(node);
        }
        
        setAlignment(getAlignment());
        calculateMatrices();
    }
    
    void NodeContainer::addChildAt(int index, NodeRef node)
    {
        setParentAndScene(node);
        mChildren.insert(mChildren.begin()+index, node);
        setAlignment(getAlignment());
        calculateMatrices();
    }
    
    void NodeContainer::addChildBefore(NodeRef before, NodeRef node)
    {
        setParentAndScene(node);
        mChildren.insert(mChildren.begin() + getChildIndex(before), node);
        setAlignment(getAlignment());
        calculateMatrices();
    }
    
    void NodeContainer::addChildAfter(NodeRef after, NodeRef node)
    {
        setParentAndScene(node);
        mChildren.insert(mChildren.begin() + getChildIndex(after)+1, node);
        setAlignment(getAlignment());
        calculateMatrices();
    }
    
    
    //------------------------------------------------------
    #pragma mark - Get Children -
    
    std::vector<NodeRef> NodeContainer::getChildren()
    {
        return mChildren;
    };
    
    std::vector<NodeRef>& NodeContainer::getChildrenByReference()
    {
        return mChildren;
    }
    
    bool NodeContainer::hasChildren()
    {
        return (mChildren.size() != 0);
    }
    
    int NodeContainer::getChildIndex(const NodeRef& child)
    {
        std::vector<NodeRef>::iterator iter = std::find(mChildren.begin(), mChildren.end(), child);
        if(iter != mChildren.end())
            return (int)std::distance(mChildren.begin(), iter);
        return INVALID_INDEX;
    }
    
    NodeRef NodeContainer::getChildByIndex(int index)
    {
        if(index < 0 || index >= mChildren.size())
            return NodeRef();
        return *(mChildren.begin() + index);
    }
    
    NodeRef NodeContainer::getChildByUID(uint32_t uid)
    {
        //Go through our tree to find any node with UID
        for(NodeRef& node : mChildren) {
            NodeContainerRef container = std::dynamic_pointer_cast<NodeContainer>(node);
            if(container) {
                NodeRef foundNode = container->getChildByUID(uid);
                if(foundNode) return foundNode;
            }
            
            else {
                if(node->getUID() == uid) return node;
            }
        }
        
        //See if it is us
        if(mUid == uid) return shared_from_this();
            
        //Not found
        return NodeRef();
    }
    
    NodeRef NodeContainer::getChildByName(const std::string &name)
    {
        for(NodeRef& node : mChildren) {
            if(node->getName() == name) return node;
        }
        
        return NodeRef();
    }
    
    NodeRef NodeContainer::getFirstChild()
    {
        if(mChildren.empty())
            return NodeRef();
        
        return mChildren.front();
    }
    
    NodeRef NodeContainer::getLastChild()
    {
        if(mChildren.empty())
            return NodeRef();
        
        return mChildren.back();
    }
    
    
    //------------------------------------------------------
    #pragma mark - Remove Children -
    
    void NodeContainer::removeChild(NodeRef node)
    {
        std::vector<NodeRef>::iterator iter = std::find(mChildren.begin(), mChildren.end(), node);
        if(iter != mChildren.end()) {
            (*iter)->removeParent();
            (*iter)->removeScene();
            
            #pragma message "This is not safe in recursion..."
            mChildren.erase(iter);
            
            setAlignment(getAlignment());
        } else {
            NodeContainerInvalidChildException exc;
            throw(exc);
        }
    }
    
    void NodeContainer::removeChildAt(int index)
    {
        if(index <= 0 || index >= mChildren.size()) {
            NodeContainerInvalidChildException exc;
            throw(exc);
        } else {
            mChildren[index]->removeParent();
            mChildren[index]->removeScene();
            
            mChildren.erase(mChildren.begin() + index);
            
            setAlignment(getAlignment());
        }
    }
    
    void NodeContainer::removeAllChildren()
    {
        for(NodeRef& node : mChildren) {
            node->removeParent();
            node->removeScene();
        }
        
        setAlignment(getAlignment());
        
        mChildren.clear();
    }
    
    
    //------------------------------------------------------
    #pragma mark - Move Children -
    
    void NodeContainer::moveChildToFront(NodeRef node)
    {
        removeChild(node);
        addChild(node);
    }
    
    void NodeContainer::moveChildForward(NodeRef node)
    {
        removeChild(node);
        #pragma message "Does this work with a vector, or would this be out of bounds?"
        int idx = getChildIndex(node);
        addChildAt(std::min(idx+1, getNumChildren()), node);
    }
    
    void NodeContainer::moveChildToBack(NodeRef node)
    {
        removeChild(node);
        addChildAt(0, node);
    }
    
    void NodeContainer::moveChildBackward(NodeRef node)
    {
        int idx = getChildIndex(node);
        removeChild(node);
        addChildAt(std::max(idx-1, 0), node);
    }
    
    void NodeContainer::setParentAndScene(NodeRef node)
    {
        //See if the node is already a child of another node.
        if(node->getParent())
            node->getParent()->removeChild(node);
        
        //Assign ourselves as the parent
        node->setParent(std::dynamic_pointer_cast<NodeContainer>(shared_from_this()));
        node->setScene(mScene.lock());
    }
    
    
    //------------------------------------------------------
    #pragma mark - Update and Draw -

    void NodeContainer::updateTree()
    {
        Node::updateTree();
        
        //We have to copy the children, because if any of the update loops remove children
        //The vector is screwed (invalidated)
        std::vector<po::NodeRef> children(mChildren);
        
        for(NodeRef &childNode : children)
            if(childNode->mVisible && childNode->hasParent()) childNode->updateTree();
    }

    
    void NodeContainer::draw()
    {
        for(NodeRef &childNode : mChildren) {
            childNode->drawTree();
        }
    }
    
    void NodeContainer::drawFbo()
    {
        Node::drawFbo();
    }
    
    //------------------------------------------------------
    #pragma mark - Matrix/Transform -
    
    void NodeContainer::matrixTree() { 
        beginDrawTree();
        
        for(NodeRef &childNode : mChildren) {
            childNode->matrixTree();
        }
        
        finishDrawTree();
    }

    
    
    //------------------------------------------------------
    #pragma mark - Dimensions -

    ci::Rectf NodeContainer::getBounds()
    {
        //Reset Bounds
        ci::Rectf bounds = ci::Rectf(0,0,0,0);
        
        for(NodeRef &childNode : mChildren) {
			if(childNode->mVisible)
				bounds.include(childNode->getFrame());
		}
            
        return bounds;
    }
    
    bool NodeContainer::pointInside(const ci::Vec2f &point, bool localize)
    {
        for (const NodeRef &node : mChildren) {
            if(!node->mVisible) continue;
            
            if(node->pointInside(point, localize)) {
                return true;
            }
        }
        
        return false;
    }
    
    void NodeContainer::calculateMatrices() {
        if(hasParent()) {
            getParent()->calculateMatrices();
        } else {
            for(NodeRef &childNode : mChildren) {
                childNode->matrixTree();
            }
        }
    }
}