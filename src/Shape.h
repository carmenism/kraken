#ifndef SHAPE_H_
#define SHAPE_H_

class Color;

#include "Object2D.h"
#include "Pickable.h"

class Shape : public Object2D {
public:
   Shape();
   virtual ~Shape();
   
   void setId(int i) { id = i; }
   int getId() { return id; }

   float getRotation() { return rotation; }
   void setRotation(float r) { rotation = r; }

   Color *getFillColor() { return fillColor; }
   void setFillColor(float r, float g, float b); // set color
   void setFillColor(Color *c) { fillColor = c; }
   
   float getBorderWidth() { return borderWidth; }
   void setBorderWidth(float w) { borderWidth = w; }

   Color *getBorderColor() { return borderColor; }
   void setBorderColor(float r, float g, float b); // set boundary color
   void setBorderColor(Color *c) { borderColor = c; }

   bool getDrawFill() { return drawFill; }
   void setDrawFill(bool df) { drawFill = df; }

   bool getDrawBorder() { return drawBorder; }
   void setDrawBorder(bool db) { drawBorder = db; }

   void setPickColor(unsigned char r, unsigned char g, unsigned char b)
                  { pickR = r; pickG = g; pickB = b; }

   void draw();   
   void drawToPick();
   
   virtual void drawAtOrigin() = 0; // to be overwritten
protected: 
   int id;                       // id of the object
   float rotation;               // rotation
   float borderWidth;            // width of the boundary

   Color *fillColor;             // color
   Color *borderColor;           // boundary color
 
   unsigned char pickR, pickG, pickB;

   bool drawFill;               // true to draw fill color
   bool drawBorder;             // true to draw boundary
};

#endif /*SHAPE_H_*/