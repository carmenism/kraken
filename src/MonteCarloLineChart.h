#ifndef MONTECARLOLINECHART_H_
#define MONTECARLOLINECHART_H_

class Color;

#include "LineChart.h"
#include <vector>
#include <string>

class MonteCarloLineChart : public LineChart {
public:
    MonteCarloLineChart(std::vector<float> *x, std::vector<float> *y, std::string label, bool displayXAxisLabels, int numGuilds, int guildIndex);
    ~MonteCarloLineChart() {}
    
    virtual void drawAtOrigin();

    void setValues(std::vector<float> *x, std::vector<float> *y);

    void addPointSeries(std::vector<float> *x, std::vector<float> *y);
    void addPointSeries(ChartPointSeries *series);
private:    
    std::string sideLabel;

    void clearValues();
};

#endif MONTECARLOLINECHART_H_ 