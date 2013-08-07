#ifndef SINGLESPECIESLINECHART_H_
#define SINGLESPECIESLINECHART_H_

class Color;
class AbsoluteSizesChart;

#include "LineChart.h"
#include <vector>
#include <string>

class SingleSpeciesLineChart : public LineChart {
public:
    SingleSpeciesLineChart();
    SingleSpeciesLineChart(std::vector<float> x, std::vector<float> y, std::string label, bool displayXAxisLabels, int numGuilds, int guildIndex);
    void setValues(std::vector<float> x, std::vector<float> y);
    void drawAtOrigin();
    virtual void draw();
    Color *getColor();
private:
    std::string sideLabel;
    AbsoluteSizesChart *absChart;
};

#endif  SINGLESPECIESLINECHART_H_ 