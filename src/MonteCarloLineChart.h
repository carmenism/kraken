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

    bool getDisplayOriginalLine();
    void setDisplayOriginalLine(bool d);
    void displayOriginalLineOn();
    void displayOriginalLineOff();

    bool getDisplayStreaks() { return displayStreaks; }
    void setDisplayStreaks(bool d) { displayStreaks = d; }
    void displayStreaksOn() { displayStreaks = true; }
    void displayStreaksOff() { displayStreaks = false; }
    
    bool getDisplayBoxPlots();
    void setDisplayBoxPlots(bool d);
    void displayBoxPlotsOn();
    void displayBoxPlotsOff();
    
    bool getDisplayErrorBars();
    void setDisplayErrorBars(bool d);
    void displayErrorBarsOn();
    void displayErrorBarsOff();
    
    bool getDisplayErrorBands();
    void setDisplayErrorBands(bool d);
    void displayErrorBandsOn();
    void displayErrorBandsOff();

    bool getDisplayMedianLine();
    void setDisplayMedianLine(bool d);
    void displayMedianLineOn();
    void displayMedianLineOff();

    bool getDisplayMeanLine();
    void setDisplayMeanLine(bool d);
    void displayMeanLineOn();
    void displayMeanLineOff();

    void useQuartiles();
    void useStandardDeviations();
    
    bool getUsingQuartiles();
    bool getUsingStandardDeviations();
private:       
    //bool displayOriginalLine;
    bool displayStreaks;

    Statistics *stats;
    Color *semiTransparentColor;
    //Color *originalLineColor;
    std::string sideLabel;

    virtual void calculateGlobalBounds();
};

#endif MONTECARLOLINECHART_H_ 