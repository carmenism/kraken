#include "Square.h"
#include <QtOpenGL>

Square::Square()
{
}

//------------- destructor -----------------------
Square::~Square()
{
}

void Square::drawAtOrigin() {
    glVertex2f(0, 0);
    glVertex2f(0, 1);
    glVertex2f(1, 1);
    glVertex2f(1, 0);
    glVertex2f(0, 0);
}