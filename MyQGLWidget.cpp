#include <QtGui/QMouseEvent>
#include "MyQGLWidget.h"
#include "stdio.h"
#include "MultiSpeciesLineChart.h"
#include "ChartPoint.h"
#include "ChangeSlider.h"
#include "MS_PROD_MainWindow.h"
#include "Parameters.h"
#include "Button.h"
#include "UndoButton.h"
#include "ResetButton.h"
#include "SliderButton.h"
#include "VerticalArc.h"
#include <QList>
#include <QStringList>

MyQGLWidget::MyQGLWidget(MS_PROD_MainWindow *mainWindow, QWidget *parent) : QGLWidget(parent) {
    hovered = NULL;
    this->mainWindow = mainWindow;

    setMouseTracking(true);
    setFixedWidth(1200);
    setFixedHeight(800);

    labelSuffix = " harvest effort";

    arc = new VerticalArc(100, 100, 0);
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

    if (!charts.empty()) {
        for (int i = 0; i < sliders.size(); i++) {
            sliders[i]->draw();
        }

        for (int i = 0; i < buttons.size(); i++) {
            buttons[i]->draw();
        }

        resetAllButton->draw();
    }

    arc->draw();
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
    if (charts.empty()) {
        return;
    }

    float x = event->x();
    float y = size().rheight() - event->y();

    if (event->button() == Qt::LeftButton) {
        bool buttonReleased = mouseReleaseButtons(x, y);

        if (!buttonReleased) {
            for (int i = 0; i < sliders.size(); i++) {
                if (sliders[i]->mouseReleased()) {
                    break;
                }
            }
        }
    }

    updateGL();
}

bool MyQGLWidget::mouseReleaseButtons(float x, float y) {
    if (resetAllButton->mouseReleased(x, y)) {
        for (int i = 0; i < sliders.size(); i++) {
            sliders[i]->reset();
            float value = sliders[i]->getValue();
            std::string title = sliders[i]->getTitle();
            std::string guild = title.substr(0, title.length() - labelSuffix.length());
            mainWindow->getParameters()->setEffortForGuild(guild, value);  
        }

        mainWindow->runModel();

        for (int i = 0; i < sliders.size(); i++) {
            sliders[i]->clearDisplay();
        }
        for (int j = 0; j < charts.size(); j++) {
            charts[j]->captureLastValues();
        }

        return true;
    }

    SliderButton *button = NULL;

    for (int i = 0; i < buttons.size(); i++) {
        bool buttonPress = buttons[i]->mouseReleased(x, y);

        if (buttonPress) {
            button = buttons[i];
            break;
        }
    }
    
    if (button != NULL) {
        float value = button->getSlider()->getValue();
        std::string title = button->getSlider()->getTitle();
        std::string guild = title.substr(0, title.length() - labelSuffix.length());
        mainWindow->getParameters()->setEffortForGuild(guild, value);
        mainWindow->runModel();

        for (int i = 0; i < sliders.size(); i++) {
            sliders[i]->clearDisplay();
        }
        for (int j = 0; j < charts.size(); j++) {
            charts[j]->captureLastValues();
        }

        return true;
    }

    return false;
}

void MyQGLWidget::mousePressEvent(QMouseEvent *event) {
    if (charts.empty()) {
        return;
    }

    float x = event->x();
    float y = size().rheight() - event->y();
    
    if (event->button() == Qt::LeftButton) {
        bool buttonPressed = mousePressButtons(x, y);

        if (!buttonPressed) {
            bool sliderPressed = mousePressSliders(x, y);
        }
    }

    updateGL();
}

bool MyQGLWidget::mousePressButtons(float x, float y) {
    if (resetAllButton->mousePressed(x, y)) {
        return true;
    }
    
    bool buttonPress = false; 

    for (int i = 0; i < buttons.size(); i++) {
        buttonPress = buttons[i]->mousePressed(x, y);

        if (buttonPress) {
            return true;
        }
    }

    return false;
}

bool MyQGLWidget::mousePressSliders(float x, float y) {
    bool sliderPressed = false;
    ChangeSlider *pressed = NULL;
    
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

    if (sliderPressed) {
        for (int i = 0; i < sliders.size(); i++) {
            if (sliders[i] != pressed) {
                sliders[i]->clearDisplay();
            }
        }
    }

    return sliderPressed;
}

void MyQGLWidget::mouseMoveEvent(QMouseEvent *event) {
    if (charts.empty()) {
        return;
    }

    //printf("%d, %d\n", event->x(), size().rheight() - event->y());
    float x = event->x();
    float y = size().rheight() - event->y();

    bool buttonMoved = mouseMoveButtons(x, y);

    if (!buttonMoved) {
        bool sliderMoved = mouseMoveSliders(x, y);

        if (!sliderMoved) {
            mouseMoveChartPoints(x, y);
        }
    }

    updateGL();
}

bool MyQGLWidget::mouseMoveButtons(float x, float y) {
    if (resetAllButton->mouseMoved(x, y)) {
        return true;
    }

    Button *moved = NULL;

    for (int i = 0; i < buttons.size(); i++) {
        bool buttonMoved = buttons[i]->mouseMoved(x, y);

        if (buttonMoved) {
            moved = buttons[i];
        }
    }

    if (moved != NULL) {
        return true;
    }

    return false;
}

bool MyQGLWidget::mouseMoveSliders(float x, float y) {
    bool sliderMoved = false;

    for (int i = 0; i < sliders.size(); i++) {
        if (sliders[i]->mouseMoved(x, y)) {
            sliderMoved = true;

            float value = sliders[i]->getValue();
            std::string title = sliders[i]->getTitle();
            std::string guild = title.substr(0, title.length() - labelSuffix.length());
            mainWindow->getParameters()->setEffortForGuild(guild, value);
            mainWindow->runModel();

            break;
        }
    }

    return sliderMoved;
}

void MyQGLWidget::mouseMoveChartPoints(int x, int y) {
    if (!charts.empty()) {
        float color[4];    
        unsigned char val[3] = {'\0'};
        unsigned int pick;

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

        for (unsigned int i = 0; i < allPoints.size(); i++) {
            allPoints[i]->setPickColor(i & 0xFF, (i >> 8) & 0xFF, 0);
        }

        glDisable(GL_BLEND);
        for (int i = 0; i < charts.size(); i++) {
            charts[i]->drawToPick();
        }
        glEnable(GL_BLEND);

        glFlush();
        glReadBuffer(GL_BACK);
        glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, val);
        
        pick = (val[1] << 8) + val[0];

        int size = allPoints.size();
        if (pick >=0 && pick < size) {
            setHovered(allPoints[pick]);
        }
        else if (hovered != NULL)
            hovered->displayLabelOff();
    }
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

void MyQGLWidget::updateCharts(QList<QList<double>> matrix, QStringList labels) {
    if (charts.empty()) {
        initializeCharts(matrix, labels);
    } else {
        QStringList guilds = mainWindow->getParameters()->getGuildList();
    
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

void MyQGLWidget::initializeCharts(QList<QList<double>> matrix, QStringList labels) {
    QStringList guilds = mainWindow->getParameters()->getGuildList();
    
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

    charts[0]->setLocation(0, 37);
    charts[1]->setLocation(600, 37);
    charts[2]->setLocation(0, 440);
    charts[3]->setLocation(600, 440);
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
        slider->setWidth(220);
        slider->setHeight(18);
        slider->displayLabelsOn();
        slider->setLabelInterval(1);
        sliders.push_back(slider);

        UndoButton *undoButton = new UndoButton(slider);
        undoButton->setHeight(18);
        undoButton->setWidth(42);
        buttons.push_back(undoButton);

        ResetButton *resetButton = new ResetButton(slider);
        resetButton->setHeight(18);
        resetButton->setWidth(42);
        buttons.push_back(resetButton);
    }

    sliders[0]->setLocation(100, 22);
    buttons[0]->setLocation(45, 22);
    buttons[1]->setLocation(45, 2);

    sliders[1]->setLocation(700, 22);
    buttons[2]->setLocation(645, 22);
    buttons[3]->setLocation(645, 2);

    sliders[2]->setLocation(100, 425);
    buttons[4]->setLocation(45, 425);
    buttons[5]->setLocation(45, 405);

    sliders[3]->setLocation(700, 425);
    buttons[6]->setLocation(645, 425);
    buttons[7]->setLocation(645, 405);

    resetAllButton = new Button("RESET ALL");
    resetAllButton->setHeight(20);
    resetAllButton->setWidth(100);
    resetAllButton->setLocation(475, 750);
}