#ifndef _MONTECARLOPLOTMANAGER_H
#define _MONTECARLOPLOTMANAGER_H

#include <QtOpenGL/QGLWidget>
#include <vector>
#include <QList>
#include <string>
#include <QStringList>
#include "PlotBySpeciesManager.h"
#include "ChartPointSeries.h"

class LineChart;
class MonteCarloLineChart;

class MonteCarloPlotManager : public PlotBySpeciesManager {
public:
    MonteCarloPlotManager(); 
    ~MonteCarloPlotManager();

    void draw(float windowWidth, float windowHeight);
    virtual void drawToPick();

    void removeMarkers();

    void captureLastValues();
    virtual std::vector<LineChart *> *getCharts();
    virtual void updateCharts(Model *model, MS_PROD_MainWindow *mainWindow);

    void addValues(int simNum, MS_PROD_MainWindow *mainWindow, bool jittered);

    void updateStatistics();

    void setDisplayStreaks(bool d);
    void displayStreaksOn();
    void displayStreaksOff();

    void setDisplayOriginalLine(bool d);
    void displayOriginalLineOn();
    void displayOriginalLineOff();

    void setDisplayBoxPlots(bool d);
    void displayBoxPlotsOn();
    void displayBoxPlotsOff();
    
    void setDisplayErrorBands(bool d);
    void displayErrorBandsOn();
    void displayErrorBandsOff();
    
    void setDisplayErrorBars(bool d);
    void displayErrorBarsOn();
    void displayErrorBarsOff();

    void displayBoxPlots();
    void displayStreaks();
    void displayErrorBands();
    void displayErrorBars();
private:      
    std::vector<MonteCarloLineChart *> *charts;

    void initializeCharts(QList<QList<double> *> *biomassMatrix, MS_PROD_MainWindow *mainWindow);
};

#endif  /* _MONTECARLOPLOTMANAGER_H */