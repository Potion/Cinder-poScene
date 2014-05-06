//
//  poUtils.h
//  ImageTest
//
//  Created by Stephen Varga on 3/31/14.
//
//

#include "cinder/gl/Texture.h"

#pragma once 

namespace po {
    
    #pragma message "Should we put this in a Texture or Utils Namespace"
    
    enum class TextureFit
    {
        NONE=0,
        EXACT,
        WIDTH,
        HEIGHT,
        INSIDE
    };
    
    static ci::Vec2f alignInRect(ci::Vec2f max, ci::Rectf rect, Alignment align) {
        ci::Vec2f offset;
        switch(align) {
            case Alignment::TOP_LEFT:
                offset.set(0.f, max.y - 1.f);
                break;
            case Alignment::TOP_CENTER:
                offset.set((max.x - 1.f)/2.f, max.y - 1.f);
                break;
            case Alignment::TOP_RIGHT:
                offset.set(max.x - 1.f, max.y - 1.f);
                break;
            case Alignment::CENTER_LEFT:
                offset.set(0.f, (max.y - 1.f)/2.f);
                break;
            case Alignment::CENTER_CENTER:
                offset.set((max.x - 1.f)/2.f, (max.y - 1.f)/2.f);
                break;
            case Alignment::CENTER_RIGHT:
                offset.set(max.x - 1.f, (max.y - 1.f)/2.f);
                break;
            case Alignment::BOTTOM_LEFT:
                offset.set(0.f, 0.f);
                break;
            case Alignment::BOTTOM_CENTER:
                offset.set((max.x - 1.f)/2.f, 0.f);
                break;
            case Alignment::BOTTOM_RIGHT:
                offset.set(max.x - 1.f, 0.f);
                break;
            case Alignment::NONE:
                break;
        }
        return offset;
    }
    
    
    //------------------------------------------------------------------------
    static void textureFitExact(ci::Rectf rect, ci::gl::TextureRef tex, Alignment align, const std::vector<ci::Vec2f> &points, std::vector<ci::Vec2f> &coords) {
        for(uint32_t i=0; i<points.size(); i++) {
            float s = (points[i].x-rect.getX1()) / rect.getWidth();
            float t = (points[i].y-rect.getY1()) / rect.getHeight();
            coords[i].set(s,1.f-t);
        }
    }
    
    
    //------------------------------------------------------------------------
    static void textureFitNone(ci::Rectf rect, ci::gl::TextureRef tex, Alignment align, const std::vector<ci::Vec2f> &points, std::vector<ci::Vec2f> &coords) {
        ci::Vec2f max(FLT_MIN, FLT_MIN);
        
        for(uint32_t i=0; i<points.size(); i++) {
            float s = (points[i].x-rect.getX1()) / tex->getWidth();
            float t = (points[i].y-rect.getY1()) / tex->getHeight();
            
            max.x = std::max(s, max.x);
            max.y = std::max(t, max.y);
            
            coords[i].set(s,t);
        }
        
        ci::Vec2f offset = alignInRect(max, ci::Rectf(0, 0, 1, 1), align);
        
        for(uint32_t i=0; i<coords.size(); i++) {
            coords[i] -= offset;
        }
    }
    
    
    //------------------------------------------------------------------------
    static void textureFitHorizontal(ci::Rectf rect, ci::gl::TextureRef tex, Alignment align, const std::vector<ci::Vec2f> &points, std::vector<ci::Vec2f> &coords ) {
        float new_w = rect.getWidth();
        float new_h = new_w / (tex->getWidth() / (float)tex->getHeight());
        
        ci::Vec2f max(FLT_MIN, FLT_MIN);
        
        for(uint32_t i=0; i<points.size(); i++) {
            float s = (points[i].x-rect.getX1()) / rect.getWidth();
            float t = (points[i].y-rect.getY1()) / new_h;
            
            max.x = std::max(s, max.x);
            max.y = std::max(t, max.y);
            
            coords[i].set(s,t);
        }
        
        ci::Vec2f offset = alignInRect(max, ci::Rectf(0,0,1,1), align);
        
        for(uint32_t i=0; i<coords.size(); i++) {
            coords[i] -= offset;
        }
    }
    
    
    //------------------------------------------------------------------------
    static void textureFitVertical(ci::Rectf rect, ci::gl::TextureRef tex, Alignment align, const std::vector<ci::Vec2f> &points, std::vector<ci::Vec2f> &coords ) {
        float new_h = rect.getHeight();
        float new_w = new_h / (tex->getHeight() / (float)tex->getWidth());
        
        ci::Vec2f max(FLT_MIN, FLT_MIN);
        
        for(uint32_t i=0; i<points.size(); i++) {
            float s = (points[i].x-rect.getX1()) / new_w;
            float t = (points[i].y-rect.getY1()) / rect.getHeight();
            
            max.x = std::max(s, max.x);
            max.y = std::max(t, max.y);
            
            coords[i].set(s,t);
        }
        
        ci::Vec2f offset = alignInRect(max, ci::Rectf(0,0,1,1), align);
        
        for(uint32_t i=0; i<coords.size(); i++) {
            coords[i] -= offset;
        }
    }
    
    
    
    //------------------------------------------------------------------------
    static void textureFit(ci::Rectf rect, ci::gl::TextureRef tex, TextureFit fit, Alignment align, const std::vector<ci::Vec2f> &points, std::vector<ci::Vec2f> &coords )
    {
        switch(fit) {
            case TextureFit::NONE:
                textureFitNone(rect, tex, align, points, coords ); break;
            case TextureFit::EXACT:
                textureFitExact(rect, tex, align, points, coords); break;
            case TextureFit::WIDTH:
                textureFitHorizontal(rect, tex, align, points, coords); break;
            case TextureFit::HEIGHT:
                textureFitVertical(rect, tex, align, points, coords);
                break;
                
            case TextureFit::INSIDE:
            {
                float new_h = ((float)rect.getWidth()) * ((float)tex->getHeight()) / ((float)tex->getWidth());
                if(new_h > rect.getHeight())
                    textureFitVertical(rect, tex, align, points, coords);
                else
                    textureFitHorizontal(rect, tex, align, points, coords);
                break;
            }
            default:
                ;
        }
    }
    
    
    //------------------------------------------------------------------------
    std::vector<ci::Vec2f> textureFit(ci::Rectf rect, ci::gl::TextureRef tex, TextureFit fit, Alignment align) {
        std::vector<ci::Vec2f> coords(4);
        std::vector<ci::Vec2f> points = {rect.getUpperLeft(), rect.getUpperRight(), rect.getLowerRight(), rect.getLowerLeft()};
        textureFit(rect, tex, fit, align, coords, points);
        return coords;
    }

}