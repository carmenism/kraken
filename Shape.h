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

   void setPickColor(int r, int g, int b) { pickR = r; pickG = g; pickB = b; }

   void draw();   
   void drawToPick();
   
   virtual void drawAtOrigin() = 0; // to be overwritten
   //virtual void mouseHover();
protected: 
   int id;                       // id of the object
   float xLoc, yLoc;             // location of the object
   float width, height;          // size of the object
   float rotation;               // rotation
   float borderWidth;            // width of the boundary

   Color *fillColor;             // color
   Color *borderColor;           // boundary color
   
   int pickR, pickG, pickB;

   bool drawFill;               // true to draw fill color
   bool drawBorder;             // true to draw boundary
//   bool drawToPick;             
};

#endif /*SHAPE_H_*/