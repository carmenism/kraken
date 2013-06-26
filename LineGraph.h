#ifndef LINEGRAPH_H_
#define LINEGRAPH_H_

#include <vector>
#include <QList>
#include "GraphMarkerSeries.h"

class GraphAxis;
class GraphLegend;
class QStringList;

typedef std::vector<GraphMarkerSeries *> GraphMarkerSeriesList;
typedef std::vector<GraphMarkerSeries *>::const_iterator GraphMarkerSeriesIterator;

#define FOREACH_MARKERSERIES(it, graphMarkerSeriesList) \
    for(GraphMarkerSeriesIterator it = graphMarkerSeriesList.begin(); it != graphMarkerSeriesList.end(); ++it)
#define FOREACH_MARKERSERIESP(it, graphMarkerSeriesList) \
    for(GraphMarkerSeriesIterator it = graphMarkerSeriesList->begin(); it != graphMarkerSeriesList->end(); ++it)

class LineGraph {
public:
    LineGraph();
    LineGraph(QList<QList<double>> matrix, QStringList labels);

    ~LineGraph();

    void setValues(QList<QList<double>> matrix);

    void draw();
    void drawToPick();
    void drawToPickLines();

    float getWidth() { return width; }
    void setWidth(float w) { width = w; }

    float getHeight() { return height; }
    void setHeight(float h) { height = h; }
    
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

    GraphMarkerList *getMarkers();

    static LineGraph *createGraph(QList<QList<double>> matrix, QStringList labels);
private:
    GraphAxis *axisX;
    GraphAxis *axisY;
    GraphLegend *legend;

    float width, height;
    GraphMarkerSeriesList seriesList;

    bool displayLegend;
    
    float globalMinX, globalMaxX;
    float globalMinY, globalMaxY;

    void calculateGlobalBounds();
    void drawBoundary();
    void drawLines();
    void drawLabels();
    void drawAxes();

    float round(float num);
    float f(float num, float c);
    float roundDown(float num);
    float g(float num, float c);
    float roundUp(float num);

    float calculateIntervalSize(float min, float max);
};

#endif /*LINEGRAPH_H_*/