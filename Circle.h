/**
 * Square.h - A class implementation representing a square object in OpenGL.
 *
 * @author Carmen St. Jean (crr8, carmen@cs.unh.edu)
 *         Based on Triangle.h from Professor Dan Bergeron's demo1 program.
 *
 * UNH CS 870, fall 2012
 *
 * History
 *  09/07/2012: Modified to inherit from NShape instead of Shape.
 *  09/06/2012: Created class; modified to redraw with border.
 */
#ifndef CIRCLE_H_
#define CIRCLE_H_


#include "Shape.h"

class Circle: public Shape
{
public:
   Circle();
   virtual ~Circle();
   void drawAtOrigin();
};

#endif /*CIRCLE_H_*/