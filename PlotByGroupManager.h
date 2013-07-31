#ifndef _PLOTBYGROUPMANAGER_H
#define _PLOTBYGROUPMANAGER_H

#include <QtOpenGL/QGLWidget>
#include <vector>
#include <QList>
#include <QStringList>
#include "PlotManager.h"

class MS_PROD_MainWindow;
class MultiSpeciesLineChart;

#include "ChartPointSeries.h"

class PlotByGroupManager {//: PlotManager {
public:
    PlotByGroupManager() {}
    //PlotByGroupManager(QList<QList<double>> matrix, QStringList labels, MS_PROD_MainWindow *mainWindow);
    ~PlotByGroupManager() {}

    void draw();
    void drawToPick();

    bool empty() { return charts.empty(); }

    void updateCharts(QList<QList<double>> matrix, QStringList labels, MS_PROD_MainWindow *mainWindow);

    void captureLastValues();
    ChartPointList getPoints();
protected:

private:
    std::vector<MultiSpeciesLineChart *> charts;

    void initializeCharts(QList<QList<double>> matrix, QStringList labels, MS_PROD_MainWindow *mainWindow);
};

#endif  /* _PLOTBYGROUPMANAGER_H */