#ifndef _SMALLMULTIPLESMANAGER_H
#define _SMALLMULTIPLESMANAGER_H

#include "PlotManager.h"
#include "LineChart.h"
#include "ChartPoint.h"
#include <vector>

class GroupReordering;

class SmallMultiplesManager : public PlotManager {
    public:
        SmallMultiplesManager();
        ~SmallMultiplesManager();
        
        void setGroupReordering(GroupReordering *gr) { groupReordering = gr; }

        virtual std::vector<LineChart *> *getCharts() = 0;

        virtual void draw(float windowWidth, float windowHeight) = 0;
        virtual void drawToPick() = 0;
        virtual void updateCharts(Model *model, MS_PROD_MainWindow *mainWindow) = 0; 
    protected:
        float spacing, topSpace, leftSpace;
        GroupReordering *groupReordering;

        void setChartLocations(float windowWidth, float windowHeight);
};

#endif  /* _SMALLMULTIPLESMANAGER_H */