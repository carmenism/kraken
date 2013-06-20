#ifndef PRINTTEXT_H_
#define PRINTTEXT_H_

#include <string>
#include <GL/glut.h>
#include <sstream>

#define DEFAULT_FONT GLUT_BITMAP_HELVETICA_18

class PrintText
{
public:
    static bool printAt(float rx, float ry, std::string text,
        bool needRaw = true, GLboolean shadow = GL_TRUE, 
        void *font = DEFAULT_FONT, float alpha = 1.0);
    
protected:
    static int inRawGL;
    static bool compiling;
    static int dragAmount;
    static bool dragging;
    static int lastTextHeight, lastTextWidth;

private:
    static void beginRaw2D();
    static void endRaw2D();

    static bool beginRaster2D(float rx, float ry, bool needRaw = true);
    static void endRaster2D();

    static int printingWidth(std::string text, void *font = DEFAULT_FONT);
    static int printingHeight(void *font = DEFAULT_FONT);
};

inline std::string toStr(float x) {
  std::ostringstream o;

  if (!(o << x)) {
      return NULL;
      //throw BadConversion("stringify(double)");
  }

  return o.str();
}

#endif /* PRINTTEXT_H_ */