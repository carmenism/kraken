#include <QtGui/QMouseEvent>
#include "MyQGLWidget.h"
#include "stdio.h"
#include "MultiSpeciesLineChart.h"
#include "ChartPoint.h"
#include "ChangeSlider.h"
#include "MS_PROD_MainWindow.h"
#include "Parameters.h"
#include <QList>
#include <QStringList>

MyQGLWidget::MyQGLWidget(MS_PROD_MainWindow *mainWindow, QWidget *parent) : QGLWidget(parent) {
    hovered = NULL;
    this->mainWindow = mainWindow;

    setMouseTracking(true);
    setFixedWidth(1200);
    setFixedHeight(800);

    labelSuffix = " harvest effort";
}

void MyQGLWidget::initializeGL() {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
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

    for (int i = 0; i < charts.size(); i++) {
        charts[i]->draw();
    }

    for (int i = 0; i < sliders.size(); i++) {
        sliders[i]->draw();
    }
}

void MyQGLWidget::selectItem(int x, int y) {
    if (!charts.empty()) {
        float color[4];    
        unsigned char val[3];
        int i;

        glGetFloatv(GL_COLOR_CLEAR_VALUE, color);
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(color[0], color[1], color[2], color[3]);

        ChartPointList allPoints;
        
        for (int i = 0; i < charts.size(); i++) {
            ChartPointList *points = charts[i]->getPoints();

            for (int j = 0; j < points->size(); j++) {
                allPoints.push_back((*points)[j]);
            }
        }

        for (i = 0; i < allPoints.size(); i++) {
            allPoints[i]->setPickColor(i & 0xFF, (i >> 8) & 0xFF, 0);
        }

        for (int i = 0; i < charts.size(); i++) {
            charts[i]->drawToPick();
        }

        glReadBuffer(GL_BACK);
        glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, val);
        
        i = (val[1] << 8) + val[0];

        if (i >=0 && i < allPoints.size()) {
            setHovered(allPoints[i]);
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
    if (event->button() == Qt::LeftButton) {
        for (int i = 0; i < sliders.size(); i++) {
            if (sliders[i]->mouseReleased()) {
                break;
            }
        }
    }
}

void MyQGLWidget::mousePressEvent(QMouseEvent *event) {
    float x = event->x();
    float y = size().rheight() - event->y();
    bool sliderPressed = false;
    ChangeSlider *pressed = NULL;

    if (event->button() == Qt::LeftButton) {
        for (int i = 0; i < sliders.size(); i++) {
            sliderPressed = sliders[i]->mousePressed(x, y);
            if (sliderPressed) {                
                for (int j = 0; j < charts.size(); j++) {
                    charts[j]->captureLastValues();
                }

                pressed = sliders[i];
                break;
            }
        }
    } 

    if (sliderPressed) {
        for (int i = 0; i < sliders.size(); i++) {
            if (sliders[i] != pressed) {
                sliders[i]->clearDisplay();
            }
        }
    }

    updateGL();
}

void MyQGLWidget::mouseMoveEvent(QMouseEvent *event) {
    //printf("%d, %d\n", event->x(), size().rheight() - event->y());
    float x = event->x();
    float y = size().rheight() - event->y();
    
    bool sliderMoved = false;

    for (int i = 0; i < sliders.size(); i++) {
        if (sliders[i]->mouseMoved(x, y)) {
            sliderMoved = true;

            float value = sliders[i]->getValue();
            std::cout << "value " << value << "\n";
            std::string title = sliders[i]->getTitle();
            std::string guild = title.substr(0, title.length() - labelSuffix.length());
            mainWindow->getParameters()->setEffortForGuild(guild, value);
            mainWindow->runModel();

            break;
        }
    }

    if (!sliderMoved) {
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
    QStringList guilds = mainWindow->getParameters()->getGuildList();
    
    if (charts.empty()) {
        for (int i = 0; i < guilds.size(); i++) {
            QList<QList<double>> matrixForGuild;
            QStringList labelsForGuild;

            for (int j = 0; j < labels.size(); j++) {
                QString g = mainWindow->getParameters()->getGuildMembership(labels.at(j));
                
                if (QString::compare(g, guilds.at(i)) == 0) {
                    matrixForGuild.append(matrix.at(j));
                    labelsForGuild.append(labels.at(j));
                }
            }

            MultiSpeciesLineChart *chart = new MultiSpeciesLineChart(matrixForGuild, labelsForGuild);
            chart->setTitle(guilds.at(i).toStdString());
            chart->setWidth(400);
            chart->setHeight(300);
            chart->setAxesFontHeight(10);
            chart->setLegendFontHeight(12);
            charts.push_back(chart);
        }

        charts[0]->setLocation(0, 35);
        charts[1]->setLocation(600, 35);
        charts[2]->setLocation(0, 440);
        charts[3]->setLocation(600, 440);
    } else {
        for (int i = 0; i < guilds.size(); i++) {
            QList<QList<double>> matrixForGuild;

            for (int j = 0; j < labels.size(); j++) {
                QString g = mainWindow->getParameters()->getGuildMembership(labels.at(j));
                
                if (QString::compare(g, guilds.at(i)) == 0) {
                    matrixForGuild.append(matrix.at(j));
                }
            }

            charts.at(i)->setValues(matrixForGuild);
        }
    }

    updateGL();
}

void MyQGLWidget::initializeSliders() {
    for (int i = 0; i < sliders.size(); i++) {
        delete sliders[i];
    }
    
    sliders.clear();

    QStringList guilds = mainWindow->getParameters()->getGuildList();

    for (int i = 0; i < guilds.size(); i++) {
        std::string guild = guilds.at(i).toStdString();
        ChangeSlider *slider = new ChangeSlider(guild + labelSuffix, 0, 10, 1);
        slider->setWidth(200);
        slider->displayLabelsOn();
        slider->setLabelInterval(1);
        sliders.push_back(slider);
    }

    sliders[0]->setLocation(50, 20);
    sliders[1]->setLocation(650, 20);
    sliders[2]->setLocation(50, 425);
    sliders[3]->setLocation(650, 425);
}