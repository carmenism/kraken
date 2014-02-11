#ifndef LINECHARTAXIS_H_
#define LINECHARTAXIS_H_

#include <string>

class LineChart;

enum AxisType {BOTTOM, TOP, LEFT, RIGHT};

#define OFFSET 5

class LineChartAxis
{
public:
    LineChartAxis(LineChart *chart, AxisType axisType = BOTTOM);
    ~LineChartAxis();

    void draw();

    float getSize();

    float getMinorTickSpacing() { return minorTickSpacing; }
    void setMinorTickSpacing(float m) { minorTickSpacing = m; }

    float getMajorTickSpacing() { return majorTickSpacing; }
    void setMajorTickSpacing(float m) { majorTickSpacing = m; }

    float getMinorTickLength() { return minorTickLength; }
    void setMinorTickLength(float len) { minorTickLength = len; }

    float getMajorTickLength() { return majorTickLength; }
    void setMajorTickLength(float len) { majorTickLength = len; }

    float getMinimumValue() { return minValue; }
    void setMinimumValue(float m);

    float getMaximumValue() { return maxValue; }
    void setMaximumValue(float m);

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

    bool getDisplayTickLabels() { return displayTickLabels; }
    void setDisplayTickLabels(bool d) { displayTickLabels = d; }
    void displayTickLabelsOn() { displayTickLabels = true; }
    void displayTickLabelsOff() { displayTickLabels = false; }

    bool getDisplayLabel() { return displayLabel; }
    void setDisplayLabel(bool d) { displayLabel = d; }
    void displayLabelOn() { displayLabel = true; }
    void displayLabelOff() { displayLabel = false; }

    std::string getLabel() { return label; }
    void setLabel(std::string s) { label = s; }
    
    float getFontHeight() { return fontHeight; }
    void setFontHeight(float h) { fontHeight = h; }
private:
    LineChart *chart;

    AxisType axisType;

    void *font;

    bool display;
    bool displayTickLabels;
    bool displayLabel;

    std::string label;

    float minValue, maxValue;
    float minorTickSpacing, majorTickSpacing;
    float minorTickLength, majorTickLength;
    bool displayMinorTicks, displayMajorTicks;

    float fontHeight;

    float valueToPosition(float axisLength, float value);
    float positionToValue(float axisLength, float position);

    void drawTicks(float axisLength, float tickSpacing, float tickLength);
    
    void drawTickLabels(float axisLength);
    void drawLabel(float axisLength);

    float round(float num);
    float roundDown(float num);
    float roundUp(float num);
    float f(float num, float c);
    float g(float num, float c);
    float calculateIntervalSize(float axisLength);
};

#endif /* LINECHARTAXIS_H_ */