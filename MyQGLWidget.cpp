#include <QtGui/QMouseEvent>
#include "MyQGLWidget.h"
#include "stdio.h"
#include "Square.h"
#include "Circle.h"
#include "Triangle.h"
#include "LineGraph.h"
#include "GraphMarker.h"

MyQGLWidget::MyQGLWidget(QWidget *parent) : QGLWidget(parent) {


    setMouseTracking(true);

    /*square = new Square();
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
    triangle->setDrawFill(false);*/

    graph = new LineGraph();
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

    /*square->draw();
    circle->draw();
    triangle->draw();*/
    graph->draw();
}

void MyQGLWidget::selectItem(int x, int y) {
    printf("Selection attempted\n");

    float color[4];    
    unsigned char val[3];
    int i;

    glGetFloatv(GL_COLOR_CLEAR_VALUE, color);
    //glDrawBuffer(GL_BACK);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(color[0], color[1], color[2], color[3]);

    printf("Setting pick colors\n");
    GraphMarkerList *markers = graph->getMarkers();
    
    printf("%d markers\n", markers->size());
    for (i = 0; i < markers->size(); i++) {
        printf("   %d marker %f\n", i, (*markers)[i]->getValueY());
        
        printf("    r: %u, g: %u, b: % u ", (i & 0xFF), ((i >> 8) & 0xFF), 0);
        (*markers)[i]->setPickColor(i & 0xFF, (i >> 8) & 0xFF, 0);
    }

    printf("Drawing to pick colors\n");
    graph->drawToPick();

    
    printf("Retrieving pick color\n");
    glReadBuffer(GL_BACK);
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, val);
    printf("    r: %u, g: %u, b: % u ", val[0], val[1], val[2]);
    
    i = (val[1] << 8) + val[0];

    

    if (i >=0 && i < markers->size()) {
        printf("Marker %d ---> %f\n", i, (*markers)[i]->getValueY());
    }
}

void MyQGLWidget::mousePressEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        selectItem(event->x(), size().rheight() - event->y());
    }
}

void MyQGLWidget::mouseMoveEvent(QMouseEvent *event) {
    printf("%d, %d\n", event->x(), size().rheight() - event->y());
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