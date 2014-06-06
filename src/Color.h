/**
 * Color.h - A class abstraction for gl color specification. This only uses the
 * float version, but it could be extended if it ever proved useful to be more
 * generic.  It supports rgb and rgba.
 *
 * @author Carmen St. Jean (crr8, carmen@cs.unh.edu)
 *         Based on Color.h by Dan Bergeron.
 *
 * UNH CS 870, fall 2012
 *
 * History:
 *  10/07/2012: Modified slightly to incorporate into homework assignment.
 *  09/24/2012: For now this is little more than a struct.
 */
#ifndef COLOR_H
#define COLOR_H

#include <vector>

class Color
{
public:
    Color();
    ~Color();
    Color( const Color& c );
    Color( float red, float green, float blue, float alpha = 1.0f );

    float r, g, b, a;

    static Color red;
    static Color orange;
    static Color yellow;
    static Color green;
    static Color blue;
    static Color purple;
    static Color goldenrod;
    static Color pink;
    static Color skyblue;
    static Color brown;
    static Color lavender;
    static Color tan;
    static Color magenta;
    static Color avocado;
    static Color navy;

    static Color darkRed;

    static Color gray;
    static Color black;
    static Color white;

    static Color *getEvenlyDistributedColor(int numberColors, int offset);

private:
    static Color *hslToRgb(float h, float s, float l, float a = 1.0);
    static float hueToRgb(float p, float q, float t);
};

#endif /* COLOR_H */