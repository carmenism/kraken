#include "AbsoluteSizeLegend.h"
#include "AbsoluteSizesChart.h"
#include "AbsoluteSizeIndicator.h"
#include "PrintText.h"
#include <QtOpenGL>
#include <string>

AbsoluteSizeLegend::AbsoluteSizeLegend(AbsoluteSizesChart *chart, std::vector<float> sampleValues)
: Point(0, 0) {
    this->chart = chart;
    points = new std::vector<AbsoluteSizeIndicator *>();

    for (unsigned int i = 0; i < sampleValues.size(); i++) {
        float value = sampleValues.at(i);
        AbsoluteSizeIndicator *a = new AbsoluteSizeIndicator(toStr(value), 0, value);
        points->push_back(a);
    }
}

AbsoluteSizeLegend::~AbsoluteSizeLegend() {
    while (!points->empty()) {
        AbsoluteSizeIndicator *p = points->back();
        points->pop_back();
        delete p;
    }
    
    delete points;
}

void AbsoluteSizeLegend::draw() {
    float padding = 5;
    float maxRadius = 0;
    float height = padding;

    for (unsigned int i = 0; i < points->size(); i++) {
        float radius = chart->getRadiusFromValue(points->at(i)->getValueY());
        float diameter = radius * 2;

        points->at(i)->setSize(diameter, diameter);

        if (maxRadius < radius) {
            maxRadius = radius;
        }

        height = height + padding + diameter;
    }

    glPushMatrix();
        glTranslatef(x, y, 0); 
    
        float posX = maxRadius + padding;
        float posXLabel = posX * 2;
        float posY = padding;
        float maxTextWidth = 0;

        for (unsigned int i = 0; i < points->size(); i++) {
            float radius = points->at(i)->getHeight() / 2;

            posY = posY + radius;
            points->at(i)->setLocation(posX, height - posY);
            points->at(i)->draw();

            float value = points->at(i)->getValueY();
            std::string label = numToStr(value);
            glColor4f(0, 0, 0, 1);
            PrintText::drawStrokeText(label, posXLabel, height - posY, 12, HORIZ_LEFT, VERT_CENTER, false, 0);
    
            float width = PrintText::strokeWidth(label, 12);

            if (maxTextWidth < width) {
                maxTextWidth = width;
            }

            posY = posY + radius + padding;
        }

        float width = 2 * maxRadius + 3 * padding + maxTextWidth;

        glColor4f(0, 0, 0, 1);
        PrintText::drawStrokeText("LEGEND", width  / 2, height, 12, HORIZ_CENTER, VERT_BOTTOM, false, 0);
    
        height = height + 12 + padding;

        glPolygonMode(GL_FRONT, GL_LINE);  
        glLineWidth(1.0);
        glColor4f(0.5, 0.5, 0.5, 1);

        glBegin(GL_LINE_LOOP);
            glVertex2f( 0, 0 );
            glVertex2f( 0, height );
            glVertex2f( width, height );
            glVertex2f( width, 0 );
        glEnd();
    glPopMatrix();
}