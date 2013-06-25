#include "GraphLegend.h"
#include "LineGraph.h"
#include "PrintText.h"

GraphLegend::GraphLegend(LineGraph *g) {
    graph = g;
}

void GraphLegend::draw(float x, float y, float lineLength, float spacing, void *font) {
    glPushMatrix();
        glTranslatef(x, y, 0);

        //void *font = GLUT_BITMAP_HELVETICA_10;
        //float lineLength = 15.0;
        //float spacing = 5.0;

        LineGroupList lines = graph->getLines();
        float h = PrintText::printingHeight(font);
        int num = lines.size();
        float maxWidth = -1;

        for (int i = 0; i < num; i++) {
            float width = lines[i]->drawInLegend(0, h * i, lineLength, spacing, font);

            if (width > maxWidth) {
                maxWidth = width;
            }
        }

        PrintText::printVerticallyCenteredAt(spacing, h * (num + 0.5), "LEGEND", false, false, font);

        drawBoundary(maxWidth, (num + 1) * h + spacing);
    glPopMatrix();
}

void GraphLegend::drawBoundary(float width, float height) {  
    glPolygonMode(GL_FRONT, GL_LINE);  
    glLineWidth(1.0);
    glColor4f(0, 0, 0, 1);

    glBegin(GL_LINE_LOOP);
        glVertex2f( 0, 0 );
        glVertex2f( 0, height );
        glVertex2f( width, height );
        glVertex2f( width, 0 );
    glEnd();
}