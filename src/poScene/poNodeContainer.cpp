//
//  poNodeContainer.cpp
//  BasicScene
//
//  Created by Stephen Varga on 3/22/14.
//
//

#include "poNodeContainer.h"

namespace po {
    NodeContainerRef NodeContainer::create()
    {
        return std::shared_ptr<NodeContainer>(new NodeContainer);
    }

    NodeContainer::NodeContainer()
    {
    }

    int NodeContainer::getNumChildren()
    {
        return children.size();
    }

    void NodeContainer::addChild(NodeRef node)
    {
        //See if the node is already a child of another node.
        if(node->getParent()) {
            node->getParent()->removeChild(node);
        }
        
        //Assign ourselves as the parent
        node->setParent(shared_from_this());
        
        //Track Node
        children.push_back(node);
    }

    bool NodeContainer::removeChild(NodeRef node)
    {
        std::vector<NodeRef>::iterator iter = std::find(children.begin(), children.end(), node);
        
        if(iter != children.end()) {
            //Remove reference to this node in child
            node->removeParent();
            
            //Erase node
            children.erase(iter);
            
            return true;
        }
        
        return false;
    }

    void NodeContainer::updateTree()
    {
        update();
        for(NodeRef node : children)
            node->updateTree();
    }

    void NodeContainer::drawTree()
    {
        #pragma message "Need to implement matrix order"
        ci::gl::pushMatrices();
        setTransformation();
        
        for(NodeRef node : children)
            node->drawTree();
        
        if(bDrawBounds || bDrawFrame) {
            if(bDrawBounds)
                drawBounds();
            
            if(bDrawFrame)
                drawFrame();
        }
        
        ci::gl::popMatrices();
    }

    ci::Rectf NodeContainer::getBounds()
    {
        //Reset Bounds
        ci::Rectf bounds = ci::Rectf(0,0,0,0);
        
        for(NodeRef child : children)
            bounds.include(child->getFrame());
        
        return bounds;
    }
}