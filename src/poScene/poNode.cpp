//
//  poNode.cpp
//  BasicTest
//
//  Created by Stephen Varga on 3/17/14.
//
//

#include "cinder/CinderMath.h"

#include "poNode.h"
#include "poNodeContainer.h"
#include "poScene.h"

namespace po {
    static uint OBJECT_UID  = 0;
    static const int ORIGIN_SIZE   = 2;
    
    NodeRef Node::create()
    {
        return std::shared_ptr<Node>(new Node());
    }
    
    Node::Node()
    :   uid(OBJECT_UID++)
    ,   position(0.f,0.f)
    ,   scale(1.f,1.f)
    ,   rotation(0)
    ,   offset(0.f,0.f)
    ,   alpha(1.f)
    ,   positionAnim(ci::Vec2f(0.f,0.f))
    ,   scaleAnim(ci::Vec2f(1.f,1.f))
    ,   rotationAnim(0)
    ,   offsetAnim(ci::Vec2f(0.f,0.f))
    ,   alphaAnim(1.f)
    ,   bUpdatePositionFromAnim(false)
    ,   bUpdateScaleFromAnim(false)
    ,   bUpdateRotationFromAnim(false)
    ,   bUpdateOffsetFromAnim(false)
    ,   bUpdateAlphaFromAnim(false)
    ,   bDrawBounds(false)
    ,   bVisible(true)
    ,   bInteractionEnabled(true)
    {
        //Initialize our animations
        initAttrAnimations();
    }
    
    Node::~Node() {
        
    }
    
    
    
    //------------------------------------------------------
    #pragma mark - Update & Draw Trees -
    
    void Node::updateTree()
    {
        //Update our tween tie-in animations
        updateAttributeAnimations();
        
        //Call our update function
        update();
    }
    
    void Node::drawTree()
    {
        //If we're invisible, nothing to do here
        if(!bVisible) return;
        
        //Update our draw order
        drawOrder = scene.lock()->getNextDrawOrder();
        
        //Push our Matrix
        ci::gl::pushMatrices();
        setTransformation();
        
        //Draw this item
        draw();
        
        //Draw bounds if necessary
        if(bDrawBounds)
            drawBounds();
        
        //Pop our Matrix
        ci::gl::popMatrices();
    }
    
    
    
    
    //------------------------------------------------------
    #pragma mark - Attributes -
    
    void Node::setPosition(float x, float y)
    {
        positionAnim.stop();
        bUpdatePositionFromAnim = false;
        position.set(x, y);
        positionAnim.ptr()->set(position);
    }
    
    void Node::setScale(float x, float y)
    {
        scaleAnim.stop();
        bUpdateScaleFromAnim = false;
        scale.set(x, y);
        scaleAnim.ptr()->set(scale);
    }
    
    void Node::setRotation(float rotation)
    {
        rotationAnim.stop();
        bUpdateRotationFromAnim = false;
        this->rotation = rotation;
        rotationAnim = rotation;
    }
    
    void Node::setAlpha(float alpha)
    {
        alphaAnim.stop();
        bUpdateAlphaFromAnim = false;
        this->alpha = ci::math<float>::clamp(alpha, 0.f, 1.f);
        alphaAnim = this->alpha;
    }
    
    void Node::setOffset(float x, float y) {
        offsetAnim.stop();
        bUpdateOffsetFromAnim = false;
        offset.set(x, y);
        offsetAnim.ptr()->set(offset);
        
        //If we are manually setting the offset, we can't have alignment
        setAlignment(Alignment::NONE);
    }
    
    void Node::initAttrAnimations()
    {
        //Initialize the isComplete() method of each tween, a bit annoying
        positionAnim.stop();
        scaleAnim.stop();
        rotationAnim.stop();
        offsetAnim.stop();
        alphaAnim.stop();
    }
    
    void Node::updateAttributeAnimations()
    {
        //See if a tween is in progress, if so we want to use that value
        //Setting an attribute calls stop(), so that will override this
        if(!positionAnim.isComplete())  bUpdatePositionFromAnim = true;
        if(!scaleAnim.isComplete())     bUpdateScaleFromAnim    = true;
        if(!rotationAnim.isComplete())  bUpdateRotationFromAnim = true;
        if(!alphaAnim.isComplete())     bUpdateAlphaFromAnim    = true;
        if(!offsetAnim.isComplete())    bUpdateOffsetFromAnim   = true;
        
        //Update Anims if we care
        if(bUpdatePositionFromAnim)     position    = positionAnim;
        if(bUpdateScaleFromAnim)        scale       = scaleAnim;
        if(bUpdateRotationFromAnim)     rotation    = rotationAnim;
        if(bUpdateAlphaFromAnim)        alpha       = alphaAnim;
        if(bUpdateOffsetFromAnim)       offset      = offsetAnim;
    }
    
    //------------------------------------------------------
    #pragma mark  - Alignment -
    
    void Node::setAlignment(po::Node::Alignment alignment)
    {
        this->alignment = alignment;
        
        if(alignment == Alignment::NONE) return;
        
        ci::Rectf bounds = getBounds();
        
        switch (alignment) {
            case Alignment::TOP_LEFT:
                offset.set(0,0); break;
            case Alignment::TOP_CENTER:
                offset.set(-bounds.getWidth()/2.f,0); break;
            case Alignment::TOP_RIGHT:
                offset.set(-bounds.getWidth(),0); break;
            case Alignment::CENTER_LEFT:
                offset.set(0,-bounds.getHeight()/2.f); break;
            case Alignment::CENTER_CENTER:
                offset.set(-bounds.getWidth()/2.f,-bounds.getHeight()/2.f); break;
            case Alignment::CENTER_RIGHT:
                offset.set(-bounds.getWidth(),-bounds.getHeight()/2.f); break;
            case Alignment::BOTTOM_LEFT:
                offset.set(0,-bounds.getHeight()); break;
            case Alignment::BOTTOM_CENTER:
                offset.set(-bounds.getWidth()/2.f,-bounds.getHeight()); break;
            case Alignment::BOTTOM_RIGHT:
                offset.set(-bounds.getWidth(),-bounds.getHeight()); break;
        }
        
        offset = offset-bounds.getUpperLeft();
    }
    
    
    //------------------------------------------------------
    #pragma mark - Transformation -
    
    void Node::setTransformation()
    {
        #pragma message "Need to implement matrix order"
        ci::gl::translate(position);
        ci::gl::rotate(rotation);
        ci::gl::scale(scale);
        
        ci::gl::translate(offset);
    
        matrix.set(ci::gl::getModelView(), ci::gl::getProjection(), ci::gl::getViewport());
    }
    
    ci::Vec2f Node::sceneToLocal(ci::Vec2f scenePoint)
    {
        return ci::Vec2f();
    }
    
    ci::Vec2f Node::globalToLocal(ci::Vec2f globalPoint)
    {
        return matrix.globalToLocal(globalPoint);
    }
    
    bool Node::pointInside(const ci::Vec2f &point)
    {
        return false;
    }
    
    
    
    
    //------------------------------------------------------
    #pragma mark - Parent & Scene -
    
    void Node::setScene(SceneRef sceneRef) {
        scene = sceneRef;
        if(hasScene()) scene.lock()->trackChildNode(shared_from_this());
    }
    
    SceneRef Node::getScene()
    {
        return scene.lock();
    }
    
    bool Node::hasScene()
    {
        return (scene.lock() ? true : false);
    }
    
    void Node::removeScene()
    {
        if(hasScene()) scene.lock()->untrackChildNode(shared_from_this());
        scene.reset();
    }
    
    void Node::setParent(NodeContainerRef containerNode)
    {
        parent = containerNode;
    }
    
    NodeContainerRef Node::getParent() const {
        return parent.lock();
    }
        
    bool Node::hasParent()
    {
        return (parent.lock() ? true : false);
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
        frame += offset;
        frame.scale(scale);
        frame.offset(position);
        return frame;
    }
    
    
    
    
    //------------------------------------------------------
    #pragma mark  - Events -
    
    #pragma mark General
    void Node::removeAllEvents() {}
    
    //Check any list of event callbacks to see if there is already a callback for a given listener
    bool Node::callbackAlreadyExistsForListener(NodeRef listener, std::vector<EventCallback> &callbackList)
    {
        for(EventCallback callback : callbackList) {
            if(callback.listener.lock() == listener) {
                return true;
            }
        }
               
        return false;
    }
    
    
    #pragma mark Mouse Events
    
    //Global Events
    void Node::notifyGlobal(po::MouseEvent &event) {
        #pragma message "If we just have one mouse event handler this gets infinitely cleaner...i.e. just node->mouseEvent(event)"
        switch (event.getType()) {
            case po::MouseEvent::Type::DOWN:
                mouseDown(event);
                break;
                
            case po::MouseEvent::Type::MOVE:
                mouseMove(event);
                break;
                
            case po::MouseEvent::Type::DRAG:
                mouseDrag(event);
                break;
                
            case po::MouseEvent::Type::UP:
                mouseUp(event);
                break;
                
            case po::MouseEvent::Type::WHEEL:
                mouseWheel(event);
                break;
        }
    }
    
    //Callbacks
    void Node::subscribeToEvent(po::MouseEvent::Type type, NodeRef source)
    {
        //If no source is provided, assume we want to subscribe to our own event
        if(!source) source = shared_from_this();
        
        //Subscribe to the source
        source->registerEventCallback(type, shared_from_this());
        
        #pragma message "Now we need to track our subscriptions on this end for removing events"
        //Track this Subscription
    }
    
    
    void Node::unsubscribeFromEvent(po::MouseEvent::Type type, NodeRef source) {
        //If no source is provided, assume we want to unsubscribe to our own event
        if(!source) source = shared_from_this();
        #pragma message "Now just need to implement this shit!"
    }
    
    //Sets this event
    void Node::registerEventCallback(po::MouseEvent::Type type, NodeRef listener)
    {
        //Check to see if we already have the callback
        if(callbackAlreadyExistsForListener(listener, mouseEventCallbacks[type]))
            return;
        
        //Otherwise Insert this callback
        EventCallback callback;
        callback.listener = listener;
        mouseEventCallbacks[type].push_back(callback);
    }
    
    //See if we care about an event
    bool Node::hasCallbacks(po::MouseEvent::Type type)
    {
        if(type == po::MouseEvent::Type::DOWN_INSIDE) {
            "Yea!";
        }
        return mouseEventCallbacks[type].size();
    }
    
    //For the given event, notify everyone that we have as a subscriber
    void Node::notifyCallbacks(po::MouseEvent &event)
    {
        event.source = shared_from_this();
        
        //Iterate through all callbacks
        for(EventCallback callback : mouseEventCallbacks[event.getType()])
        {
            //Notify the callback
            NodeRef listener = callback.listener.lock();
            
            #pragma message "Can we do this any better, maybe using templates?"
            if(listener && !callback.markedForRemoval) {
                switch (event.getType()) {
                    case po::MouseEvent::Type::DOWN_INSIDE:
                        listener->mouseDownInside(event);
                        break;
                        
                    case po::MouseEvent::Type::MOVE_INSIDE:
                        listener->mouseMoveInside(event);
                        break;
                        
                    case po::MouseEvent::Type::DRAG_INSIDE:
                        listener->mouseDragInside(event);
                        break;
                        
                    case po::MouseEvent::Type::UP_INSIDE:
                        listener->mouseUpInside(event);
                        break;
                }
            }
        }
        
        //Cleanup our callbacks, checking for deleted node refs or callbacks that have been markedForRemoval
        std::vector<EventCallback>::iterator iter = mouseEventCallbacks[event.getType()].begin();
        for(; iter != mouseEventCallbacks[event.getType()].end(); ++iter)
        {
            if(!(*iter).listener.lock() || (*iter).markedForRemoval) {
                iter = mouseEventCallbacks[event.getType()].erase(iter);
            }
        }
    }
}
