#ifndef MONTECARLOLINECHART_H_
#define MONTECARLOLINECHART_H_

class Color;

#include "LineChart.h"
#include <vector>
#include <string>

class MonteCarloLineChart : public LineChart {
public:
    MonteCarloLineChart(std::string label, bool displayXAxisLabels, int numGuilds, int guildIndex);
    //MonteCarloLineChart(std::vector<float> *x, std::vector<float> *y, std::string label, bool displayXAxisLabels, int numGuilds, int guildIndex);
    ~MonteCarloLineChart();
    
    virtual void drawAtOrigin();

    void setValues(std::vector<float> *x, std::vector<float> *y);

    void addSemiTransparentPointSeries(int simNum, std::vector<float> *x, std::vector<float> *y);
    void addSemiTransparentPointSeries(int simNum, ChartPointSeries *series);
    void addBlackPointSeries(int simNum, std::vector<float> *x, std::vector<float> *y);
    void addBlackPointSeries(int simNum, ChartPointSeries *series);
    void addPointSeries(int simNum, std::vector<float> *x, std::vector<float> *y, Color *color);
    void addPointSeries(int simNum, ChartPointSeries *series, Color *color);

    void clearAllValues();
private:    
    Color *semiTransparentColor;
    std::string sideLabel;
};

#endif MONTECARLOLINECHART_H_ 