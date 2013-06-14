#ifndef SHAPE_H_
#define SHAPE_H_

class Color;

class Shape
{
public:
   Shape();
   virtual ~Shape();
   
   void setId(int i) { id = i; }
   int getId() { return id; }

   float getX() { return xLoc; }
   float getY() { return yLoc; }
   void setLocation( float x, float y ) { xLoc = x; yLoc = y; }

   float getRotation() { return rotation; }
   void setRotation(float r) { rotation = r; }

   float getWidth() { return width; }
   float getHeight() { return height; }
   void setSize(float w, float h) { width = w; height = h; }

   Color *getColor() { return color; }
   void setColor(float r, float g, float b); // set color
   void setColor(Color *c) { color = c; }
   
   float getBorderWidth() { return borderWidth; }
   void setBorderWidth(float w) { borderWidth = w; }

   Color *getBorderColor() { return borderColor; }
   void setBorderColor(float r, float g, float b); // set boundary color
   void setBorderColor(Color *c) { borderColor = c; }

   bool getDrawFill() { return drawFill; }
   void setDrawFill(bool df) { drawFill = df; }

   bool getDrawBorder() { return drawBorder; }
   void setDrawBorder(bool db) { drawBorder = db; }

   void redraw();            
   
   virtual void drawAtOrigin() = 0; // to be overwritten
protected: 
   int id;                       // id of the object
   float xLoc, yLoc;             // location of the object
   float width, height;          // size of the object
   float rotation;               // rotation
   Color *color;                 // color
   float borderWidth;            // width of the boundary
   Color *borderColor;           // boundary color
   bool  drawFill;               // true to draw fill color
   bool  drawBorder;             // true to draw boundary
};

#endif /*SHAPE_H_*/