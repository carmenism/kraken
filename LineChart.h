#ifndef LINECHART_H_
#define LINECHART_H_

#include <vector>
#include <QList>
#include "ChartPointSeries.h"
#include "Chart.h"

class LineChartAxis;
class LineChartLegend;
class QStringList;

typedef std::vector<LineChartAxis *> LineChartAxisList;
typedef std::vector<LineChartAxis *>::const_iterator LineChartAxisIterator;

#define FOREACH_LINECHARTAXIS(it, lineChartAxisList) \
    for(LineChartAxisIterator it = lineChartAxisList.begin(); it != lineChartAxisList.end(); ++it)
#define FOREACH_LINECHARTAXISP(it, lineChartAxisList) \
    for(LineChartAxisIterator it = lineChartAxisList->begin(); it != lineChartAxisList->end(); ++it)

typedef std::vector<ChartPointSeries *> ChartPointSeriesList;
typedef std::vector<ChartPointSeries *>::const_iterator ChartPointSeriesIterator;

#define FOREACH_POINTSERIES(it, chartPointSeriesList) \
    for(ChartPointSeriesIterator it = chartPointSeriesList.begin(); it != chartPointSeriesList.end(); ++it)
#define FOREACH_POINTSERIESP(it, chartPointSeriesList) \
    for(ChartPointSeriesIterator it = chartPointSeriesList->begin(); it != chartPointSeriesList->end(); ++it)

class LineChart : public Chart {
public:
    LineChart();
    LineChart(QList<QList<double>> matrix, QStringList labels);

    ~LineChart();

    void setValues(QList<QList<double>> matrix);

    void draw();
    void drawToPick();
    void drawToPickLines();

    void captureLastValues();

    void addPointSeries(ChartPointSeries *s);
    void clearPointSeriesList() { seriesList.clear(); }
    ChartPointSeriesList getPointSeriesList() { return seriesList; }

    void setLineWidths(float w);

    void setDisplayMarkers(bool d);
    void displayMarkersOn();
    void displayMarkersOff();

    void setMarkersSize(float s);

    void displayAsAreas();
    void displayAsLines();

    bool getDisplayLegend() { return displayLegend; }
    void setDisplayLegend(bool d) { displayLegend = d; }
    void displayLegendOn() { displayLegend = true; }
    void displayLegendOff() { displayLegend = false; }

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

    ChartPointList *getPoints();
    
    float getGlobalMinX() { return globalMinX; }
    float getGlobalMaxX() { return globalMaxX; }
    float getGlobalMinY() { return globalMinY; }
    float getGlobalMaxY() { return globalMaxY; }
private:
    LineChartAxisList axes;
    LineChartLegend *legend;

    ChartPointSeriesList seriesList;

    bool displayLegend;
    
    float offsetX, offsetY;

    float globalMinX, globalMaxX;
    float globalMinY, globalMaxY;

    void calculateGlobalBounds();
    void drawBoundary();
    void drawLines();
    void drawLabels();
    void drawAxes();
    void drawXAxis(LineChartAxis *axisX);
    void drawYAxis(LineChartAxis *axisY);

    float calculateIntervalSize(float min, float max);

    void setUpAxes();
};

#endif /*LINECHART_H_*/