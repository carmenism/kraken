#ifndef SINGLESPECIESLINECHART_H_
#define SINGLESPECIESLINECHART_H_

class Color;
class AbsoluteSizesChart;
class AbsoluteSizeIndicator;

#include "LineChart.h"
#include <vector>
#include <string>

class SingleSpeciesLineChart : public LineChart {
public:
    SingleSpeciesLineChart();
    ~SingleSpeciesLineChart();
    SingleSpeciesLineChart(std::vector<float> x, std::vector<float> y, std::string label, bool displayXAxisLabels, int numGuilds, int guildIndex);
    void setValues(std::vector<float> x, std::vector<float> y);
    void drawAtOrigin();
    virtual void draw();
    virtual void drawToPick();
    Color *getColor();
    
    std::vector<AbsoluteSizeIndicator *> *getAbsPoints();

    bool getDisplayChart() { return displayChart; }
    void setDisplayChart(bool d) { displayChart = d; }
    void displayChartOn() { displayChart = true; }
    void displayChartOff() { displayChart = false; }

    bool getDisplayAbsoluteSizes() { return displayAbsoluteSizes; }
    void setDisplayAbsoluteSizes(bool d) { displayAbsoluteSizes = d; }
    void displayAbsoluteSizesOn() { displayAbsoluteSizes = true; }
    void displayAbsoluteSizesOff() { displayAbsoluteSizes = false; }
private:
    bool displayAbsoluteSizes;
    bool displayChart;
    std::string sideLabel;
    AbsoluteSizesChart *absChart;
};

#endif  SINGLESPECIESLINECHART_H_ 