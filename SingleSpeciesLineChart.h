#ifndef SINGLESPECIESLINECHART_H_
#define SINGLESPECIESLINECHART_H_

#include "LineChart.h"
#include <vector>
#include <string>

class SingleSpeciesLineChart : public LineChart {
public:
    SingleSpeciesLineChart();
    SingleSpeciesLineChart(std::vector<float> x, std::vector<float> y, std::string label);
    void setValues(std::vector<float> x, std::vector<float> y);
};

#endif  SINGLESPECIESLINECHART_H_ 