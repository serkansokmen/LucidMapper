
#include "ImageLayer.h"

void ImageLayer::setup(ofVec2f pos)
{	
	originalPosition	= pos;
	position			= originalPosition;
	
	isMoving = false;
}

void ImageLayer::update()
{
	velocity = (velocity * 0.1) + (destination - position) * 0.1;
	
	//velocity *= .9;
	
	position += velocity;
	
	float dist = position.distance(destination);
	
	if (dist < 1)
	{
		destination = originalPosition;
		isMoving = false;
	}
	else 
	{
		isMoving = true;
	}
}

void ImageLayer::draw()
{
	ofSetColor(255, 255, 255);
	glPushMatrix();
	glTranslatef(position.x, position.y, 0);
	img.draw(0, 0);
	glPopMatrix();
}
