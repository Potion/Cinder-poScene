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
    
    #pragma mark Set and Remove Scene
    void NodeContainer::setScene(SceneRef scene)
    {
        Node::setScene(scene);
        for(NodeRef childNode : children) {
            childNode->setScene(scene);
        }
    }
    
    void NodeContainer::removeScene()
    {
        Node::removeScene();
        for(NodeRef node : children) {
            node->removeScene();
        }
    }

    void NodeContainer::addChild(NodeRef node)
    {
        //See if the node is already a child of another node.
        if(node->getParent()) {
            node->getParent()->removeChild(node);
        }
        
        //Assign ourselves as the parent
        #pragma message "This is ugly...if this needs to be done a lot its gonna get messy with inheritance and shared_from_this"
        //node->setParent(enable_shared_from_this<NodeContainer>::shared_from_this());
        node->setParent(std::dynamic_pointer_cast<NodeContainer>(shared_from_this()));
        node->setScene(scene.lock());
        
        //Track Node
        children.push_back(node);
    }

    bool NodeContainer::removeChild(NodeRef node)
    {
        std::vector<NodeRef>::iterator iter = std::find(children.begin(), children.end(), node);
        
        if(iter != children.end()) {
            //Remove reference to this node in child
            node->removeParent();
            node->removeScene();
            
            //Erase node
            children.erase(iter);
            
            return true;
        }
        
        return false;
    }

    void NodeContainer::updateTree()
    {
        update();
        for(NodeRef childNode : children)
            childNode->updateTree();
    }

    void NodeContainer::drawTree()
    {
        #pragma message "Need to implement matrix order"
        ci::gl::pushMatrices();
        setTransformation();
        
        for(NodeRef childNode : children)
            childNode->drawTree();
        
        if(bDrawBounds)
            drawBounds();
        
        ci::gl::popMatrices();
    }

    ci::Rectf NodeContainer::getBounds()
    {
        //Reset Bounds
        ci::Rectf bounds = ci::Rectf(0,0,0,0);
        
        for(NodeRef childNode : children)
            bounds.include(childNode->getFrame());
        
        return bounds;
    }
    
    bool NodeContainer::pointInside(const ci::Vec2f &point)
    {
        for (NodeRef node : children) {
            if (node->pointInside(point)) {
                return true;
            }
        }
        
        return false;
    }
}