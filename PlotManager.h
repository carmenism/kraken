#ifndef _PLOTMANAGER_H
#define _PLOTMANAGER_H

#include "Displayable.h"
#include "LineChart.h"
#include "ChartPoint.h"
#include <vector>
#include <QList>

class MS_PROD_MainWindow;
class QStringList;

class PlotManager : public Displayable {
    public:
        PlotManager() {  }
        ~PlotManager();
     
        virtual void draw();
        virtual void drawToPick();
        void captureLastValues();
        ChartPointList *getPoints();
        
        bool empty() { return getCharts()->empty(); }

        virtual std::vector<LineChart *> *getCharts() = 0;
        virtual void updateCharts(QList<QList<double>> matrix, QStringList labels, MS_PROD_MainWindow *mainWindow) = 0;
    protected:

    private: 
};

#endif  /* _PLOTMANAGER_H */