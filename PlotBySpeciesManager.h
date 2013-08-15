#ifndef _PLOTBYSPECIESMANAGER_H
#define _PLOTBYSPECIESMANAGER_H

#include <QtOpenGL/QGLWidget>
#include <vector>
#include <QList>
#include <string>
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

    bool getDisplayHarvest();
    void setDisplayHarvest(bool d);
    void displayHarvestOn();
    void displayHarvestOff();

    void displayPredation();
    void displayInteraction();
    void displayNoArcs();

    SingleSpeciesLineChart *getChartAt(int i) { return charts->at(i); }
private:
    QList<int> *oldIndices;
    QStringList *newLabels;
   
    BetweenSpeciesArcCollection *arcsInter;
    BetweenSpeciesArcCollection *arcsPred;
    BetweenSpeciesArcCollection *arcsCurrent;

    std::vector<SingleSpeciesLineChart *> *charts;
    
    QStringList *getNewLabels(QStringList labels);
    QList<int> *getNewOrder(QStringList labels, MS_PROD_MainWindow *mainWindow);
    QList<QList<double> *> *getNewTimeSeriesMatrix(QList<QList<double>> matrix);
    QList<QList<double> *> *getNewSquareMatrix(QList<QList<double>> matrix);

    void initializeCharts(QList<QList<double> *> *biomassMatrix, QList<QList<double> *> *harvestMatrix, MS_PROD_MainWindow *mainWindow);
    BetweenSpeciesArcCollection *initializeArcs(std::string title, int arcType, QList<QList<double>> matix);
    void initializeInteractionArcs(MS_PROD_MainWindow *mainWindow);
    void initializePredationArcs(MS_PROD_MainWindow *mainWindow);
};

#endif  /* _PLOTBYSPECIESMANAGER_H */