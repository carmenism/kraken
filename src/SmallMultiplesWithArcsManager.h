#ifndef _SMALLMULTIPLESWITHARCSMANAGER_H
#define _SMALLMULTIPLESWITHARCSMANAGER_H

#include <QtOpenGL/QGLWidget>
#include <vector>
#include <QList>
#include <string>
#include <QStringList>
#include "SmallMultiplesManager.h"
#include "ChartPointSeries.h"

class AbsoluteSizeIndicator;
class SmallMultiple;
class InteractionArc;
class PredationArc;

#include "InterSpeciesArcCollection.h"

class SmallMultiplesWithArcsManager : public SmallMultiplesManager {
public:
    SmallMultiplesWithArcsManager(); 
    ~SmallMultiplesWithArcsManager();

    void draw(float windowWidth, float windowHeight);
    virtual void drawToPick();

    void capturePreviousValues();
    //ChartPointList *getPoints();
    InterSpeciesArcList *getArcs();    
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

    bool getArcsAnimated();
    void setArcsAnimated(bool a);
    void arcsAnimatedOn();
    void arcsAnimatedOff();

    SmallMultiple *getChartAt(int i) { return charts->at(i); }
private:   
    QList<double> percentChangeInFinalBiomass;

    InterSpeciesArcCollection *arcsInter;
    InterSpeciesArcCollection *arcsPred;
    InterSpeciesArcCollection *arcsBoth;
    InterSpeciesArcCollection *arcsCurrent;

    std::vector<SmallMultiple *> *charts;
    
    void initializeCharts(QList<QList<double> *> *biomassMatrix, QList<QList<double> *> *harvestMatrix, MS_PROD_MainWindow *mainWindow);
    void initializeArcs(InterSpeciesArcCollection *arcs, int arcType, QList<QList<double>> matix, bool useColorOfChart);
    void initializeInteractionArcs(MS_PROD_MainWindow *mainWindow);
    void initializePredationArcs(MS_PROD_MainWindow *mainWindow);
    void initializeBothArcs(MS_PROD_MainWindow *mainWindow);
};

#endif  /* _SMALLMULTIPLESWITHARCSMANAGER_H */