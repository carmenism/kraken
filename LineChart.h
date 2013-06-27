#ifndef LINECHART_H_
#define LINECHART_H_

#include <vector>
#include <QList>
#include "GraphMarkerSeries.h"
#include "Chart.h"

class GraphAxis;
class GraphLegend;
class QStringList;

typedef std::vector<GraphAxis *> GraphAxisList;
typedef std::vector<GraphAxis *>::const_iterator GraphAxesIterator;

#define FOREACH_GRAPHAXIS(it, graphAxisList) \
    for(GraphAxesIterator it = graphAxisList.begin(); it != graphAxisList.end(); ++it)
#define FOREACH_GRAPHAXISP(it, graphAxisList) \
    for(GraphAxesIterator it = graphAxisList->begin(); it != graphAxisList->end(); ++it)

typedef std::vector<GraphMarkerSeries *> GraphMarkerSeriesList;
typedef std::vector<GraphMarkerSeries *>::const_iterator GraphMarkerSeriesIterator;

#define FOREACH_MARKERSERIES(it, graphMarkerSeriesList) \
    for(GraphMarkerSeriesIterator it = graphMarkerSeriesList.begin(); it != graphMarkerSeriesList.end(); ++it)
#define FOREACH_MARKERSERIESP(it, graphMarkerSeriesList) \
    for(GraphMarkerSeriesIterator it = graphMarkerSeriesList->begin(); it != graphMarkerSeriesList->end(); ++it)

class LineChart : public Chart {
public:
    LineChart();
    LineChart(QList<QList<double>> matrix, QStringList labels);

    ~LineChart();

    void setValues(QList<QList<double>> matrix);

    void draw();
    void drawToPick();
    void drawToPickLines();
    
    void addMarkerSeries(GraphMarkerSeries *s);
    void clearMarkerSeriesList() { seriesList.clear(); }
    GraphMarkerSeriesList getMarkerSeriesList() { return seriesList; }

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

    GraphAxis *getBottomAxis();
    GraphAxis *getTopAxis();
    GraphAxis *getLeftAxis();
    GraphAxis *getRightAxis();

    bool getBottomAxisDisplay();
    bool getTopAxisDisplay();
    bool getLeftAxisDisplay();
    bool getRightAxisDisplay();

    void setBottomAxisDisplay(bool d);
    void setTopAxisDisplay(bool d);
    void setLeftAxisDisplay(bool d);
    void setRightAxisDisplay(bool d);

    ChartPointList *getPoints();

    static LineChart *createGraph(QList<QList<double>> matrix, QStringList labels);
private:
    GraphAxisList axes;
    GraphLegend *legend;

    GraphMarkerSeriesList seriesList;

    bool displayLegend;
    
    float globalMinX, globalMaxX;
    float globalMinY, globalMaxY;

    void calculateGlobalBounds();
    void drawBoundary();
    void drawLines();
    void drawLabels();
    void drawAxes();
    void drawXAxis(GraphAxis *axisX);
    void drawYAxis(GraphAxis *axisY);

    float calculateIntervalSize(float min, float max);

    void setUpAxes();
};

#endif /*LINECHART_H_*/