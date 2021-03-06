#ifndef LINECHART_H_
#define LINECHART_H_

#include <vector>
#include "ChartPointSeries.h"
#include "Chart2D.h"

class LineChartAxis;
class LineChartLegend;
class Pickable;

typedef std::vector<ChartPointSeries *> ChartPointSeriesList;
typedef std::vector<ChartPointSeries *>::const_iterator ChartPointSeriesIterator;

#define FOREACH_POINTSERIES(it, chartPointSeriesList) \
    for(ChartPointSeriesIterator it = chartPointSeriesList.begin(); it != chartPointSeriesList.end(); ++it)
#define FOREACH_POINTSERIESP(it, chartPointSeriesList) \
    for(ChartPointSeriesIterator it = chartPointSeriesList->begin(); it != chartPointSeriesList->end(); ++it)

class LineChart : public Chart2D {
public:
    LineChart();
    ~LineChart();

    virtual void drawAtOrigin();
    void drawToPickAtOrigin();
    void drawToPickLines();

    void capturePreviousValues();

    virtual void addPointSeries(ChartPointSeries *s);
    ChartPointSeriesList *getPointSeriesList() { return seriesList; }

    void setLineWidths(float w);

    void setDisplayMarkers(bool d);
    void displayMarkersOn();
    void displayMarkersOff();

    void setMarkersSize(float s);

    void displayAsAreas();
    void displayAsLines();

    LineChartAxis *getBottomAxis();
    LineChartAxis *getTopAxis();
    LineChartAxis *getLeftAxis();
    LineChartAxis *getRightAxis();

    bool getBottomAxisDisplay();
    bool getTopAxisDisplay();
    bool getLeftAxisDisplay();
    bool getRightAxisDisplay();

    void setBottomAxisDisplay(bool d);
    void setTopAxisDisplay(bool d);
    void setLeftAxisDisplay(bool d);
    void setRightAxisDisplay(bool d);

    virtual std::vector<Pickable *> *getPickables();
    
    void setAxesFontHeight(float h);
    void setLegendFontHeight(float h);

    float getOffsetX() { return offsetX; }
    float getOffsetY() { return offsetY; }

    float getInnerWidth() { return innerWidth; }
    float getInnerHeight() { return innerHeight; }
    
    virtual void setWidth(float w);
    virtual void setHeight(float h);

    bool getAdjustYAxisToData() { return adjustYAxisToData; }
    void setAdjustYAxisToData(bool a) { adjustYAxisToData = a; }

    float getLegendWidth();

    ChartPointSeriesList *getSeriesList() { return seriesList; }

    bool getDisplayGhost() { return displayGhost; }
    void setDisplayGhost(bool d) { displayGhost = d; }
    void displayGhostOn() { displayGhost = true; }
    void displayGhostOff() { displayGhost = false; }

    bool getDisplayGhostAsLine() { return displayGhostAsLine; }
    void setDisplayGhostAsLine(bool d) { displayGhostAsLine = d; }
    void displayGhostAsALine() { displayGhostAsLine = true; }
    void displayGhostAsABlend() { displayGhostAsLine = false; }
    
    float getLargestFinalValue();
protected:
    int numberAxes;
    bool adjustYAxisToData;
    bool displayGhost, displayGhostAsLine;
    ChartPointSeriesList *seriesList;
    LineChartAxis **axes;
    float offsetX, offsetY;
    float innerWidth, innerHeight;
    void updateActualSize();
    LineChartLegend *legend;
private:
    virtual void calculateGlobalBounds();
    void drawBoundary();
    void drawLines();
    void drawSelected();
    void drawAxes();
    void drawXAxis(LineChartAxis *axisX);
    void drawYAxis(LineChartAxis *axisY);

    void setUpAxes();

    std::vector<Pickable *> *allPickables;
};

#endif /*LINECHART_H_*/