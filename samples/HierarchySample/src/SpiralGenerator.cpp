//
//  SpiralGenerator.cpp
//  HierarchySample
//
//  Created by bruce on 6/9/15.
//
//

#include "SpiralGenerator.h"

std::vector<SpiralGenerator::SpiralItemProperties> SpiralGenerator::getSpiralItemProperties(SpiralGenerator::SpiralProperties properties)
{
	std::vector<SpiralItemProperties> itemProperties;
	
	int maxItems = properties.maxItems;
	float slope = properties.slope;
	float expansion = properties.expansion;
	float r_min = properties.r_min;
	float r_max = properties.r_max;
	int spiral_type = properties.spiral_type;
	
	int i;
	float phi, distance, increment;
	float d_min, d_max;
	float result_radius;
	float c;
	float r_avg;
	float d_avg;
	
	c = 100;
	r_avg = (r_min + r_max)/2.0;
	d_min = 1.0;
	d_max = sqrt(maxItems)*expansion;
	d_avg = (d_min + d_max)/2.0;
	phi = (1+sqrt(5.0))/2.0;
	
	if(spiral_type <= 1) {
		increment = 2*M_PI*phi;
	} else {
		increment = 2*M_PI*phi + (2*M_PI/spiral_type);
	}
	
	for(i = 1; i < maxItems; i++) {
		distance = sqrt(i)*expansion;
		result_radius = r_avg + slope*(distance-d_avg)/(d_max-d_avg)*(r_max-r_avg);
		
		float angle = increment*i;
		
		float scaleDiff = result_radius / r_max;
		ci::Vec2f scale = ci::Vec2f(scaleDiff, scaleDiff);
		
		float cx = (distance*scale.x)*cos(angle);
		float cy = (distance*scale.y)*sin(angle);
		ci::Vec2f point = ci::Vec2f(cx, cy);
		
		//			ci::app::console() << r_max << ", " << result_radius << ", " << scaleDiff << std::endl;
		
		SpiralItemProperties property;
		property.position = point;
		property.angle = angle;
		property.scale = scale;
		itemProperties.push_back(property);
	}
	
	return itemProperties;
}