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

#include "cinder/app/App.h"
#include "poEvents.h"

namespace po { namespace scene {
    
    //  Events in poScene extend the functionality of
    
    class Node;
    typedef std::shared_ptr<Node> NodeRef;
    
    class EventCenter;
	
	
    //------------------------------------
    //	Base Event
    
    class Event {
        friend class Node;
		
    public:
        Event();
        
        void setShouldPropagate(bool shouldPropagate) { mShouldPropagate = shouldPropagate; };
        bool getShouldPropagate() { return mShouldPropagate; };
        
        ci::Vec2f getWindowPos() { return mWindowPos; }
        ci::Vec2f getLocalPos();
        ci::Vec2f getScenePos();
        NodeRef getSource() { return mSource.lock(); };
        
    protected:
        void setSource(NodeRef source) { mSource = source; };
        ci::Vec2f mWindowPos;
        
    private:
        bool mShouldPropagate;
        std::weak_ptr<Node> mSource;
		
    };
	
    
    //------------------------------------
    //	Mouse Event
		#pragma mark - Mouse Event
	//------------------------------------
    
    class MouseEvent
    : public po::scene::Event
    {
        friend class EventCenter;
        
    public:
        enum Type {
            DOWN,
            DOWN_INSIDE,
            MOVE,
            MOVE_INSIDE,
            DRAG,
            DRAG_INSIDE,
            UP,
            UP_INSIDE,
            WHEEL
        };
        
        MouseEvent(ci::app::MouseEvent event, Type type);
        ci::app::MouseEvent getCiEvent() { return mCiEvent; };
        Type getType() { return mType; };
        
    protected:
        void setType(Type type) { mType = type; };
        
    private:
        ci::app::MouseEvent mCiEvent;
        Type mType;
		
    };
    
	
    //------------------------------------
    //	Touch Event
		#pragma mark - Touch Event
	//------------------------------------
    
    class TouchEvent
    : public Event
    {
        friend class EventCenter;
    public:
        enum Type {
            BEGAN,
            BEGAN_INSIDE,
            MOVED,
            MOVED_INSIDE,
            ENDED,
            ENDED_INSIDE
        };
        
        TouchEvent(ci::app::TouchEvent::Touch event, Type type);
        ci::app::TouchEvent::Touch getCiEvent() { return mCiEvent; };
        Type getType() { return mType; }
        
    protected:
        void setType(Type type) { mType = type; };
        
    private:
        ci::app::TouchEvent::Touch mCiEvent;
        Type mType;
		
    };
	
} } //  namespace po::scene