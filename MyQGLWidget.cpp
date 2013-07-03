#include <QtGui/QMouseEvent>
#include "MyQGLWidget.h"
#include "stdio.h"
#include "Square.h"
#include "Circle.h"
#include "Triangle.h"
#include "LineChart.h"
#include "ChartPoint.h"
#include "Slider.h"
#include "MS_PROD_MainWindow.h"
#include "Parameters.h"
#include <QList>
#include <QStringList>

MyQGLWidget::MyQGLWidget(MS_PROD_MainWindow *mainWindow, QWidget *parent) : QGLWidget(parent) {
    hovered = NULL;
    chart = NULL;
    this->mainWindow = mainWindow;

    setMouseTracking(true);

    slider = new Slider(10, 550, 200, 0.25);
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

    if (chart != NULL) {
        chart->draw();
    }

    slider->draw();
}

void MyQGLWidget::selectItem(int x, int y) {
    if (chart != NULL) {
        float color[4];    
        unsigned char val[3];
        int i;

        glGetFloatv(GL_COLOR_CLEAR_VALUE, color);
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(color[0], color[1], color[2], color[3]);

        ChartPointList *points = chart->getPoints();
        
        for (i = 0; i < points->size(); i++) {
            (*points)[i]->setPickColor(i & 0xFF, (i >> 8) & 0xFF, 0);
        }

        chart->drawToPick();

        glReadBuffer(GL_BACK);
        glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, val);
        
        i = (val[1] << 8) + val[0];

        if (i >=0 && i < points->size()) {
            setHovered((*points)[i]);
        } else if (hovered != NULL) {
            hovered->displayLabelOff();
        }
    }
}

void MyQGLWidget::setHovered(ChartPoint *point) {
    point->displayLabelOn();
    
    if (hovered != point) {
        if (hovered != NULL) {
            hovered->displayLabelOff();
        }
        
        hovered = point;
    }
}

void MyQGLWidget::mouseReleaseEvent(QMouseEvent *event) {
    slider->mouseReleased();
}

void MyQGLWidget::mousePressEvent(QMouseEvent *event) {

    if (event->button() == Qt::LeftButton) {
        slider->mousePressed(event->x(), size().rheight() - event->y());
    } 

    updateGL();
}

void MyQGLWidget::mouseMoveEvent(QMouseEvent *event) {
    //printf("%d, %d\n", event->x(), size().rheight() - event->y());
    float x = event->x();
    float y = size().rheight() - event->y();
    
    bool sliderMoved = slider->mouseMoved(x, y);

    if (sliderMoved) {
        float value = slider->getValue() * 100;
        mainWindow->getParameters()->setEffortForGuild(QString("Flatfish"), value);
        mainWindow->runModel();
    } else {
        selectItem(x, y);
    }

    updateGL();
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

void MyQGLWidget::updateLineChart(QList<QList<double>> matrix, QStringList labels) {
    if (chart == NULL) {
        chart = new LineChart(matrix, labels);
    } else {
        chart->setValues(matrix);
    }

    updateGL();
}