#include "LineChartLegend.h"
#include "LineChart.h"
#include "PrintText.h"

LineChartLegend::LineChartLegend(LineChart *c) {
    chart = c;
}

void LineChartLegend::draw(float x, float y, float lineLength, float spacing, void *font) {
    glPushMatrix();
        glTranslatef(x, y, 0);

        ChartPointSeriesList seriesList = chart->getPointSeriesList();
        float h = PrintText::printingHeight(font);
        int num = seriesList.size();
        float maxWidth = -1;

        for (int i = num - 1; i >= 0; i--) {
            float width = seriesList[i]->drawInLegend(0, h * (num - i - 1), lineLength, spacing, font);

            if (width > maxWidth) {
                maxWidth = width;
            }
        }

        PrintText::printVerticallyCenteredAt(spacing, h * (num + 0.5), "LEGEND", false, false, font);

        drawBoundary(maxWidth, (num + 1) * h + spacing);
    glPopMatrix();
}

void LineChartLegend::drawBoundary(float width, float height) {  
    glPolygonMode(GL_FRONT, GL_LINE);  
    glLineWidth(1.0);
    glColor4f(0.5, 0.5, 0.5, 1);

    glBegin(GL_LINE_LOOP);
        glVertex2f( 0, 0 );
        glVertex2f( 0, height );
        glVertex2f( width, height );
        glVertex2f( width, 0 );
    glEnd();
}