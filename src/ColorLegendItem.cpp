#include "ColorLegendItem.h"
#include "PrintText.h"
#include <GL/glut.h>

ColorLegendItem::ColorLegendItem(Color *color, std::string label) {
    boxColor = color;
    this->label = label;
    boxWidth = 25;
    boxHeight = 15;
}

ColorLegendItem::~ColorLegendItem() {
    delete boxColor;
}

float ColorLegendItem::draw(float fontHeight, float padding) {
    glPolygonMode(GL_FRONT, GL_FILL); 
    glColor4f(boxColor->r, boxColor->g, boxColor->b, boxColor->a);

    glBegin(GL_QUADS);
        glVertex2f( 0, 0 );
        glVertex2f( 0, boxHeight );
        glVertex2f( boxWidth, boxHeight );
        glVertex2f( boxWidth, 0 );
    glEnd();

    glPolygonMode(GL_FRONT, GL_LINE);  
    glLineWidth(1.0);
    glColor4f(0.5, 0.5, 0.5, 1);

    glBegin(GL_LINE_LOOP);
        glVertex2f( 0, 0 );
        glVertex2f( 0, boxHeight );
        glVertex2f( boxWidth, boxHeight );
        glVertex2f( boxWidth, 0 );
    glEnd();

    glColor4f(0, 0, 0, 1);
    PrintText::drawStrokeText(label, boxWidth + padding, boxHeight / 2, fontHeight, HORIZ_LEFT, VERT_CENTER);

    return boxWidth + padding + PrintText::strokeWidth(label, fontHeight);
}