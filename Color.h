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

class Color
{
public:
    Color();
    Color( const Color& c );
    Color( float red, float green, float blue, float alpha = 1.0f );

    float r, g, b, a;
};

#endif /* COLOR_H */