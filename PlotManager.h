#ifndef _PLOTMANAGER_H
#define _PLOTMANAGER_H

#include "Displayable.h"
#include "LineChart.h"
#include "ChartPoint.h"
#include <vector>
#include <QList>

class Pickable;
class Model;
class MS_PROD_MainWindow;
class QStringList;

class PlotManager : public Displayable {
    public:
        PlotManager() {  }
        ~PlotManager();
     
        virtual void draw(float windowWidth, float windowHeight);
        virtual void drawToPick();
        void captureLastValues();
        std::vector<Pickable *> *getPickables();
        
        bool empty() { return getCharts()->empty(); }

        virtual std::vector<LineChart *> *getCharts() = 0;
        virtual void updateCharts(Model *model, MS_PROD_MainWindow *mainWindow) = 0;
    protected:
        float getFontSize(float windowWidth, float windowHeight);
        float getMarkersSize(float chartWidth, float chartHeight);
    private: 
};

#endif  /* _PLOTMANAGER_H */