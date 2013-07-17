#pragma once

#include "ofMain.h"

class ImageLayer
{
	ofVec2f	originalPosition;
	
public :
	
	void	setup(ofVec2f pos);
	void	update();
	void	draw();
	
	ofImage		img;
	
	ofVec2f	position;
	ofVec2f	velocity;
	ofVec2f	destination;
	
	bool		isMoving;
};