#ifndef SMALLMULTIPLE_H_
#define SMALLMULTIPLE_H_

class Color;
class AbsoluteSizesChart;
class AbsoluteSizeIndicator;

#include "LineChart.h"
#include <vector>
#include <string>

class SmallMultiple : public LineChart {
public:
    SmallMultiple();
    ~SmallMultiple();
    SmallMultiple(std::vector<float> *x, std::vector<float> *yBiomass, std::vector<float> *yHarvest, std::string label, bool displayXAxisLabels, int numGuilds, int guildIndex);
    void setValues(std::vector<float> *x, std::vector<float> *yBiomass, std::vector<float> *yHarvest);
    void drawAtOrigin();
    virtual void draw();
    virtual void drawToPick();
    Color *getColor();
    
    std::vector<AbsoluteSizeIndicator *> *getAbsPoints();

    //virtual void addPointSeries(ChartPointSeries *s) {}

    bool getDisplayChart() { return displayChart; }
    void setDisplayChart(bool d) { displayChart = d; }
    void displayChartOn() { displayChart = true; }
    void displayChartOff() { displayChart = false; }

    bool getDisplayAbsoluteSizes() { return displayAbsoluteSizes; }
    void setDisplayAbsoluteSizes(bool d) { displayAbsoluteSizes = d; }
    void displayAbsoluteSizesOn() { displayAbsoluteSizes = true; }
    void displayAbsoluteSizesOff() { displayAbsoluteSizes = false; }

    bool getDisplayHarvest() { return displayHarvest; }
    void setDisplayHarvest(bool d) { displayHarvest = d; }
    void displayHarvestOn() { displayHarvest = true; }
    void displayHarvestOff() { displayHarvest = false; }

    virtual ChartPointList *getPoints();

    void setAbsLegendLocation(float x, float y);

    float getPercentIncreaseOfFinalValue();
    float getFinalValue();
    float getPreviousFinalValue();
private:
    bool displayAbsoluteSizes;
    bool displayChart;
    bool displayHarvest;
    std::string sideLabel;
    AbsoluteSizesChart *absChart;
    LineChart *harvest;
};

#endif SMALLMULTIPLE_H_ 