#include <QtGui/QMouseEvent>
#include "MyQGLWidget.h"
#include "stdio.h"
#include "Square.h"
#include "Circle.h"
#include "Triangle.h"

MyQGLWidget::MyQGLWidget(QWidget *parent) : QGLWidget(parent) {
    setMouseTracking(true);

    square = new Square();
    square->setSize(100, 100);
    square->setLocation(150, 100);
    square->setBorderColor(0.0, 0.0, 1.0);
    square->setDrawFill(false);

    circle = new Circle();
    circle->setSize(100, 100);
    circle->setLocation(150, 100);
    circle->setBorderColor(0.0, 1.0, 0.0);
    circle->setDrawFill(false);

    triangle = new Triangle();
    triangle->setLocation(150, 100);
    triangle->setSize(100, 100);
    triangle->setBorderColor(1.0, 0.0, 0.0);
    triangle->setDrawFill(false);
}

void MyQGLWidget::initializeGL() {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(1.0, 1.0, 1.0, 0);
}

void MyQGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1.0l, 1.0l); // set origin to bottom left corner
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MyQGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    /*glColor3f(1,0,0);
    glBegin(GL_POLYGON);
    glVertex2f(0,0);
    glVertex2f(100,500);
    glVertex2f(500,100);
    glEnd();*/

    square->redraw();
    circle->redraw();
    triangle->redraw();
}

void MyQGLWidget::mousePressEvent(QMouseEvent *event) {

}
void MyQGLWidget::mouseMoveEvent(QMouseEvent *event) {
    printf("%d, %d\n", event->x(), event->y());
}

void MyQGLWidget::keyPressEvent(QKeyEvent* event) {
    switch(event->key()) {
    case Qt::Key_Escape:
        close();
        break;
    default:
        event->ignore();
        break;
    }
}