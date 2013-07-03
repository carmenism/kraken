#ifndef LINECHARTAXIS_H_
#define LINECHARTAXIS_H_

#include <string>

class LineChart;

enum {AXIS_BOTTOM, AXIS_TOP, AXIS_LEFT, AXIS_RIGHT};

class LineChartAxis
{
public:
    LineChartAxis(LineChart *chart, int type = AXIS_BOTTOM);
    ~LineChartAxis();

    void draw();

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

    bool getDisplay() { return display; }
    void setDisplay(bool d) { display = d; }
    void displayOn() { display = true; }
    void displayOff() { display = false; }

    bool getDisplayMinorTicks() { return displayMinorTicks; }
    void setDisplayMinorTicks(bool d) { displayMinorTicks = d; }
    void displayMinorTicksOn() { displayMinorTicks = true; }
    void displayMinorTicksOff() { displayMinorTicks = false; }

    bool getDisplayMajorTicks() { return displayMajorTicks; }
    void setDisplayMajorTicks(bool d) { displayMajorTicks = d; }
    void displayMajorTicksOn() { displayMajorTicks = true; }
    void displayMajorTicksOff() { displayMajorTicks = false; }

    bool getDisplayLabels() { return displayLabels; }
    void setDisplayLabels(bool d) { displayLabels = d; }
    void displayLabelsOn() { displayLabels = true; }
    void displayLabelsOff() { displayLabels = false; }
private:
    LineChart *chart;

    int axisType;

    bool display;
    bool displayLabels;

    float minValue, maxValue;
    float minorTickSpacing, majorTickSpacing;
    float minorTickLength, majorTickLength;
    bool displayMinorTicks, displayMajorTicks;

    float valueToPosition(float axisLength, float value);
    float positionToValue(float axisLength, float position);

    void drawTicks(float axisLength, float tickSpacing, float tickLength);
    
    void drawLabels();
    
    std::string getLabel(float value);
};

#endif /* LINECHARTAXIS_H_ */