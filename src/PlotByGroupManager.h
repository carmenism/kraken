#ifndef _PLOTBYGROUPMANAGER_H
#define _PLOTBYGROUPMANAGER_H

#include <QtOpenGL/QGLWidget>
#include <vector>
#include "PlotManager.h"

class MS_PROD_MainWindow;
class MultiSpeciesLineChart;

#include "ChartPointSeries.h"

class PlotByGroupManager : public PlotManager {
public:
    PlotByGroupManager();
    ~PlotByGroupManager();

    void draw(float windowWidth, float windowHeight);
    void updateCharts(Model *model, MS_PROD_MainWindow *mainWindow);

    std::vector<LineChart *> *getCharts();
    
    MultiSpeciesLineChart *getChartAt(int i) { return charts->at(i); }
protected:

private:
    std::vector<MultiSpeciesLineChart *> *charts;

    void initializeCharts(QList<QList<double>> matrix, QStringList labels, MS_PROD_MainWindow *mainWindow);
};

#endif  /* _PLOTBYGROUPMANAGER_H */