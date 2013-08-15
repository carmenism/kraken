#ifndef LINECHARTLEGEND_H_
#define LINECHARTLEGEND_H_

class LineChart;

#include "Legend.h"

class LineChartLegend : public Legend {
public:
    LineChartLegend(LineChart *c);
    ~LineChartLegend();

    virtual void drawAtOrigin();

    float getLineLength() { return lineLength; }
    void setLineLength(float len) { lineLength = len; }
private:
    float lineLength;
    LineChart *chart;
};

#endif /* LINECHARTLEGEND_H_ */