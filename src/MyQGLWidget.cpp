#include <QtGui/QMouseEvent>
#include "MyQGLWidget.h"
#include "stdio.h"

#include "AbsoluteSizeIndicator.h"
#include "InterSpeciesArc.h"
#include "Button.h"
#include "ButtonGroup.h"
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
#include "FourPanelManager.h"
#include "SmallMultiplesWithArcsManager.h"
#include "PrintText.h"
#include "ResetButton.h"
#include "SmallMultiple.h"
#include "SliderButton.h"
#include "UndoButton.h"
#include "ToggleButton.h"
#include "MonteCarloPlotManager.h"
#include "KrakenMonteCarlo.h"

#include <QList>
#include <QStringList>
#include <iostream>

MyQGLWidget::MyQGLWidget(MS_PROD_MainWindow *mainWindow, QWidget *parent) : QGLWidget(parent) {
    plotManagers = new std::vector<PlotManager *>();
    sliders = new std::vector<ChangeSlider *>();
    sliderButtons = new std::vector<SliderButton *>();
    speciesButtons = new std::vector<Button *>();
    speciesGroupButtons = new std::vector<Button *>();
    monteCarloButtons = new std::vector<Button *>();

    //CW_CODE this triggers timerEvent callback
	startTimer(0); //QT_Timer has the effect of glutIdle timer is called whenever widget is inactive

    paddingRight = 0;
    paddingLeft = 0;
    paddingBottom = 0;
    paddingTop = 45;

    timer = 0;

    this->mainWindow = mainWindow;

    setMouseTracking(true);
    setMinimumWidth(800);
    setMinimumHeight(600);

    labelSuffix = " harvest effort";
    
    managerPanel = new FourPanelManager();
    managerPanel->displayOff();

    managerSmallMult = new SmallMultiplesWithArcsManager();

    managerMC = new MonteCarloPlotManager();
    managerMC->displayOff();

    kmc = new KrakenMonteCarlo(mainWindow, managerMC);

    plotManagers->push_back(managerPanel);
    plotManagers->push_back(managerSmallMult);
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

    deleteButtonList(speciesButtons);
    deleteButtonList(speciesGroupButtons);
    deleteButtonList(monteCarloButtons);

    delete bgUncertaintyStats;
    delete bgUncertaintyLine;
    delete bgView;
    delete bgChange;
    delete bgArc;
    delete bgArcStyle;
    delete bgArcAnimate;
    delete bgUncertainty;
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

void MyQGLWidget::timerEvent(QTimerEvent *) {
    //CW_CODE  This is called when startTimer(0); has been initialized

    updateGL(); 

    //std::cerr << "TT " << timer << "\n";
}

void MyQGLWidget::paintGL() {
    ++timer; //CW_CODE for animation

    glClear(GL_COLOR_BUFFER_BIT);

    if (!managerPanel->empty()) {
        glPushMatrix();
            glTranslatef(paddingLeft, paddingBottom, 0);
            for (unsigned int i = 0; i < plotManagers->size(); i++) {
                if (plotManagers->at(i)->getDisplay()) {
                    plotManagers->at(i)->draw(size().rwidth() - paddingRight - paddingLeft, size().rheight() - paddingTop - paddingBottom);
                }
            }
        glPopMatrix();

        float xPos = size().rwidth() - 100;
        float yPos = size().rheight() - 25;
        float panelTopY = size().rheight() - 30;
        float panelBottomY = size().rheight() - 60;
        float panelStartX = 5;
        float spacing = 5;
        
        baselineButton->setLocation(280, yPos);
        resetAllButton->setLocation(355, yPos);

        if (managerSmallMult->getDisplay() || managerPanel->getDisplay()) {
            bgChange->setLocation(panelStartX, panelBottomY);
            bgChange->draw();

            float changeWidth = bgChange->getWidth();

            if (managerSmallMult->getDisplay()) {
                positionSlidersForSpecies();
               
                bgArc->setLocation(panelStartX + changeWidth + spacing, panelBottomY);
                bgArc->draw();

                if (bgArc->getActive(3)) {
                    bgArcStyle->setLocation(bgArc->getX() + bgArc->getWidth() + spacing, bgArc->getY());
                    bgArcStyle->draw();

                    bgArcAnimate->setLocation(bgArcStyle->getX() + bgArcStyle->getWidth() + spacing, bgArcStyle->getY());
                    bgArcAnimate->draw();
                }

                float arcWidth = bgArc->getWidth();

                float xPos = 445;
                toggleAbsButton->setLocation(xPos, yPos);

                xPos = xPos + toggleAbsButton->getWidth() + spacing;
                toggleChartsButton->setLocation(xPos, yPos);
                
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
        } else {
            runMCButton->setLocation(280, yPos);
            runMCButton->draw();

            bgUncertainty->setLocation(panelStartX, panelBottomY);
            bgUncertainty->draw();
            
            if (bgUncertainty->getActive(0)) {
                float xx = panelStartX + spacing + bgUncertainty->getWidth();

                bgUncertaintyStats->setLocation(xx, panelBottomY);
                bgUncertaintyStats->draw();
                
                bgUncertaintyLine->setLocation(xx + spacing + bgUncertaintyStats->getWidth(), panelBottomY);
                bgUncertaintyLine->draw();
            }
        }

        bgView->setLocation(panelStartX, panelTopY);
        bgView->draw();
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
    if (managerPanel->empty()) {
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
    if (bgView->mouseReleased(x, y)) {
        int releasedIndex = bgView->getReleasedIndex();

        if (releasedIndex == 0) {
            displayByGroup();
        } else if (releasedIndex == 1) {
            displayBySpecies();
        } else if (releasedIndex == 2) {
            displayMonteCarlo();
        }

        return true;
    }

    if (managerSmallMult->getDisplay() || managerPanel->getDisplay()) {
        if (bgChange->mouseReleased(x, y)) {
            int releasedIndex = bgChange->getReleasedIndex();
            
            if (releasedIndex == 0) {
                displayGhostAsLine();
            } else if (releasedIndex == 1) {
                displayGhostAsBlend();
            } else if (releasedIndex == 2) {
                displayGhostOff();
            }

            return true;
        }

        if (resetAllButton->mouseReleased(x, y)) {
            resetAllSliders();
            return true;
        }
        if (baselineButton->mouseReleased(x, y)) {
            setBaseline();
            return true;
        } 
    }

    if (managerSmallMult->getDisplay()) {
        if (bgArc->mouseReleased(x, y)) {
            int releasedIndex = bgArc->getReleasedIndex();

            if (releasedIndex == 0) {      
                managerSmallMult->displayInteraction();
            } else if (releasedIndex == 1) {
                managerSmallMult->displayPredation();
            } else if (releasedIndex == 2) {
                managerSmallMult->displayBothArcs();
            } else if (releasedIndex == 3) {              
                managerSmallMult->displayNoArcs();
            }
                
            return true;
        }

        if (bgArc->getActive(3)) {
            if (bgArcStyle->mouseReleased(x, y)) {
                int releasedIndex = bgArcStyle->getReleasedIndex();

                if (releasedIndex == 0) {
                    managerSmallMult->displayArcsDynamicallyOff();
                } else if (releasedIndex == 1) {
                    managerSmallMult->displayArcsDynamicallyOn();
                }

                return true;
            }

            if (bgArcAnimate->mouseReleased(x, y)) {
                int releasedIndex = bgArcAnimate->getReleasedIndex();

                if (releasedIndex == 0) {
                    managerSmallMult->arcsAnimatedOn();
                } else if (releasedIndex == 1) {
                    managerSmallMult->arcsAnimatedOff();    
                }

                return true;
            }
        }
        
        if (toggleChartsButton->mouseReleased(x, y)) {
            toggleCharts();
            return true;
        }            
        if (toggleAbsButton->mouseReleased(x, y)) {
            toggleAbsoluteSizes();
            return true;
        }
        /*if (toggleHarvButton->mouseReleased(x, y)) {
            toggleHarvest();
            return true;
        }*/
    }

    if (managerMC->getDisplay()) {
        if (runMCButton->mouseReleased(x, y)) {
            kmc->run();
            
            bgUncertainty->setActive(0, !managerMC->getDisplayStreaks());
            bgUncertainty->setActive(1, !managerMC->getDisplayBoxPlots());
            bgUncertainty->setActive(2, !managerMC->getDisplayErrorBars());
            bgUncertainty->setActive(3, !managerMC->getDisplayErrorBands());        

            bgUncertaintyStats->setActive(0, !managerMC->getUsingQuartiles());
            bgUncertaintyStats->setActive(1, !managerMC->getUsingStandardDeviations());

            bgUncertaintyLine->setActive(0, !managerMC->getDisplayOriginalLine());
            bgUncertaintyLine->setActive(1, !managerMC->getDisplayMeanLine());
            bgUncertaintyLine->setActive(2, !managerMC->getDisplayMedianLine());

            return true;
        }
        if (bgUncertaintyLine->mouseReleased(x, y)) {
            int releasedIndex = bgUncertaintyLine->getReleasedIndex();

            if (releasedIndex == 0) {
                managerMC->displayOriginalLineOn();
                managerMC->displayMeanLineOff();
                managerMC->displayMedianLineOff();
            } else if (releasedIndex == 1) {
                managerMC->displayMeanLineOn();
                managerMC->displayMedianLineOff();
                managerMC->displayOriginalLineOff();
            } else if (releasedIndex == 2) {
                managerMC->displayMedianLineOn();
                managerMC->displayMeanLineOff();
                managerMC->displayOriginalLineOff();
            }

            return true;
        }
        if (bgUncertaintyStats->mouseReleased(x, y)) {
            int releasedIndex = bgUncertaintyStats->getReleasedIndex();

            if (releasedIndex == 0) {
                managerMC->useQuartiles();
            } else if (releasedIndex == 1) {
                managerMC->useStandardDeviations();
            }
        }
        if (bgUncertainty->mouseReleased(x, y)) {
            int releasedIndex = bgUncertainty->getReleasedIndex();

            if (releasedIndex == 0) {
                managerMC->displayStreaks();
            } else if (releasedIndex == 1) {
                managerMC->displayBoxPlots();
            } else if (releasedIndex == 2) {
                managerMC->displayErrorBands();
            } else if (releasedIndex == 3) {
                managerMC->displayErrorBars();
            }

            return true;
        }
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
    if (managerPanel->empty()) {
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
    if (bgView->mousePressed(x, y)) {
        return true;
    }


    if (managerSmallMult->getDisplay()) {
        for (unsigned int i = 0; i < speciesButtons->size(); i++) {
            if (speciesButtons->at(i)->mousePressed(x, y)) {
                return true;
            }
        }

        if (bgArc->mousePressed(x, y)) {
            return true;
        }

        if (bgArc->getActive(3)) {
            if (bgArcStyle->mousePressed(x, y)) {
                return true;
            }

            if (bgArcAnimate->mousePressed(x, y)) {
                return true;
            }
        }
    }

    if (managerSmallMult->getDisplay() || managerPanel->getDisplay()) {
        if (bgChange->mousePressed(x, y)) {
            return true;
        }

        for (unsigned int i = 0; i < speciesGroupButtons->size(); i++) {
            if (speciesGroupButtons->at(i)->mousePressed(x, y)) {
                return true;
            }
        }
    }

    if (managerMC->getDisplay()) {
        if (bgUncertainty->mousePressed(x, y)) {
            return true;
        }

        if (bgUncertaintyStats->mousePressed(x, y)) {
            return true;
        }

        if (bgUncertaintyLine->mousePressed(x, y)) {
            return true;
        }

        for (unsigned int i = 0; i < monteCarloButtons->size(); i++) {
            if (monteCarloButtons->at(i)->mousePressed(x, y)) {
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
    if (managerPanel->empty()) {
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
    if (bgView->mouseMoved(x, y)) {
        return true;
    }

    if (managerSmallMult->getDisplay() || managerPanel->getDisplay()) {
        if (mouseMoveButtonHelper(speciesGroupButtons, x, y)) {
            return true;
        }

        if (bgArc->getActive(3)) {
            if (bgArcStyle->mouseMoved(x, y)) {
                return true;
            }

            if (bgChange->mouseMoved(x, y)) {
                return true;
            }
        }
    }

    if (managerSmallMult->getDisplay()) {        
        if (mouseMoveButtonHelper(speciesButtons, x, y)) {
            return true;
        }

        if (bgArc->mouseMoved(x, y)) {
            return true;
        }

        if (bgArcAnimate->mouseMoved(x, y)) {
            return true;
        }
    }

    if (managerMC->getDisplay()) {
        if (mouseMoveButtonHelper(monteCarloButtons, x, y)) {
            return true;
        }

        if (bgUncertainty->mouseMoved(x, y)) {
            return true;
        }

        if (bgUncertaintyStats->mouseMoved(x, y)) {
            return true;
        }

        if (bgUncertaintyLine->mouseMoved(x, y)) {
            return true;
        }
    }

    return false;
}

bool MyQGLWidget::mouseMoveButtonHelper(std::vector<Button *> *list, float x, float y) {
    Button *moved = NULL;

    for (unsigned int i = 0; i < list->size(); i++) {
        bool buttonMoved = list->at(i)->mouseMoved(x, y);

        if (buttonMoved) {
            moved = list->at(i);
        }
    }

    return (moved != NULL);
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
    if (!managerPanel->empty()) {
        std::vector<Pickable *> *allPickables = new std::vector<Pickable *>();
        for (unsigned int i = 0; i < plotManagers->size(); i++) {
            if (plotManagers->at(i)->getDisplay()) {
                std::vector<Pickable *> *p = plotManagers->at(i)->getPickables();
                allPickables->insert(allPickables->end(), p->begin(), p->end());
            }
        }

        InterSpeciesArcList *allArcs = managerSmallMult->getArcs();
        if (allArcs != NULL) {
            allPickables->insert(allPickables->end(), allArcs->begin(), allArcs->end());
        }

        if (managerSmallMult->getDisplay() 
            && managerSmallMult->getDisplayAbsoluteSizes() 
            && !managerSmallMult->getDisplayCharts()) {
            std::vector<AbsoluteSizeIndicator *> *absPoints = managerSmallMult->getAbsPoints();
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


void MyQGLWidget::capturePreviousValues() {
    for (unsigned int i = 0; i < plotManagers->size(); i++) {
        plotManagers->at(i)->capturePreviousValues();
    }
}

void MyQGLWidget::initialize() {
    gr = new GroupReordering(mainWindow);
    managerSmallMult->setGroupReordering(gr);
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

    toggleAbsButton = new ToggleButton("Abs. Sizes", true);
    toggleAbsButton->setHeight(buttonHeight);
    speciesButtons->push_back(toggleAbsButton);

    toggleChartsButton = new ToggleButton("Biomass", true);
    toggleChartsButton->setHeight(buttonHeight);
    speciesButtons->push_back(toggleChartsButton);

    /*toggleHarvButton = new ToggleButton("Harvest", false);
    toggleHarvButton->setHeight(buttonHeight);
    speciesButtons->push_back(toggleHarvButton);*/
    
    runMCButton = new Button("Run Simulation");
    runMCButton->setHeight(buttonHeight);
    runMCButton->setWidth(buttonWidth);
    monteCarloButtons->push_back(runMCButton);
    
    std::vector<std::string> viewLabels;
    viewLabels.push_back("Four Panel");
    viewLabels.push_back("Small Mult.");
    viewLabels.push_back("Uncertainty");
    bgView = new ButtonGroup("View", viewLabels, 0);

    std::vector<std::string> changeLabels;
    changeLabels.push_back("Line");
    changeLabels.push_back("Blend");
    changeLabels.push_back("Off");
    bgChange = new ButtonGroup("Change", changeLabels, 1);

    std::vector<std::string> arcLabels;
    arcLabels.push_back("Interaction");
    arcLabels.push_back("Predation");
    arcLabels.push_back("Both");
    arcLabels.push_back("Off");
    bgArc = new ButtonGroup("Arcs", arcLabels, 2);

    std::vector<std::string> arcAnimation;
    arcAnimation.push_back("On");
    arcAnimation.push_back("Off");
    bgArcAnimate = new ButtonGroup("Arc Animation", arcAnimation, 1);
    
    std::vector<std::string> arcStyle;
    arcStyle.push_back("Static");
    arcStyle.push_back("Dynamic");
    bgArcStyle = new ButtonGroup("Arc Style", arcStyle, 0);

    std::vector<std::string> mcLabels;
    mcLabels.push_back("Multi-Line");
    mcLabels.push_back("Box Plots");
    mcLabels.push_back("Error Bands");
    mcLabels.push_back("Error Bars");
    bgUncertainty = new ButtonGroup("Type", mcLabels, 0);
    bgUncertainty->activeOff();

    std::vector<std::string> statsType;
    statsType.push_back("Quartiles");
    statsType.push_back("Std Dev");
    bgUncertaintyStats = new ButtonGroup("Stats Type", statsType, 0);
    bgUncertaintyStats->activeOff();

    std::vector<std::string> lineType;
    lineType.push_back("Original");
    lineType.push_back("Mean");
    lineType.push_back("Median");
    bgUncertaintyLine = new ButtonGroup("Line Type", lineType, 1);
    bgUncertaintyLine->activeOff();

    displayByGroup();
}

void MyQGLWidget::displayByGroup() {
    managerPanel->displayOn();
    managerSmallMult->displayOff();
    managerMC->displayOff();

    positionSlidersForGroups();
    updateGL();
}

void MyQGLWidget::displayBySpecies() {
    managerSmallMult->displayOn();
    managerPanel->displayOff();
    managerMC->displayOff();

    positionSlidersForSpecies();
    updateGL();
}

void MyQGLWidget::displayMonteCarlo() {
    //kmc->run();

    managerMC->displayOn();
    managerSmallMult->displayOff();
    managerPanel->displayOff();
}

void MyQGLWidget::positionSlidersForSpecies() {
    for (unsigned int i = 0; i < sliders->size(); i++) {
        sliders->at(i)->titlePositionAbove();
    }
    
    std::vector<LineChart *> *charts = managerSmallMult->getCharts();
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
    if (managerPanel->empty()) {
        return;
    }   

    for (unsigned int i = 0; i < sliders->size(); i++) {
        MultiSpeciesLineChart *chart = managerPanel->getChartAt(i);
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
    if (managerSmallMult->getDisplay()) {
        if (toggleAbsButton->getValue()) {
            managerSmallMult->displayAbsoluteSizesOn();            
        } else {
            managerSmallMult->displayAbsoluteSizesOff();
        }

        updateGL();
    }
}

void MyQGLWidget::toggleCharts() {
    if (managerSmallMult->getDisplay()) {
        if (toggleChartsButton->getValue()) {
            managerSmallMult->displayChartsOn();
            managerSmallMult->displayHarvestOff();
            //toggleHarvButton->setValue(false);
        } else {
            managerSmallMult->displayChartsOff();
        }

        updateGL();
    }
}
/*
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
}*/

void MyQGLWidget::setBaseline() {
    setSlidersBaseline();
    capturePreviousValues();
}

void MyQGLWidget::displayGhostOff() {
    managerPanel->displayGhostOff();
    managerSmallMult->displayGhostOff();
    managerMC->displayGhostOff();
}

void MyQGLWidget::displayGhostAsLine() {
    managerPanel->displayGhostAsLine();
    managerSmallMult->displayGhostAsLine();
    managerMC->displayGhostOff();
}

void MyQGLWidget::displayGhostAsBlend() {
    managerPanel->displayGhostAsBlend();
    managerSmallMult->displayGhostAsBlend();
    managerMC->displayGhostOff();
}