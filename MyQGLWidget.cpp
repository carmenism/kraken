#include <QtGui/QMouseEvent>
#include "MyQGLWidget.h"
#include "stdio.h"

#include "AbsoluteSizeIndicator.h"
#include "BetweenSpeciesArc.h"
#include "Button.h"
#include "ChangeSlider.h"
#include "ChartPoint.h"
#include "MS_PROD_MainWindow.h"
#include "MultiSpeciesLineChart.h"
#include "Parameters.h"
#include "Pickable.h"
#include "Picker.h"
#include "PlotManager.h"
#include "PlotByGroupManager.h"
#include "PlotBySpeciesManager.h"
#include "ResetButton.h"
#include "SliderButton.h"
#include "UndoButton.h"
#include "ToggleButton.h"

#include <QList>
#include <QStringList>

MyQGLWidget::MyQGLWidget(MS_PROD_MainWindow *mainWindow, QWidget *parent) : QGLWidget(parent) {
    plotManagers = new std::vector<PlotManager *>();
    sliders = new std::vector<ChangeSlider *>();
    sliderButtons = new std::vector<SliderButton *>();
    buttons = new std::vector<Button *>();

    this->mainWindow = mainWindow;

    setMouseTracking(true);
    setFixedWidth(1200);
    setFixedHeight(800);

    labelSuffix = " harvest effort";
    
    managerGroup = new PlotByGroupManager();
    managerGroup->displayOff();
    managerSpecies = new PlotBySpeciesManager();

    plotManagers->push_back(managerGroup);
    plotManagers->push_back(managerSpecies);

    picker = new Picker(this);
}

MyQGLWidget::~MyQGLWidget() {
    while (!plotManagers->empty()) {
        PlotManager *m = plotManagers->back();
        plotManagers->pop_back();
        delete m;
    }

    while (!sliders->empty()) {
        ChangeSlider *s = sliders->back();
        sliders->pop_back();
        delete s;
    }

    while (!buttons->empty()) {
        Button *b = buttons->back();
        buttons->pop_back();
        delete b;
    }
   
    delete buttons;
    delete sliderButtons;
    delete sliders;
    delete plotManagers;
    delete picker;
    //delete displayTypeButtons;
}

void MyQGLWidget::initializeGL() {
    glDepthMask(FALSE);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
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

    for (unsigned int i = 0; i < plotManagers->size(); i++) {
        if (plotManagers->at(i)->getDisplay()) {
            plotManagers->at(i)->draw();
        }
    }

    if (!managerGroup->empty()) {
        for (unsigned int i = 0; i < sliders->size(); i++) {
            sliders->at(i)->draw();
        }

        for (unsigned int i = 0; i < sliderButtons->size(); i++) {
            sliderButtons->at(i)->draw();
        }

        resetAllButton->draw();
        displayGroupButton->draw();
        displaySpeciesButton->draw();

        if (managerSpecies->getDisplay()) {
            toggleAbsButton->draw();
            toggleChartsButton->draw();
            toggleInterButton->draw();
            togglePredButton->draw();
        }
    }
}

void MyQGLWidget::drawToPick() {
    for (unsigned int i = 0; i < plotManagers->size(); i++) {
        if (plotManagers->at(i)->getDisplay()) {
            plotManagers->at(i)->drawToPick();
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
        if (!mouseReleaseButtons(x, y)) {
            for (unsigned int i = 0; i < sliders->size(); i++) {
                if (sliders->at(i)->mouseReleased(x, y)) {
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
        resetAllSliders();
        return true;
    }

    if (toggleInterButton->mouseReleased(x, y)) {
        toggleInteraction();
        return true;
    }
    if (togglePredButton->mouseReleased(x, y)) {
        togglePredation();
        return true;
    }

    SliderButton *button = NULL;

    for (unsigned int i = 0; i < sliderButtons->size(); i++) {
        if (sliderButtons->at(i)->mouseReleased(x, y)) {
            button = sliderButtons->at(i);
            break;
        }
    }
    
    if (button != NULL) {
        updateEffortToSlider(button->getSlider());

        runModel();
        clearDisplayOfSliders();
        captureLastValues();

        return true;
    }

    return false;
}

void MyQGLWidget::resetAllSliders() {
    for (unsigned int i = 0; i < sliders->size(); i++) {
        sliders->at(i)->reset();            
        updateEffortToSlider(sliders->at(i));
    }

    runModel();
    clearDisplayOfSliders();
    captureLastValues();
}

void MyQGLWidget::clearDisplayOfSliders() {
    for (unsigned int i = 0; i < sliders->size(); i++) {
        sliders->at(i)->clearDisplay();
    }
}

void MyQGLWidget::updateEffortToSlider(Slider *slider) {
    float value = slider->getValue();
    std::string title = slider->getTitle();
    std::string guild = title.substr(0, title.length() - labelSuffix.length());
    setEffort(value, guild);
}

void MyQGLWidget::setEffort(float value, std::string guildName) {
    mainWindow->getParameters()->setEffortForGuild(guildName, value);
}

void MyQGLWidget::runModel() {
    mainWindow->runModel();
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

    for (unsigned int i = 0; i < buttons->size(); i++) {
        buttonPress = buttons->at(i)->mousePressed(x, y);

        if (buttonPress) {
            return true;
        }
    }

    return false;
}

bool MyQGLWidget::mousePressSliders(float x, float y) {
    bool sliderPressed = false;
    ChangeSlider *pressed = NULL;
    
    for (unsigned int i = 0; i < sliders->size(); i++) {
        sliderPressed = sliders->at(i)->mousePressed(x, y);
        if (sliderPressed) {                
            captureLastValues();

            pressed = sliders->at(i);
            break;
        }
    }

    if (sliderPressed) {
        for (unsigned int i = 0; i < sliders->size(); i++) {
            if (sliders->at(i) != pressed) {
                sliders->at(i)->clearDisplay();
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

    for (unsigned int i = 0; i < buttons->size(); i++) {
        bool buttonMoved = buttons->at(i)->mouseMoved(x, y);

        if (buttonMoved) {
            moved = buttons->at(i);
        }
    }

    if (moved != NULL) {
        return true;
    }

    return false;
}

bool MyQGLWidget::mouseMoveSliders(float x, float y) {
    bool sliderMoved = false;

    for (unsigned int i = 0; i < sliders->size(); i++) {
        if (sliders->at(i)->mouseMoved(x, y)) {
            sliderMoved = true;

            float value = sliders->at(i)->getValue();
            std::string title = sliders->at(i)->getTitle();
            std::string guild = title.substr(0, title.length() - labelSuffix.length());
            setEffort(value, guild);
            runModel();

            break;
        }
    }

    return sliderMoved;
}

void MyQGLWidget::mouseMovePickables(int x, int y) {
    if (!managerGroup->empty()) {
        std::vector<Pickable *> *allPickables = new std::vector<Pickable *>();
        for (unsigned int i = 0; i < plotManagers->size(); i++) {
            if (plotManagers->at(i)->getDisplay()) {
                ChartPointList *p = plotManagers->at(i)->getPoints();
                allPickables->insert(allPickables->end(), p->begin(), p->end());
            }
        }

        BetweenSpeciesArcList *allArcs = managerSpecies->getArcs();
        if (allArcs != NULL) {
            allPickables->insert(allPickables->end(), allArcs->begin(), allArcs->end());
        }

        if (managerSpecies->getDisplay() 
            && managerSpecies->getDisplayAbsoluteSizes() 
            && !managerSpecies->getDisplayCharts()) {
            std::vector<AbsoluteSizeIndicator *> *absPoints = managerSpecies->getAbsPoints();
            allPickables->insert(allPickables->end(), absPoints->begin(), absPoints->end());
        }

        picker->pick(allPickables, x, y);   

        delete allPickables;
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
    for (unsigned int i = 0; i < plotManagers->size(); i++) {
        plotManagers->at(i)->updateCharts(matrix, labels, mainWindow);
    }

    updateGL();
}


void MyQGLWidget::captureLastValues() {
    for (unsigned int i = 0; i < plotManagers->size(); i++) {
        plotManagers->at(i)->captureLastValues();
    }
}

void MyQGLWidget::initializeSliders() {
    while (!sliders->empty()) {
        Slider *s = sliders->back();
        sliders->pop_back();
        delete s;
    }
    
    sliders->clear();

    QStringList guilds = mainWindow->getParameters()->getGuildList();

    for (int i = 0; i < guilds.size(); i++) {
        std::string guild = guilds.at(i).toStdString();
        ChangeSlider *slider = new ChangeSlider(guild + labelSuffix, 0, 10, 1);
        slider->setWidth(220);
        slider->setHeight(18);
        slider->displayLabelsOn();
        slider->setLabelInterval(1);
        sliders->push_back(slider);

        UndoButton *undoButton = new UndoButton(slider);
        undoButton->setHeight(18);
        undoButton->setWidth(42);
        sliderButtons->push_back(undoButton);
        buttons->push_back(undoButton);

        ResetButton *resetButton = new ResetButton(slider);
        resetButton->setHeight(18);
        resetButton->setWidth(42);
        sliderButtons->push_back(resetButton);
        buttons->push_back(resetButton);
    }

    resetAllButton = new Button("RESET ALL");
    resetAllButton->setHeight(20);
    resetAllButton->setWidth(100);
    resetAllButton->setLocation(300, 775);
    buttons->push_back(resetAllButton);

    displayGroupButton = new Button("Display by Group");
    displayGroupButton->setHeight(20);
    displayGroupButton->setWidth(130);
    displayGroupButton->setLocation(10, 775);
    buttons->push_back(displayGroupButton);
    //displayTypeButtons->addDisplayButton(displayGroupButton);

    displaySpeciesButton = new Button("Display by Species");
    displaySpeciesButton->setHeight(20);
    displaySpeciesButton->setWidth(130);
    displaySpeciesButton->setLocation(150, 775);
    buttons->push_back(displaySpeciesButton);
    //displayTypeButtons->addDisplayButton(displaySpeciesButton);

    toggleAbsButton = new Button("Toggle Abs. Sizes");
    toggleAbsButton->setHeight(20);
    toggleAbsButton->setWidth(130);
    toggleAbsButton->setLocation(5, 5);
    buttons->push_back(toggleAbsButton);

    toggleChartsButton = new Button("Toggle Charts");
    toggleChartsButton->setHeight(20);
    toggleChartsButton->setWidth(130);
    toggleChartsButton->setLocation(5, 30);
    buttons->push_back(toggleChartsButton);

    togglePredButton = new ToggleButton("Predation", true);
    togglePredButton->setHeight(20);
    togglePredButton->setWidth(130);
    togglePredButton->setLocation(5, 55);
    buttons->push_back(togglePredButton);

    toggleInterButton = new ToggleButton("Interaction", false);
    toggleInterButton->setHeight(20);
    toggleInterButton->setWidth(130);
    toggleInterButton->setLocation(140, 55);
    buttons->push_back(toggleInterButton);

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
    sliders->at(0)->setLocation(65, 150); // flatfish
    sliders->at(1)->setLocation(65, 350); // groundfish
    sliders->at(2)->setLocation(65, 550); // pelagics
    sliders->at(3)->setLocation(65, 705); // elasmobranchs

    for (unsigned int i = 0; i < sliders->size(); i++) {
        sliders->at(i)->titlePositionAbove();
    }

    positionSliderButtons();
}

void MyQGLWidget::positionSlidersForGroups() {    
    sliders->at(0)->setLocation(100, 22);  // flatfish
    sliders->at(1)->setLocation(700, 22);  // groundfish
    sliders->at(2)->setLocation(100, 410); // pelagics
    sliders->at(3)->setLocation(700, 410); // elasmobranchs

    for (unsigned int i = 0; i < sliders->size(); i++) {
        sliders->at(i)->titlePositionRight();
    }

    positionSliderButtons();
}

void MyQGLWidget::positionSliderButtons() {
    for (unsigned int i = 0; i < sliderButtons->size() / 2; i++) {
        sliderButtons->at(i * 2)->setLocation(sliders->at(i)->getX() - 55, sliders->at(i)->getY());
        sliderButtons->at(i * 2 + 1)->setLocation(sliders->at(i)->getX() - 55, sliders->at(i)->getY() - 20);
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
 
void MyQGLWidget::togglePredation() {
    if (togglePredButton->getValue()) {
        managerSpecies->displayPredation();
        toggleInterButton->setValue(false);
    } else {
        managerSpecies->displayNoArcs();
    }
}

void MyQGLWidget::toggleInteraction() {
    if (toggleInterButton->getValue()) {
        managerSpecies->displayInteraction();
        togglePredButton->setValue(false);
    } else {
        managerSpecies->displayNoArcs();
    }
}