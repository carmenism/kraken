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
class InteractionArc;
class PredationArc;

#include "BetweenSpeciesArcCollection.h"

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
    void updateCharts(Model *model, MS_PROD_MainWindow *mainWindow);

    std::vector<AbsoluteSizeIndicator *> *getAbsPoints();

    bool getDisplayCharts();
    void setDisplayCharts(bool d);
    void displayChartsOn();
    void displayChartsOff();

    bool getDisplayAbsoluteSizes();
    void setDisplayAbsoluteSizes(bool d);
    void displayAbsoluteSizesOn();
    void displayAbsoluteSizesOff();

    void displayPredation();
    void displayInteraction();
    void displayNoArcs();
private:
    QList<int> oldIndices;
    QStringList newLabels;
   
    BetweenSpeciesArcCollection *arcsInter;
    BetweenSpeciesArcCollection *arcsPred;
    BetweenSpeciesArcCollection *arcsCurrent;

    std::vector<SingleSpeciesLineChart *> *charts;
    
    QStringList getNewLabels(QStringList labels, QList<int> oldIndices);
    QList<int> getNewOrder(QStringList labels, MS_PROD_MainWindow *mainWindow);
    QList<QList<double>> getNewTimeSeriesMatrix(QList<QList<double>> matrix, QList<int> oldIndices);
    QList<QList<double>> getNewSquareMatrix(QList<QList<double>> matrix, QList<int> oldIndices);

    void initializeCharts(QList<QList<double>> matrix, QStringList labels, MS_PROD_MainWindow *mainWindow);
    BetweenSpeciesArcCollection *initializeArcs(int arcType, QList<QList<double>> matix);
    void initializeInteractionArcs(MS_PROD_MainWindow *mainWindow);
    void initializePredationArcs(MS_PROD_MainWindow *mainWindow);
};

#endif  /* _PLOTBYSPECIESMANAGER_H */