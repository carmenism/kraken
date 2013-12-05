#include <QtGui/QMouseEvent>
#include "MyQGLWidget.h"
#include "stdio.h"

#include "AbsoluteSizeIndicator.h"
#include "BetweenSpeciesArc.h"
#include "Button.h"
#include "ChangeSlider.h"
#include "ChartPoint.h"
#include "Color.h"
#include "GroupReordering.h"
#include "Model.h"
#include "MS_PROD_MainWindow.h"
#include "MultiSpeciesLineChart.h"
#include "Parameters.h"
#include "Pickable.h"
#include "Picker.h"
#include "PlotManager.h"
#include "PlotByGroupManager.h"
#include "PlotBySpeciesWithArcsManager.h"
#include "PrintText.h"
#include "ResetButton.h"
#include "SingleSpeciesLineChart.h"
#include "SliderButton.h"
#include "UndoButton.h"
#include "ToggleButton.h"
#include "MonteCarloPlotManager.h"
#include "KrakenMonteCarlo.h"

#include <QList>
#include <QStringList>

MyQGLWidget::MyQGLWidget(MS_PROD_MainWindow *mainWindow, QWidget *parent) : QGLWidget(parent) {
    plotManagers = new std::vector<PlotManager *>();
    sliders = new std::vector<ChangeSlider *>();
    sliderButtons = new std::vector<SliderButton *>();
    speciesButtons = new std::vector<Button *>();
    displayButtons = new std::vector<Button *>();
    speciesGroupButtons = new std::vector<Button *>();
    changeTypeButtons = new std::vector<Button *>();
    monteCarloTypeButtons = new std::vector<Button *>();
    monteCarloOtherButtons = new std::vector<Button *>();

    paddingRight = 0;
    paddingLeft = 0;
    paddingBottom = 0;
    paddingTop = 40;

    this->mainWindow = mainWindow;

    setMouseTracking(true);
    setMinimumWidth(800);
    setMinimumHeight(600);

    labelSuffix = " harvest effort";
    
    managerGroup = new PlotByGroupManager();
    managerGroup->displayOff();
    managerSpecies = new PlotBySpeciesWithArcsManager();

    managerMC = new MonteCarloPlotManager();
    managerMC->displayOff();

    kmc = new KrakenMonteCarlo(mainWindow, managerMC);

    plotManagers->push_back(managerGroup);
    plotManagers->push_back(managerSpecies);
    plotManagers->push_back(managerMC);

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

    deleteButtonList(displayButtons);
    deleteButtonList(speciesButtons);
    deleteButtonList(speciesGroupButtons);
    deleteButtonList(changeTypeButtons);
    deleteButtonList(monteCarloTypeButtons);
    deleteButtonList(monteCarloOtherButtons);

    delete sliders;
    delete plotManagers;
    delete picker;
    delete kmc;
    delete gr;
}

void MyQGLWidget::deleteButtonList(std::vector<Button *> *list) {
    while (!list->empty()) {
        Button *b = list->back();
        list->pop_back();
        delete b;
    }

    delete list;
}

void MyQGLWidget::initializeGL() {
    glDepthMask(FALSE);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint( GL_LINE_SMOOTH_HINT, GL_DONT_CARE );
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

    glPushMatrix();
        glTranslatef(paddingLeft, paddingBottom, 0);
        for (unsigned int i = 0; i < plotManagers->size(); i++) {
            if (plotManagers->at(i)->getDisplay()) {
                plotManagers->at(i)->draw(size().rwidth() - paddingRight - paddingLeft, size().rheight() - paddingTop - paddingBottom);
            }
        }
    glPopMatrix();

    if (!managerGroup->empty()) {
        float xPos = size().rwidth() - 100;
        float yPos = size().rheight() - 25;
        float yOff = 30;
        
        baselineButton->setLocation(280, yPos);
        resetAllButton->setLocation(370, yPos);

        if (managerSpecies->getDisplay() || managerGroup->getDisplay()) {
            if (managerSpecies->getDisplay()) {
                positionSlidersForSpecies();

                for (int i = 0; i < speciesButtons->size(); i++) {
                    speciesButtons->at(i)->draw();
                }
            } else {
                positionSlidersForGroups();
            }

            for (int i = 0; i < speciesGroupButtons->size(); i++) {
                speciesGroupButtons->at(i)->draw();
            }

            for (unsigned int i = 0; i < sliders->size(); i++) {
                sliders->at(i)->draw();
            }

            for (unsigned int i = 0; i < sliderButtons->size(); i++) {
                sliderButtons->at(i)->draw();
            }

            drawBoxedGroup("Change", 5, size().rheight() - 2 * yOff - 1, 5, changeTypeButtons);
        } else {
            runMCButton->setLocation(280, yPos);
            runMCButton->draw();

            drawBoxedGroup("Type", 5, size().rheight() - 2 * yOff - 1, 5, monteCarloTypeButtons);
        }

        drawBoxedGroup("Display", 5, size().rheight() - yOff, 5, displayButtons);
    }
}

void MyQGLWidget::drawBoxedGroup(std::string label, float x, float y, float spacing, std::vector<Button *> *buttons) {
    float textWidth = int(PrintText::strokeWidth(label, 10)) + 1;
    
    float xPos = 4 * spacing + textWidth;
    float height = 0;

    for (int i = 0; i < buttons->size(); i++) {
        buttons->at(i)->setLocation(xPos, y + spacing);

        xPos = xPos + spacing + buttons->at(i)->getWidth();
        height = max(height, buttons->at(i)->getHeight());
    }

    for (int i = 0; i < buttons->size(); i++) {
        buttons->at(i)->draw();
    }

    glColor4f(0, 0, 0, 1);
    PrintText::drawStrokeText(label, x + spacing, y + spacing + height / 2, 10, HORIZ_LEFT, VERT_CENTER);
    
    drawBox(x, y, xPos - spacing, height + spacing * 2);
}

void MyQGLWidget::drawBox(float x, float y, float w, float h) {
    glDisable(GL_LINE_SMOOTH);
    glPolygonMode(GL_FRONT, GL_LINE);  
        glLineWidth(0.5);
        glColor4f(0.5, 0.5, 0.5, 1);

        glBegin(GL_LINE_LOOP);
        glVertex2f( x, y );
        glVertex2f( x, y + h );
        glVertex2f( x + w, y + h );
        glVertex2f( x + w, y );
    glEnd();
    glEnable(GL_LINE_SMOOTH);
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
    if (runMCButton->mouseReleased(x, y)) {
        kmc->run();
        
        streaksButton->setActive(!managerMC->getDisplayStreaks());
        boxPlotsButton->setActive(!managerMC->getDisplayBoxPlots());
        errorBarsButton->setActive(!managerMC->getDisplayErrorBars());
        errorBandsButton->setActive(!managerMC->getDisplayErrorBands());        

        return true;
    }

    if (baselineButton->mouseReleased(x, y)) {
        setBaseline();
        return true;
    }

    if (toggleChartsButton->mouseReleased(x, y)) {
        toggleCharts();
        return true;
    }
        
    if (toggleAbsButton->mouseReleased(x, y)) {
        toggleAbsoluteSizes();
        return true;
    }

    if (toggleHarvButton->mouseReleased(x, y)) {
        toggleHarvest();
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

    if (displayMCButton->mouseReleased(x, y)) {
        displayMonteCarlo();
        return true;
    }

    if (changeLineButton->mouseReleased(x, y)) {
        displayGhostAsLine();
        return true;
    }
    if (changeBlendButton->mouseReleased(x, y)) {
        displayGhostAsBlend();
        return true;
    }
    if (changeOffButton->mouseReleased(x, y)) {
        displayGhostOff();
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

    if (toggleArcsDynamicButton->mouseReleased(x, y)) {
        toggleDynamicArcs();
        return true;
    }

    if (streaksButton->mouseReleased(x, y)) {
        monteCarloStreaks();
        return true;
    }
    if (boxPlotsButton->mouseReleased(x, y)) {
        monteCarloBoxPlots();
        return true;
    }
    if (errorBandsButton->mouseReleased(x, y)) {
        monteCarloErrorBands();
        return true;
    }
    if (errorBarsButton->mouseReleased(x, y)) {
        monteCarloErrorBars();
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
}

void MyQGLWidget::setSlidersBaseline() {
    for (unsigned int i = 0; i < sliders->size(); i++) {
        sliders->at(i)->setBaseline();//->clearDisplay();
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

    for (unsigned int i = 0; i < displayButtons->size(); i++) {
        buttonPress = displayButtons->at(i)->mousePressed(x, y);

        if (buttonPress) {
            return true;
        }
    }

    for (unsigned int i = 0; i < changeTypeButtons->size(); i++) {
        buttonPress = changeTypeButtons->at(i)->mousePressed(x, y);

        if (buttonPress) {
            return true;
        }
    }

    if (managerSpecies->getDisplay()) {
        for (unsigned int i = 0; i < speciesButtons->size(); i++) {
            buttonPress = speciesButtons->at(i)->mousePressed(x, y);

            if (buttonPress) {
                return true;
            }
        }
    }

    if (managerSpecies->getDisplay() || managerGroup->getDisplay()) {
        for (unsigned int i = 0; i < speciesGroupButtons->size(); i++) {
            buttonPress = speciesGroupButtons->at(i)->mousePressed(x, y);

            if (buttonPress) {
                return true;
            }
        }
    }

    if (managerMC->getDisplay()) {
        for (unsigned int i = 0; i < monteCarloTypeButtons->size(); i++) {
            buttonPress = monteCarloTypeButtons->at(i)->mousePressed(x, y);

            if (buttonPress) {
                return true;
            }
        }
    }

    if (managerMC->getDisplay()) {
        for (unsigned int i = 0; i < monteCarloOtherButtons->size(); i++) {
            buttonPress = monteCarloOtherButtons->at(i)->mousePressed(x, y);

            if (buttonPress) {
                return true;
            }
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
            pressed = sliders->at(i);
            break;
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

    moved = mouseMoveButtonHelper(displayButtons, x, y);

    if (moved != NULL) {
        return true;
    }

    if (managerSpecies->getDisplay() || managerGroup->getDisplay()) {
        moved = mouseMoveButtonHelper(speciesGroupButtons, x, y);
        moved = mouseMoveButtonHelper(changeTypeButtons, x, y);
    }

    if (moved != NULL) {
        return true;
    }

    if (managerSpecies->getDisplay()) {        
        moved = mouseMoveButtonHelper(speciesButtons, x, y);
    }

    if (moved != NULL) {
        return true;
    }

    if (managerMC->getDisplay()) {
        moved = mouseMoveButtonHelper(monteCarloOtherButtons, x, y);

        if (moved != NULL) {
            return true;
        }

        moved = mouseMoveButtonHelper(monteCarloTypeButtons, x, y);
    }

    if (moved != NULL) {
        return true;
    }

    return false;
}

Button *MyQGLWidget::mouseMoveButtonHelper(std::vector<Button *> *list, float x, float y) {
    Button *moved = NULL;

    for (unsigned int i = 0; i < list->size(); i++) {
        bool buttonMoved = list->at(i)->mouseMoved(x, y);

        if (buttonMoved) {
            moved = list->at(i);
        }
    }

    return moved;
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
                std::vector<Pickable *> *p = plotManagers->at(i)->getPickables();
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

void MyQGLWidget::updateCharts(Model *model) {
    for (unsigned int i = 0; i < plotManagers->size(); i++) {
        plotManagers->at(i)->updateCharts(model, mainWindow);
    }

    updateGL();    

    updateGL();
}


void MyQGLWidget::captureLastValues() {
    for (unsigned int i = 0; i < plotManagers->size(); i++) {
        plotManagers->at(i)->captureLastValues();
    }
}

void MyQGLWidget::initialize() {
    gr = new GroupReordering(mainWindow);
    managerSpecies->setGroupReordering(gr);
    managerMC->setGroupReordering(gr);

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
        speciesGroupButtons->push_back(undoButton);

        ResetButton *resetButton = new ResetButton(slider);
        resetButton->setHeight(18);
        resetButton->setWidth(42);
        sliderButtons->push_back(resetButton);
        speciesGroupButtons->push_back(resetButton);
    }

    float buttonHeight = 18;
    float buttonWidth = 55;

    baselineButton = new Button("Set Baseline");
    baselineButton->setWidth(buttonWidth);
    baselineButton->setHeight(buttonHeight);
    speciesGroupButtons->push_back(baselineButton);

    resetAllButton = new Button("Reset All");
    resetAllButton->setHeight(buttonHeight);
    resetAllButton->setWidth(buttonWidth);
    speciesGroupButtons->push_back(resetAllButton);

    displayGroupButton = new Button("Group");
    displayGroupButton->setHeight(buttonHeight);
    displayGroupButton->setWidth(buttonWidth);
    displayButtons->push_back(displayGroupButton);

    displaySpeciesButton = new Button("Species");
    displaySpeciesButton->setHeight(buttonHeight);
    displaySpeciesButton->setWidth(buttonWidth);
    displayButtons->push_back(displaySpeciesButton);

    displayMCButton = new Button("Monte Carlo");
    displayMCButton->setHeight(buttonHeight);
    displayMCButton->setWidth(buttonWidth);
    displayButtons->push_back(displayMCButton);

    changeLineButton = new Button("Line");
    changeLineButton->setHeight(buttonHeight);
    changeLineButton->setWidth(buttonWidth);
    changeTypeButtons->push_back(changeLineButton);

    changeBlendButton = new Button("Blend");
    changeBlendButton->setHeight(buttonHeight);
    changeBlendButton->setWidth(buttonWidth);
    changeTypeButtons->push_back(changeBlendButton);
    changeBlendButton->activeOff();

    changeOffButton = new Button("Off");
    changeOffButton->setHeight(buttonHeight);
    changeOffButton->setWidth(buttonWidth);
    changeTypeButtons->push_back(changeOffButton);

    toggleAbsButton = new ToggleButton("Abs. Sizes", false);
    toggleAbsButton->setHeight(buttonHeight);
    toggleAbsButton->setWidth(buttonWidth);
    toggleAbsButton->setLocation(5, 5);
    speciesButtons->push_back(toggleAbsButton);

    toggleChartsButton = new ToggleButton("Biomass", true);
    toggleChartsButton->setHeight(buttonHeight);
    toggleChartsButton->setWidth(buttonWidth);
    toggleChartsButton->setLocation(5, 30);
    speciesButtons->push_back(toggleChartsButton);

    toggleHarvButton = new ToggleButton("Harvest", false);
    toggleHarvButton->setHeight(buttonHeight);
    toggleHarvButton->setWidth(buttonWidth);
    toggleHarvButton->setLocation(140, 30);
    speciesButtons->push_back(toggleHarvButton);

    togglePredButton = new ToggleButton("Predation", true);
    togglePredButton->setHeight(buttonHeight);
    togglePredButton->setWidth(buttonWidth);
    togglePredButton->setLocation(5, 55);
    speciesButtons->push_back(togglePredButton);

    toggleInterButton = new ToggleButton("Interaction", false);
    toggleInterButton->setHeight(buttonHeight);
    toggleInterButton->setWidth(buttonWidth);
    toggleInterButton->setLocation(140, 55);
    speciesButtons->push_back(toggleInterButton);

    toggleArcsDynamicButton = new ToggleButton("Dynamic Arcs", false);
    toggleArcsDynamicButton->setHeight(buttonHeight);
    toggleArcsDynamicButton->setWidth(buttonWidth);
    toggleArcsDynamicButton->setLocation(5, 80);
    speciesButtons->push_back(toggleArcsDynamicButton);

    runMCButton = new Button("Run Simulation");
    runMCButton->setHeight(buttonHeight);
    runMCButton->setWidth(buttonWidth);
    monteCarloOtherButtons->push_back(runMCButton);

    streaksButton = new Button("Streaks");
    streaksButton->setHeight(buttonHeight);
    streaksButton->setWidth(buttonWidth);
    monteCarloTypeButtons->push_back(streaksButton);
    streaksButton->activeOff();

    boxPlotsButton = new Button("Box Plots");
    boxPlotsButton->setHeight(buttonHeight);
    boxPlotsButton->setWidth(buttonWidth + 5);
    monteCarloTypeButtons->push_back(boxPlotsButton);
    boxPlotsButton->activeOff();

    errorBandsButton = new Button("Error Bands");
    errorBandsButton->setHeight(buttonHeight);
    errorBandsButton->setWidth(buttonWidth + 20);
    monteCarloTypeButtons->push_back(errorBandsButton);
    errorBandsButton->activeOff();

    errorBarsButton = new Button("Error Bars");
    errorBarsButton->setHeight(buttonHeight);
    errorBarsButton->setWidth(buttonWidth + 20);
    monteCarloTypeButtons->push_back(errorBarsButton);
    errorBarsButton->activeOff();

    displayByGroup();
}

void MyQGLWidget::displayByGroup() {
    displayGroupButton->activeOff();
    displaySpeciesButton->activeOn();
    displayMCButton->activeOn();

    managerGroup->displayOn();
    managerSpecies->displayOff();
    managerMC->displayOff();

    positionSlidersForGroups();
    updateGL();
}

void MyQGLWidget::displayBySpecies() {
    displaySpeciesButton->activeOff();
    displayGroupButton->activeOn();
    displayMCButton->activeOn();

    managerSpecies->displayOn();
    managerGroup->displayOff();
    managerMC->displayOff();

    positionSlidersForSpecies();
    updateGL();
}

void MyQGLWidget::displayMonteCarlo() {
    //kmc->run();
    displaySpeciesButton->activeOn();
    displayGroupButton->activeOn();
    displayMCButton->activeOff();

    managerMC->displayOn();
    managerSpecies->displayOff();
    managerGroup->displayOff();
}

void MyQGLWidget::positionSlidersForSpecies() {
    for (unsigned int i = 0; i < sliders->size(); i++) {
        sliders->at(i)->titlePositionAbove();
    }
    
    std::vector<LineChart *> *charts = managerSpecies->getCharts();
    //float width = max(charts->at(0)->getInnerWidth() * 0.50, 220);

    for (unsigned int i = 0; i < sliders->size(); i++) {
        std::string sliderTitle = sliders->at(i)->getTitle();
        std::string guild = sliderTitle.substr(0, sliderTitle.length() - labelSuffix.length());
        QString qGuild = QString::fromStdString(guild);

        std::vector<int> indices;
        
        for (unsigned int j = 0; j < charts->size(); j++) {
            std::string speciesTitle = charts->at(j)->getTitle();
            QString qSpeciesTitle = QString::fromStdString(speciesTitle);
            QString speciesGuild = mainWindow->getParameters()->getGuildMembership(qSpeciesTitle);

            if (QString::compare(qGuild, speciesGuild) == 0) {
                indices.push_back(j);
            }
        }

        if (indices.size() % 2 == 1) {
            int middle = indices[indices.size() / 2];
            LineChart *middleChart = charts->at(middle);
            float y = middleChart->getYLocation() + middleChart->getOffsetY() + middleChart->getInnerHeight() / 2;
            sliders->at(i)->setLocation(65, y);
        } else {
            int a = indices[indices.size() / 2];
            int b = indices[indices.size() / 2 - 1];

            LineChart *chartA = charts->at(a);
            LineChart *chartB = charts->at(b);
            float yA = chartA->getYLocation() + chartA->getOffsetY() + chartA->getInnerHeight() / 2;
            float yB = chartB->getYLocation() + chartB->getOffsetY() + chartB->getInnerHeight() / 2;
            sliders->at(i)->setLocation(65, (yA + yB) / 2);
        }

        sliders->at(i)->setWidth(220);
        sliders->at(i)->setLabelFontHeight(10);
        sliders->at(i)->setTitleFontHeight(12);
        
        Color *c=Color::getEvenlyDistributedColor(sliders->size(), i);
        sliders->at(i)->setMainColor(c);
        sliders->at(i)->setCursorColor(c);
    }
    
    positionSliderButtons();
}

void MyQGLWidget::setSliderFontSizes() {
    float labelHeight = 10;
    float titleHeight = 12;

    if (size().rwidth() < 900) {
        labelHeight = 8;
        titleHeight = 10;
    } else if (size().rwidth() < 1100) {
        labelHeight = 9;
        titleHeight = 11;
    }

    for (unsigned int i = 0; i < sliders->size(); i++) {
        sliders->at(i)->setLabelFontHeight(labelHeight);
        sliders->at(i)->setTitleFontHeight(titleHeight);
        sliders->at(i)->setMainColor(&Color::gray);
        sliders->at(i)->setCursorColor(&Color::gray);
    }
}

void MyQGLWidget::positionSlidersForGroups() {  
    if (managerGroup->empty()) {
        return;
    }   

    for (unsigned int i = 0; i < sliders->size(); i++) {
        MultiSpeciesLineChart *chart = managerGroup->getChartAt(i);
        float width = min(chart->getInnerWidth() * 0.65, 220);
        sliders->at(i)->setLocation(chart->getXLocation() + 55, chart->getYLocation() - 30);
        sliders->at(i)->titlePositionRight();
        sliders->at(i)->setWidth(width);
    }
    
    setSliderFontSizes();
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
        if (toggleAbsButton->getValue()) {
            managerSpecies->displayAbsoluteSizesOn();            
        } else {
            managerSpecies->displayAbsoluteSizesOff();
        }

        updateGL();
    }
}

void MyQGLWidget::toggleCharts() {
    if (managerSpecies->getDisplay()) {
        if (toggleChartsButton->getValue()) {
            managerSpecies->displayChartsOn();
            managerSpecies->displayHarvestOff();
            toggleHarvButton->setValue(false);
        } else {
            managerSpecies->displayChartsOff();
        }

        updateGL();
    }
}

void MyQGLWidget::toggleHarvest() {
    if (managerSpecies->getDisplay()) {
        if (toggleHarvButton->getValue()) {
            managerSpecies->displayHarvestOn();
            managerSpecies->displayChartsOff();
            toggleChartsButton->setValue(false);
        } else {
            managerSpecies->displayHarvestOff();
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

void MyQGLWidget::setBaseline() {
    setSlidersBaseline();
    captureLastValues();
}

void MyQGLWidget::monteCarloStreaks() {
    managerMC->displayStreaks();

    streaksButton->activeOff();    
    boxPlotsButton->activeOn();
    errorBandsButton->activeOn();
    errorBarsButton->activeOn();
}

void MyQGLWidget::monteCarloBoxPlots() {
    managerMC->displayBoxPlots();

    streaksButton->activeOn();    
    boxPlotsButton->activeOff();
    errorBandsButton->activeOn();
    errorBarsButton->activeOn();
}

void MyQGLWidget::monteCarloErrorBands() {
    managerMC->displayErrorBands();

    streaksButton->activeOn();    
    boxPlotsButton->activeOn();
    errorBandsButton->activeOff();
    errorBarsButton->activeOn();
}

void MyQGLWidget::monteCarloErrorBars() {
    managerMC->displayErrorBars();

    streaksButton->activeOn();    
    boxPlotsButton->activeOn();
    errorBandsButton->activeOn();
    errorBarsButton->activeOff();
}

void MyQGLWidget::toggleDynamicArcs() {
    if (toggleArcsDynamicButton->getValue()) {
       managerSpecies->displayArcsDynamicallyOn();
    } else {
       managerSpecies->displayArcsDynamicallyOff();
    }
}

void MyQGLWidget::displayGhostOff() {
    changeLineButton->activeOn();
    changeBlendButton->activeOn();
    changeOffButton->activeOff();

    managerGroup->displayGhostOff();
    managerSpecies->displayGhostOff();
    managerMC->displayGhostOff();
}

void MyQGLWidget::displayGhostAsLine() {
    changeLineButton->activeOff();
    changeBlendButton->activeOn();
    changeOffButton->activeOn();

    managerGroup->displayGhostAsLine();
    managerSpecies->displayGhostAsLine();
    managerMC->displayGhostOff();
}

void MyQGLWidget::displayGhostAsBlend() {
    changeLineButton->activeOn();
    changeBlendButton->activeOff();
    changeOffButton->activeOn();

    managerGroup->displayGhostAsBlend();
    managerSpecies->displayGhostAsBlend();
    managerMC->displayGhostOff();
}