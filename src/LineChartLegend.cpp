#include "LineChartLegend.h"
#include "LineChart.h"
#include "PrintText.h"

LineChartLegend::LineChartLegend(LineChart *c) {
    chart = c;
    lineLength = 15;
}

LineChartLegend::~LineChartLegend() {

}

void LineChartLegend::drawAtOrigin() {
    ChartPointSeriesList *seriesList = chart->getPointSeriesList();
    int num = seriesList->size();
    float maxWidth = -1;

    for (int i = num - 1; i >= 0; i--) {
        float xpos = 0;
        float ypos = (fontHeight + padding) * (num - i - 1) + padding;
        float width = seriesList->at(i)->drawInLegend(xpos, ypos, lineLength, padding, fontHeight);

        if (width > maxWidth) {
            maxWidth = width;
        }
    }

    PrintText::drawStrokeText("LEGEND", padding, (fontHeight + padding) * num + padding, fontHeight, HORIZ_LEFT, VERT_BOTTOM);

    width = maxWidth;
    height = (num + 1) * (fontHeight + padding) + padding;
}