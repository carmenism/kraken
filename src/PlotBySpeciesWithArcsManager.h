#ifndef _PLOTBYSPECIESWITHARCSMANAGER_H
#define _PLOTBYSPECIESWITHARCSMANAGER_H

#include <QtOpenGL/QGLWidget>
#include <vector>
#include <QList>
#include <string>
#include <QStringList>
#include "PlotBySpeciesManager.h"
#include "ChartPointSeries.h"

class AbsoluteSizeIndicator;
class SingleSpeciesLineChart;
class InteractionArc;
class PredationArc;

#include "BetweenSpeciesArcCollection.h"

class PlotBySpeciesWithArcsManager : public PlotBySpeciesManager {
public:
    PlotBySpeciesWithArcsManager(); 
    ~PlotBySpeciesWithArcsManager();

    void draw(float windowWidth, float windowHeight);
    virtual void drawToPick();

    void capturePreviousValues();
    //ChartPointList *getPoints();
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
    void displayBothArcs();
    void displayNoArcs();
    
    void setDisplayArcsDynamically(bool d);
    void displayArcsDynamicallyOn();
    void displayArcsDynamicallyOff();

    SingleSpeciesLineChart *getChartAt(int i) { return charts->at(i); }
private:   
    QList<double> percentChangeInFinalBiomass;

    BetweenSpeciesArcCollection *arcsInter;
    BetweenSpeciesArcCollection *arcsPred;
    BetweenSpeciesArcCollection *arcsBoth;
    BetweenSpeciesArcCollection *arcsCurrent;

    std::vector<SingleSpeciesLineChart *> *charts;
    
    void initializeCharts(QList<QList<double> *> *biomassMatrix, QList<QList<double> *> *harvestMatrix, MS_PROD_MainWindow *mainWindow);
    void initializeArcs(BetweenSpeciesArcCollection *arcs, int arcType, QList<QList<double>> matix, bool useColorOfChart);
    void initializeInteractionArcs(MS_PROD_MainWindow *mainWindow);
    void initializePredationArcs(MS_PROD_MainWindow *mainWindow);
    void initializeBothArcs(MS_PROD_MainWindow *mainWindow);
};

#endif  /* _PLOTBYSPECIESWITHARCSMANAGER_H */