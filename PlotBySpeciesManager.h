#ifndef _PLOTBYSPECIESMANAGER_H
#define _PLOTBYSPECIESMANAGER_H

#include <QtOpenGL/QGLWidget>
#include <vector>
#include <QList>
#include <QStringList>
#include "PlotManager.h"
#include "ChartPointSeries.h"

class AbsoluteSizeIndicator;
class SingleSpeciesLineChart;
class BetweenSpeciesArc;
class InteractionArc;
class PredationArc;

typedef std::vector<BetweenSpeciesArc *> BetweenSpeciesArcList;
typedef std::vector<BetweenSpeciesArc *>::const_iterator BetweenSpeciesArcIterator;

#define FOREACH_BETWEENSPECIESARC(it, arcList) \
    for(BetweenSpeciesArcIterator it = arcList.begin(); it != arcList.end(); ++it)
#define FOREACH_BETWEENSPECIESARCP(it, arcList) \
    for(BetweenSpeciesArcIterator it = arcList->begin(); it != arcList->end(); ++it)

class PlotBySpeciesManager : public PlotManager {
public:
    PlotBySpeciesManager(); 
    ~PlotBySpeciesManager();

    void draw();
    virtual void drawToPick();

    void captureLastValues();
    ChartPointList *getPoints();
    BetweenSpeciesArcList *getArcs();    
    std::vector<LineChart *> *getCharts();
    void updateCharts(QList<QList<double>> matrix, QStringList labels, MS_PROD_MainWindow *mainWindow);

    std::vector<AbsoluteSizeIndicator *> *getAbsPoints();

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
   
    BetweenSpeciesArcList *arcsInter;
    BetweenSpeciesArcList *arcsPred;
    BetweenSpeciesArcList *currentArcs;

    std::vector<SingleSpeciesLineChart *> *charts;

    void initializeCharts(QList<QList<double>> matrix, QStringList labels, MS_PROD_MainWindow *mainWindow);
    void initializeInteractionArcs(MS_PROD_MainWindow *mainWindow);
    void initializePredationArcs(MS_PROD_MainWindow *mainWindow);

    BetweenSpeciesArc *drawArcs();
};

#endif  /* _PLOTBYSPECIESMANAGER_H */