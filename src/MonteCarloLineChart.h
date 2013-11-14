#ifndef MONTECARLOLINECHART_H_
#define MONTECARLOLINECHART_H_

class Color;
class Statistics;

#include "LineChart.h"
#include <vector>
#include <string>

class MonteCarloLineChart : public LineChart {
public:
    MonteCarloLineChart(std::string label, bool displayXAxisLabels, int numGuilds, int guildIndex);
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

    void updateStatistics();

    bool getDisplayOriginalLine() { return displayOriginalLine; }
    void setDisplayOriginalLine(bool d) { displayOriginalLine = d; }
    void displayOriginalLineOn() { displayOriginalLine = true; }
    void displayOriginalLineOff() { displayOriginalLine = false; }

    bool getDisplayStreaks() { return displayStreaks; }
    void setDisplayStreaks(bool d) { displayStreaks = d; }
    void displayStreaksOn() { displayStreaks = true; }
    void displayStreaksOff() { displayStreaks = false; }
    void setDisplayStatistics(bool d);
    void displayStatisticsOn();
    void displayStatisticsOff();
private:       
    bool displayOriginalLine;
    bool displayStreaks;

    Statistics *stats;
    Color *semiTransparentColor;
    std::string sideLabel;

    virtual void calculateGlobalBounds();
};

#endif MONTECARLOLINECHART_H_ 