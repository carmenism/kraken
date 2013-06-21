/**
 * Color.cpp - A class abstraction for gl color specification. This only uses
 *  the float version, but it could be extended if it ever proved useful to be
 *  more generic.  It supports rgb and rgba.
 *
 * @author Carmen St. Jean (crr8, carmen@cs.unh.edu)
 *         Based on Color.cpp by Dan Bergeron.
 *
 * UNH CS 870, fall 2012
 *
 * History:
 *  10/07/2012: Modified slightly to incorporate into homework assignment.
 *  09/24/2012: For now this is little more than a struct.
 */
#include "Color.h"

//------------- simple constructors -----------------------
Color::Color() {
    r = g = b = 0.0f;
    a = 1.0f;

    assigned = false;
}

Color::Color( float red, float green, float blue, float alpha ) {
    // Should check that values are in the range [0,1]
    r = red;
    g = green;
    b = blue;
    a = alpha;

    assigned = false;
}
//------------- copy constructor -----------------------
/**
 * Not really needed since this is default behavior
 */
Color::Color( const Color& col ) {
    r = col.r;
    g = col.g;
    b = col.b;
    a = col.a;

    assigned = false;
}

Color *Color::getUnassignedColor() {
    for (int i = 0; i < numberColors; i++) {
        if (!colors[i].assigned) {
            colors[i].assigned = true;

            return &colors[i];
        }
    }

    return NULL;
}

Color Color::red(0.85, 0.0, 0.0);
Color Color::orange(0.93, 0.60, 0.0);
Color Color::goldenrod(0.85, 0.65, 0.13);
Color Color::yellow(0.93, 0.93, 0.0);
Color Color::avocado(0.64, 0.74, 0.07);
Color Color::green(0.0, 0.5, 0.0);
Color Color::skyblue(0.53, 0.81, 0.92);
Color Color::blue(0.0, 0.0, 0.93);
Color Color::navy(0.0, 0.0, 0.35);
Color Color::purple(0.33, 0.1, 0.55);
Color Color::lavender(0.8, 0.6, 0.8);
Color Color::magenta(0.93, 0.07, 0.54);
Color Color::pink(0.96, 0.66, 0.71);
Color Color::tan(0.8, 0.67, 0.49);
Color Color::brown(0.54, 0.21, 0.06);

Color Color::gray(0.5, 0.5, 0.5);
Color Color::black(0.0, 0.0, 0.0);
Color Color::white(1.0, 1.0, 1.0);

Color arr[] = {Color::red, Color::blue, Color::green, 
                  Color::orange, Color::yellow, Color::purple,
                  Color::goldenrod, Color::pink, Color::skyblue,
                  Color::magenta, Color::avocado, Color::navy,
                  Color::brown, Color::lavender, Color::tan};

Color *Color::colors = arr;

int Color::numberColors = (sizeof(arr)/sizeof(*arr));

//std::vector<Color> Color::colors = {Color::red, Color::blue, Color::green, 
//                   Color::orange, Color::yellow, Color::purple,
//                   Color::goldenrod, Color::pink, Color::skyblue,
//                   Color::magenta, Color::avocado, Color::navy,
//                   Color::brown, Color::lavender, Color::tan};