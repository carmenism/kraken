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
}

Color::~Color() {

}

Color::Color( float red, float green, float blue, float alpha ) {
    // Should check that values are in the range [0,1]
    r = red;
    g = green;
    b = blue;
    a = alpha;
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
}

// http://stackoverflow.com/questions/180/function-for-creating-color-wheels
Color *Color::getEvenlyDistributedColor(int numberColors, int offset) {
    float hueInterval = 1.0f / numberColors;
    float hue = hueInterval * offset;

    float saturation = 1.0;
    float brightness = 0.5;

    //if (offset % 3 == 1) {
        brightness = 0.4f;
    //} else if (offset % 3 == 2) {
    //    brightness = 0.6f;
    //}

    return hslToRgb(hue, saturation, brightness);
}

// http://stackoverflow.com/questions/2353211/hsl-to-rgb-color-conversion
Color *Color::hslToRgb(float h, float s, float l, float a) {
    if (s == 0) {
        return new Color(l, l, l, a);
    }

    float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
    float p = 2.0f * l - q;

    float r = hueToRgb(p, q, h + 1.0f / 3.0f);
    float g = hueToRgb(p, q, h);
    float b = hueToRgb(p, q, h - 1.0f / 3.0f);

    return new Color(r, g, b, a);
}

float Color::hueToRgb(float p, float q, float t) {
    if (t < 0) {
        t = t + 1;
    }

    if (t > 1) {
        t = t - 1;
    }

    if (t < (1.0 / 6.0)) {
        return p + (q - p) * 6 * t;
    }

    if (t < (1.0 / 2.0)) {
        return q;
    }

    if (t < (2.0 / 3.0)) {
        return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
    }

    return p;
}

Color Color::red(0.85f, 0.0f, 0.0f);
Color Color::orange(0.93f, 0.60f, 0.0f);
Color Color::goldenrod(0.85f, 0.65f, 0.13f);
Color Color::yellow(0.93f, 0.93f, 0.0f);
Color Color::avocado(0.64f, 0.74f, 0.07f);
Color Color::green(0.0f, 0.5f, 0.0f);
Color Color::skyblue(0.53f, 0.81f, 0.92f);
Color Color::blue(0.0f, 0.0f, 0.93f);
Color Color::navy(0.0f, 0.0f, 0.35f);
Color Color::purple(0.33f, 0.1f, 0.55f);
Color Color::lavender(0.8f, 0.6f, 0.8f);
Color Color::magenta(0.93f, 0.07f, 0.54f);
Color Color::pink(0.96f, 0.66f, 0.71f);
Color Color::tan(0.8f, 0.67f, 0.49f);
Color Color::brown(0.54f, 0.21f, 0.06f);

Color Color::darkRed(0.55f, 0, 0);

Color Color::gray(0.5, 0.5, 0.5);
Color Color::black(0.0, 0.0, 0.0);
Color Color::white(1.0, 1.0, 1.0);