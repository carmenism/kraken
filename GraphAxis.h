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
#ifndef GRAPHAXIS_H_
#define GRAPHAXIS_H_

class GraphAxis
{
public:
    GraphAxis();
    GraphAxis(float minV, float maxV);
    ~GraphAxis();

    void draw(float graphLength);

   // float getLength() { return length; }
   // void setLength(float len) { length = len; }

    float getMinorTickSpacing() { return minorTickSpacing; }
    void setMinorTickSpacing(float m) { minorTickSpacing = m; }

    float getMajorTickSpacing() { return majorTickSpacing; }
    void setMajorTickSpacing(float m) { majorTickSpacing = m; }

    float getMinorTickLength() { return minorTickLength; }
    void setMinorTickLength(float len) { minorTickLength = len; }

    float getMajorTickLength() { return majorTickLength; }
    void setMajorTickLength(float len) { majorTickLength = len; }

    float getMinimumValue() { return minValue; }
    void setMinimumValue(float m) { minValue = m; }

    float getMaximumValue() { return maxValue; }
    void setMaximumValue(float m) { maxValue = m; }

    bool getDisplayMinorTicks() { return displayMinorTicks; }
    void setDisplayMinorTicks(bool d) { displayMinorTicks = d; }
    void displayMinorTicksOn() { displayMinorTicks = true; }
    void displayMinorTicksOff() { displayMinorTicks = false; }

    bool getDisplayMajorTicks() { return displayMajorTicks; }
    void setDisplayMajorTicks(bool d) { displayMajorTicks = d; }
    void displayMajorTicksOn() { displayMajorTicks = true; }
    void displayMajorTicksOff() { displayMajorTicks = false; }

private:
    //float length;
    float minValue, maxValue;
    float minorTickSpacing, majorTickSpacing;
    float minorTickLength, majorTickLength;
    bool displayMinorTicks, displayMajorTicks;

    float valueToPosition(float length, float value);
    void drawTicks(float graphLength, float tickSpacing, float tickLength);
};

#endif /* GRAPHAXIS_H_ */