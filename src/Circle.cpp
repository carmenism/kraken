// http://slabode.exofire.net/circle_draw.shtml

#include "Circle.h"
#include <GL/glut.h>
#include <cmath>

Circle::Circle()
{
}

//------------- destructor -----------------------
Circle::~Circle()
{
}

void Circle::drawAtOrigin() {
    float r = 0.5;
    float num_segments = 72.0;

	float theta = 2 * 3.1415926 / num_segments; 
    float tangetial_factor = tanf(theta);//calculate the tangential factor 

	float radial_factor = cosf(theta);//calculate the radial factor 
	
	float x = r;//we start at angle = 0 

	float y = 0; 
    
	for(int ii = 0; ii < num_segments; ii++) 
	{ 
        glVertex2f(x, y);
		
        //calculate the tangential vector 
		//remember, the radial vector is (x, y) 
		//to get the tangential vector we flip those coordinates and negate one of them 

		float tx = -y; 
		float ty = x; 
        
		//add the tangential vector 

		x += tx * tangetial_factor; 
		y += ty * tangetial_factor; 
        
		//correct using the radial factor 

		x *= radial_factor; 
		y *= radial_factor; 
	} 
}