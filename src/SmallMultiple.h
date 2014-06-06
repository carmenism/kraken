#ifndef SMALLMULTIPLE_H_
#define SMALLMULTIPLE_H_

class Color;
class AbsoluteSizesChart;
class AbsoluteSizeIndicator;
class Pickable;

#include "LineChart.h"
#include <vector>
#include <string>

class SmallMultiple : public LineChart {
public:
    SmallMultiple();
    ~SmallMultiple();
    SmallMultiple(std::vector<float> *xValues, std::vector<float> *yBiomass, std::string label, bool displayXAxisLabels, int numGuilds, int guildIndex);
    void setValues(std::vector<float> *xValues, std::vector<float> *yBiomass);
    void drawAtOrigin();
    virtual void draw();
    virtual void drawToPick();
    Color *getColor();
    
    AbsoluteSizeIndicator **getAbsPoints();
    int getAbsSize();

    bool getDisplayChart() { return displayChart; }
    void setDisplayChart(bool d) { displayChart = d; }
    void displayChartOn() { displayChart = true; }
    void displayChartOff() { displayChart = false; }

    bool getDisplayAbsoluteSizes() { return displayAbsoluteSizes; }
    void setDisplayAbsoluteSizes(bool d) { displayAbsoluteSizes = d; }
    void displayAbsoluteSizesOn() { displayAbsoluteSizes = true; }
    void displayAbsoluteSizesOff() { displayAbsoluteSizes = false; }

    void setAbsLegendLocation(float x, float y);

    float getStartValue();

    float getPercentIncreaseOfFinalValue();
    float getFinalValue();
    float getPreviousFinalValue();
private:
    bool displayAbsoluteSizes;
    bool displayChart;
    std::string sideLabel;
    AbsoluteSizesChart *absChart;
};

#endif SMALLMULTIPLE_H_ 