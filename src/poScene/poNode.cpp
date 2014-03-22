//
//  poNode.cpp
//  BasicTest
//
//  Created by Stephen Varga on 3/17/14.
//
//

#include "poNode.h"
#include "poNodeContainer.h"

namespace po {
    static uint OBJECT_UID  = 0;
    static const int ORIGIN_SIZE   = 2;
    
    NodeRef Node::create()
    {
        return std::shared_ptr<Node>(new Node());
    }
    
    Node::Node()
    :   uid(OBJECT_UID++)
    ,   scale(1.0,1.0)
    ,   bDrawBounds(false)
    ,   bDrawFrame(false)
    {
    }
    
    Node::~Node() {}
    
    void Node::updateTree()
    {
        update();
    }
    
    void Node::update() {}
    
    void Node::drawTree()
    {
        //Push our Matrix
        ci::gl::pushMatrices();
        setTransformation();
        
        //Draw this item
        draw();
        
        //Draw bounds and/or frame if necessary
        if(bDrawBounds || bDrawFrame) {
            if(bDrawBounds)
                drawBounds();
            
            if(bDrawFrame)
                drawFrame();
        }
        
        //Pop our Matrix
        ci::gl::popMatrices();
    }
    
    void Node::draw() {
    }
    
    //------------------------------------------------------
    #pragma mark - Transformation -
    
    void Node::setTransformation()
    {
        #pragma message "Need to implement matrix order"
        ci::gl::translate(position);
        ci::gl::rotate(rotation);
        ci::gl::scale(scale);
    }
    
    
    //------------------------------------------------------
    #pragma mark - SceneGraph -
    
    SceneRef Node::getScene()
    {
        return scene;
    }
    
    NodeContainerRef Node::getParent() const {
        return parent.lock();
    }
    
    void Node::setParent(NodeContainerRef node)
    {
        scene = node->getScene();
        parent = node;
    }
        
    bool Node::hasParent()
    {
        if(parent.lock()) {
            return true;
        }
        
        return false;
    }
    
    void Node::removeParent() {
        parent.reset();
    }
    
    
    //------------------------------------------------------
    #pragma mark  - Dimensions -
    
    ci::Rectf Node::getBounds()
    {
        //Reset Bounds
        ci::Rectf bounds = ci::Rectf(0,0,0,0);
        return bounds;
    }
    
    void Node::setDrawBoundsEnabled(bool enabled)
    {
        bDrawBounds = enabled;
    }
    
    void Node::drawBounds()
    {
        ci::gl::color(255,0,0);
        
        //Draw bounding box
        ci::gl::drawStrokedRect(getBounds());
        
        //Draw origin
        ci::gl::pushMatrices();
        ci::gl::scale(ci::Vec2f(1.f,1.f)/ scale);
        ci::gl::drawSolidRect(ci::Rectf(-ORIGIN_SIZE/2, -ORIGIN_SIZE/2, ORIGIN_SIZE, ORIGIN_SIZE));
        ci::gl::popMatrices();
    }
    
    ci::Rectf Node::getFrame()
    {
        ci::Rectf frame = getBounds();
        frame.scale(scale);
        frame.offset(position);
        return frame;
    }
    
    void Node::setDrawFrameEnabled(bool enabled)
    {
        bDrawFrame = enabled;
    }
    
    void Node::drawFrame()
    {
        ci::gl::color(255,0,0);
        ci::gl::drawStrokedRect(getBounds());
    }
}
