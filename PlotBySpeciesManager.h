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

class PlotBySpeciesManager : public PlotManager {
public:
    PlotBySpeciesManager(); //: PlotManager() {}
    ~PlotBySpeciesManager() {}

    virtual void draw();

    void captureLastValues();
    ChartPointList getPoints();
    
    std::vector<LineChart *> getCharts();
    void updateCharts(QList<QList<double>> matrix, QStringList labels, MS_PROD_MainWindow *mainWindow);
protected:

private:
    std::vector<InteractionArc *> arcs;
    std::vector<SingleSpeciesLineChart *> charts;

    void initializeCharts(QList<QList<double>> matrix, QStringList labels, MS_PROD_MainWindow *mainWindow);
};

#endif  /* _PLOTBYSPECIESMANAGER_H */