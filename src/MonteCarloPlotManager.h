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

    void captureLastValues();
    virtual std::vector<LineChart *> *getCharts();
    virtual void updateCharts(Model *model, MS_PROD_MainWindow *mainWindow);

    void addJitteredValues(MS_PROD_MainWindow *mainWindow);
private:      
    std::vector<MonteCarloLineChart *> *charts;

    void initializeCharts(QList<QList<double> *> *biomassMatrix, MS_PROD_MainWindow *mainWindow);
};

#endif  /* _MONTECARLOPLOTMANAGER_H */