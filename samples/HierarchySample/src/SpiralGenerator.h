//
//  SpiralGenerator.h
//  HierarchySample
//
//  Created by bruce on 6/9/15.
//
//

#pragma once

class SpiralGenerator
{
public:
	struct SpiralProperties {
		SpiralProperties(){};
		
		int maxItems; // max items
		float slope; // slope for scaling items
		float expansion; // space between items
		float r_min; // minimum radius
		float r_max; // maximum radius
		int spiral_type;
	};
	
	struct SpiralItemProperties {
		SpiralItemProperties(){};
		
		ci::Vec2f position;
		float angle;
		ci::Vec2f scale;
	};
	
	static std::vector<ci::Vec2f> getPoints(SpiralProperties properties);
	static std::vector<SpiralItemProperties> getSpiralItemProperties(SpiralProperties properties);
	
private:
	SpiralGenerator();
	
};
