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
#include "PlotByGroupManager.h"
#include "PlotBySpeciesManager.h"
#include "PlotManager.h"
#include "Pickable.h"
#include "Picker.h"
#include "BetweenSpeciesArc.h"
#include <QList>
#include <QStringList>

MyQGLWidget::MyQGLWidget(MS_PROD_MainWindow *mainWindow, QWidget *parent) : QGLWidget(parent) {
    this->mainWindow = mainWindow;

    setMouseTracking(true);
    setFixedWidth(1200);
    setFixedHeight(800);

    labelSuffix = " harvest effort";
    
    managerGroup = new PlotByGroupManager();
    managerGroup->displayOff();
    managerSpecies = new PlotBySpeciesManager();

    plotManagers.push_back(managerGroup);
    plotManagers.push_back(managerSpecies);

    picker = new Picker(this);
}

void MyQGLWidget::initializeGL() {
    //glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glDepthMask(FALSE);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    //glEnable(GL_MULTISAMPLE_ARB);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
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

    for (unsigned int i = 0; i < plotManagers.size(); i++) {
        if (plotManagers[i]->getDisplay()) {
            plotManagers[i]->draw();
        }
    }

    if (!managerGroup->empty()) {
        for (unsigned int i = 0; i < sliders.size(); i++) {
            sliders[i]->draw();
        }

        for (unsigned int i = 0; i < sliderButtons.size(); i++) {
            sliderButtons[i]->draw();
        }

        resetAllButton->draw();
        displayGroupButton->draw();
        displaySpeciesButton->draw();

        if (managerSpecies->getDisplay()) {
            toggleAbsButton->draw();
            toggleChartsButton->draw();
        }
    }
}

void MyQGLWidget::drawToPick() {
    for (unsigned int i = 0; i < plotManagers.size(); i++) {
        if (plotManagers[i]->getDisplay()) {
            plotManagers[i]->drawToPick();
        }
    }
}

void MyQGLWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (managerGroup->empty()) {
        return;
    }

    float x = event->x();
    float y = size().rheight() - event->y();

    if (event->button() == Qt::LeftButton) {
        bool buttonReleased = mouseReleaseButtons(x, y);

        if (!buttonReleased) {
            for (unsigned int i = 0; i < sliders.size(); i++) {
                if (sliders[i]->mouseReleased()) {
                    break;
                }
            }
        }
    }

    updateGL();
}

bool MyQGLWidget::mouseReleaseButtons(float x, float y) {
    if (toggleChartsButton->mouseReleased(x, y)) {
        toggleCharts();

        return true;
    }
        
    if (toggleAbsButton->mouseReleased(x, y)) {
        toggleAbsoluteSizes();

        return true;
    }

    if (displayGroupButton->mouseReleased(x, y)) {
        displayByGroup();

        return true;
    }

    if (displaySpeciesButton->mouseReleased(x, y)) {
        displayBySpecies();

        return true;
    }

    if (resetAllButton->mouseReleased(x, y)) {
        for (unsigned int i = 0; i < sliders.size(); i++) {
            sliders[i]->reset();
            float value = sliders[i]->getValue();
            std::string title = sliders[i]->getTitle();
            std::string guild = title.substr(0, title.length() - labelSuffix.length());
            mainWindow->getParameters()->setEffortForGuild(guild, value);  
        }

        mainWindow->runModel();

        for (unsigned int i = 0; i < sliders.size(); i++) {
            sliders[i]->clearDisplay();
        }

        captureLastValues();

        return true;
    }

    SliderButton *button = NULL;

    for (unsigned int i = 0; i < sliderButtons.size(); i++) {
        bool buttonPress = sliderButtons[i]->mouseReleased(x, y);

        if (buttonPress) {
            button = sliderButtons[i];
            break;
        }
    }
    
    if (button != NULL) {
        float value = button->getSlider()->getValue();
        std::string title = button->getSlider()->getTitle();
        std::string guild = title.substr(0, title.length() - labelSuffix.length());
        mainWindow->getParameters()->setEffortForGuild(guild, value);
        mainWindow->runModel();

        for (unsigned int i = 0; i < sliders.size(); i++) {
            sliders[i]->clearDisplay();
        }

        captureLastValues();

        return true;
    }

    return false;
}

void MyQGLWidget::mousePressEvent(QMouseEvent *event) {
    if (managerGroup->empty()) {
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
    bool buttonPress = false; 

    for (unsigned int i = 0; i < buttons.size(); i++) {
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
    
    for (unsigned int i = 0; i < sliders.size(); i++) {
        sliderPressed = sliders[i]->mousePressed(x, y);
        if (sliderPressed) {                
            captureLastValues();

            pressed = sliders[i];
            break;
        }
    }

    if (sliderPressed) {
        for (unsigned int i = 0; i < sliders.size(); i++) {
            if (sliders[i] != pressed) {
                sliders[i]->clearDisplay();
            }
        }
    }

    return sliderPressed;
}

void MyQGLWidget::mouseMoveEvent(QMouseEvent *event) {
    if (managerGroup->empty()) {
        return;
    }

    //printf("%d, %d\n", event->x(), size().rheight() - event->y());
    float x = event->x();
    float y = size().rheight() - event->y();

    bool buttonMoved = mouseMoveButtons(x, y);

    if (!buttonMoved) {
        bool sliderMoved = mouseMoveSliders(x, y);

        if (!sliderMoved) {
            mouseMovePickables(x, y);
        }
    }

    updateGL();
}

bool MyQGLWidget::mouseMoveButtons(float x, float y) {
    Button *moved = NULL;

    for (unsigned int i = 0; i < buttons.size(); i++) {
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

    for (unsigned int i = 0; i < sliders.size(); i++) {
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

void MyQGLWidget::mouseMovePickables(int x, int y) {
    if (!managerGroup->empty()) {
        std::vector<Pickable *> allPickables;
        for (unsigned int i = 0; i < plotManagers.size(); i++) {
            if (plotManagers[i]->getDisplay()) {
                ChartPointList p = plotManagers[i]->getPoints();
                allPickables.insert(allPickables.end(), p.begin(), p.end());
            }
        }

        BetweenSpeciesArcList *allArcs = managerSpecies->getArcs();
        if (allArcs != NULL) {
            allPickables.insert(allPickables.end(), allArcs->begin(), allArcs->end());
        }

        picker->pick(allPickables, x, y);        
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
    for (unsigned int i = 0; i < plotManagers.size(); i++) {
        plotManagers[i]->updateCharts(matrix, labels, mainWindow);
    }

    updateGL();
}


void MyQGLWidget::captureLastValues() {
    for (unsigned int i = 0; i < plotManagers.size(); i++) {
        plotManagers[i]->captureLastValues();
    }
}

void MyQGLWidget::initializeSliders() {
    for (unsigned int i = 0; i < sliders.size(); i++) {
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
        sliderButtons.push_back(undoButton);
        buttons.push_back(undoButton);

        ResetButton *resetButton = new ResetButton(slider);
        resetButton->setHeight(18);
        resetButton->setWidth(42);
        sliderButtons.push_back(resetButton);
        buttons.push_back(resetButton);
    }

    resetAllButton = new Button("RESET ALL");
    resetAllButton->setHeight(20);
    resetAllButton->setWidth(100);
    resetAllButton->setLocation(300, 775);
    buttons.push_back(resetAllButton);

    displayGroupButton = new Button("Display by Group");
    displayGroupButton->setHeight(20);
    displayGroupButton->setWidth(130);
    displayGroupButton->setLocation(10, 775);
    buttons.push_back(displayGroupButton);

    displaySpeciesButton = new Button("Display by Species");
    displaySpeciesButton->setHeight(20);
    displaySpeciesButton->setWidth(130);
    displaySpeciesButton->setLocation(150, 775);
    buttons.push_back(displaySpeciesButton);

    toggleAbsButton = new Button("Toggle Abs. Sizes");
    toggleAbsButton->setHeight(20);
    toggleAbsButton->setWidth(130);
    toggleAbsButton->setLocation(5, 5);
    buttons.push_back(toggleAbsButton);

    toggleChartsButton = new Button("Toggle Charts");
    toggleChartsButton->setHeight(20);
    toggleChartsButton->setWidth(130);
    toggleChartsButton->setLocation(5, 30);
    buttons.push_back(toggleChartsButton);

    displayByGroup();
}

void MyQGLWidget::displayByGroup() {
    displayGroupButton->activeOff();
    displaySpeciesButton->activeOn();
    managerGroup->displayOn();
    managerSpecies->displayOff();
    positionSlidersForGroups();
    updateGL();
}

void MyQGLWidget::displayBySpecies() {
    displaySpeciesButton->activeOff();
    displayGroupButton->activeOn();
    managerSpecies->displayOn();
    managerGroup->displayOff();
    positionSlidersForSpecies();
    updateGL();
}

void MyQGLWidget::positionSlidersForSpecies() {
    sliders[0]->setLocation(65, 150); // flatfish
    sliders[1]->setLocation(65, 350); // groundfish
    sliders[2]->setLocation(65, 550); // pelagics
    sliders[3]->setLocation(65, 705); // elasmobranchs

    for (unsigned int i = 0; i < sliders.size(); i++) {
        sliders[i]->titlePositionAbove();
    }

    positionSliderButtons();
}

void MyQGLWidget::positionSlidersForGroups() {    
    sliders[0]->setLocation(100, 22);  // flatfish
    sliders[1]->setLocation(700, 22);  // groundfish
    sliders[2]->setLocation(100, 410); // pelagics
    sliders[3]->setLocation(700, 410); // elasmobranchs

    for (unsigned int i = 0; i < sliders.size(); i++) {
        sliders[i]->titlePositionRight();
    }

    positionSliderButtons();
}

void MyQGLWidget::positionSliderButtons() {
    for (unsigned int i = 0; i < sliderButtons.size() / 2; i++) {
        sliderButtons[i * 2]->setLocation(sliders[i]->getX() - 55, sliders[i]->getY());
        sliderButtons[i * 2 + 1]->setLocation(sliders[i]->getX() - 55, sliders[i]->getY() - 20);
    }
}

void MyQGLWidget::toggleAbsoluteSizes() {
    if (managerSpecies->getDisplay()) {
        if (managerSpecies->getDisplayAbsoluteSizes()) {
            managerSpecies->displayAbsoluteSizesOff();
        } else {
            managerSpecies->displayAbsoluteSizesOn();
        }

        updateGL();
    }
}

void MyQGLWidget::toggleCharts() {
    if (managerSpecies->getDisplay()) {
        if (managerSpecies->getDisplayCharts()) {
            managerSpecies->displayChartsOff();
        } else {
            managerSpecies->displayChartsOn();
        }

        updateGL();
    }
}