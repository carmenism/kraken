#ifndef _FOURPANELMANAGER_H
#define _FOURPANELMANAGER_H

#include <QtOpenGL/QGLWidget>
#include <vector>
#include "PlotManager.h"

class MS_PROD_MainWindow;
class MultiSpeciesLineChart;

#include "ChartPointSeries.h"

class FourPanelManager : public PlotManager {
public:
    FourPanelManager();
    ~FourPanelManager();

    void draw(float windowWidth, float windowHeight);
    void updateCharts(Model *model, MS_PROD_MainWindow *mainWindow);
    
    MultiSpeciesLineChart *getChartAt(int i) { return charts->at(i); }
protected:

private:
    std::vector<MultiSpeciesLineChart *> *charts;

    void initializeCharts(QList<QList<double>> matrix, QStringList labels, MS_PROD_MainWindow *mainWindow);
};

#endif /* _FOURPANELMANAGER_H */