#ifndef _PLOTBYSPECIESMANAGER_H
#define _PLOTBYSPECIESMANAGER_H

#include <QtOpenGL/QGLWidget>
#include <vector>
#include <QList>
#include <QStringList>
#include "PlotManager.h"
#include "ChartPointSeries.h"

class SingleSpeciesLineChart;
class InteractionArc;

typedef std::vector<InteractionArc *> InteractionArcList;
typedef std::vector<InteractionArc *>::const_iterator InteractionArcIterator;

#define FOREACH_INTERACTIONARC(it, arcList) \
    for(InteractionArcIterator it = arcList.begin(); it != arcList.end(); ++it)
#define FOREACH_INTERACTIONARCP(it, arcList) \
    for(InteractionArcIterator it = arcList->begin(); it != arcList->end(); ++it)

class PlotBySpeciesManager : public PlotManager {
public:
    PlotBySpeciesManager(); 
    ~PlotBySpeciesManager() {}

    void draw();
    virtual void drawToPick();

    void captureLastValues();
    ChartPointList getPoints();
    InteractionArcList getArcs();    
    std::vector<LineChart *> getCharts();
    void updateCharts(QList<QList<double>> matrix, QStringList labels, MS_PROD_MainWindow *mainWindow);

    bool getDisplayCharts();
    void setDisplayCharts(bool d);
    void displayChartsOn();
    void displayChartsOff();

    bool getDisplayAbsoluteSizes();
    void setDisplayAbsoluteSizes(bool d);
    void displayAbsoluteSizesOn();
    void displayAbsoluteSizesOff();
protected:

private:
    QList<int> oldIndices;
    QStringList newLabels;
    QStringList getNewLabels(QStringList labels, QList<int> oldIndices);
    QList<int> getNewOrder(QStringList labels, MS_PROD_MainWindow *mainWindow);
    QList<QList<double>> getNewTimeSeriesMatrix(QList<QList<double>> matrix, QList<int> oldIndices);
    QList<QList<double>> getNewSquareMatrix(QList<QList<double>> matrix, QList<int> oldIndices);
   
    InteractionArcList arcs;
    std::vector<SingleSpeciesLineChart *> charts;

    void initializeCharts(QList<QList<double>> matrix, QStringList labels, MS_PROD_MainWindow *mainWindow);
};

#endif  /* _PLOTBYSPECIESMANAGER_H */