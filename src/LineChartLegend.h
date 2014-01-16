#ifndef LINECHARTLEGEND_H_
#define LINECHARTLEGEND_H_

class LineChart;

#include "Legend.h"

class LineChartLegend : public Legend {
public:
    LineChartLegend(LineChart *c);
    ~LineChartLegend();

    float getLineLength() { return lineLength; }
    void setLineLength(float len) { lineLength = len; }
private:
    float lineLength;
    LineChart *chart;
    
    virtual void drawAtOrigin();
};

#endif /* LINECHARTLEGEND_H_ */