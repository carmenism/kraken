#ifndef SINGLESPECIESLINECHART_H_
#define SINGLESPECIESLINECHART_H_

#include "LineChart.h"
#include <vector>
#include <string>

class SingleSpeciesLineChart : public LineChart {
public:
    SingleSpeciesLineChart();
    SingleSpeciesLineChart(std::vector<float> x, std::vector<float> y, std::string label, bool displayXAxisLabels, int numGuilds, int guildIndex);
    void setValues(std::vector<float> x, std::vector<float> y);
    void drawAtOrigin();
private:
    std::string sideLabel;
};

#endif  SINGLESPECIESLINECHART_H_ 