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
#include "EvaluationWidget.h"
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
#include <QCloseEvent>
#include <iostream>

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

MyQGLWidget::MyQGLWidget(MS_PROD_MainWindow *mainWindow, QWidget *parent) : QGLWidget(parent) {
    mode = NORMAL;

    if (mode == EXPERIMENTAL) {
        evalWidget = new EvaluationWidget(this, NULL);
    }

    numberManagers = 3;

    plotManagers = new PlotManager *[numberManagers];
    sliders = new std::vector<ChangeSlider *>();
    sliderButtons = new std::vector<SliderButton *>();
    speciesButtons = new std::vector<Button *>();
    speciesGroupButtons = new std::vector<Button *>();
    monteCarloButtons = new std::vector<Button *>();

    //CW_CODE this triggers timerEvent callback
	timerId = startTimer(0); //QT_Timer has the effect of glutIdle timer is called whenever widget is inactive

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

    plotManagers[0] = managerPanel;
    plotManagers[1] = managerSmallMult;
    plotManagers[2] = managerMC;

    picker = new Picker(this);

    splinesUninitialized = true;
}

MyQGLWidget::~MyQGLWidget() {
    std::cerr << "ENTERING DESTRUCTOR\n";
    killTimer(timerId);

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
    delete[] plotManagers;
    delete picker;
    delete kmc;
    delete gr;

    
    std::cerr << "EXITING DESTRUCTOR\n";
}

void MyQGLWidget::closeEvent(QCloseEvent * event) {
    killTimer(timerId);

    mainWindow->close();
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

    if (!managerPanel->empty()) {
        glClear(GL_COLOR_BUFFER_BIT);

        if (mode == NORMAL) {
            drawNormal();
        } else if (mode == EXPERIMENTAL || mode == DYN_ANIM_ARCS_ONLY) {
            drawExperimental();
        } else if (mode == PRESENTATION) {
            drawPresentation();
        }
        
        if (!managerMC->getDisplay() || mode == EXPERIMENTAL) {
            for (unsigned int i = 0; i < sliders->size(); i++) {
                sliders->at(i)->draw();
            }

            for (unsigned int i = 0; i < sliderButtons->size(); i++) {
                sliderButtons->at(i)->draw();
            }  
        }
    }
}

void MyQGLWidget::drawPresentation() {
    glPushMatrix();
        glTranslatef(paddingLeft, paddingBottom, 0);
        for (int i = 0; i < numberManagers; i++) {
            if (plotManagers[i]->getDisplay()) {
                plotManagers[i]->draw(size().rwidth() - paddingRight - paddingLeft, size().rheight() - paddingTop - paddingBottom);
            }
        }
    glPopMatrix();

    float xPos = size().rwidth() - 100;
    float yPos = size().rheight() - 25;
    float panelTopY = size().rheight() - 30;
    float panelBottomY = size().rheight() - 60;
    float panelStartX = 5;
    float spacing = 5;
    
    bgView->setLocation(panelStartX, panelTopY);
    bgView->draw();

    if (managerSmallMult->getDisplay()) {
        positionSlidersForSpecies();
    } else if (managerPanel->getDisplay()) {
        positionSlidersForGroups();
    }

    if (managerSmallMult->getDisplay() || managerPanel->getDisplay()) {
        baselineButton->setLocation(280, yPos);
        baselineButton->draw();
        
        resetAllButton->setLocation(355, yPos);
        resetAllButton->draw();
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

    if (managerSmallMult->getDisplay()) {
        bgArcConditions->setLocation(5, panelBottomY);
        bgArcConditions->draw();        
    }
}

void MyQGLWidget::drawNormal() { 
    glPushMatrix();
        glTranslatef(paddingLeft, paddingBottom, 0);
        for (unsigned int i = 0; i < numberManagers; i++) {
            if (plotManagers[i]->getDisplay()) {
                plotManagers[i]->draw(size().rwidth() - paddingRight - paddingLeft, size().rheight() - paddingTop - paddingBottom);
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

                if (bgArcStyle->getActive(0)) {
                    bgArcAnimate->setLocation(bgArcStyle->getX() + bgArcStyle->getWidth() + spacing, bgArcStyle->getY());
                    bgArcAnimate->draw();
                }
            }

            float arcWidth = bgArc->getWidth();

            float xPos = 445;
            toggleAbsButton->setLocation(xPos, yPos);

            xPos = xPos + toggleAbsButton->getWidth() + spacing;
            toggleChartsButton->setLocation(xPos, yPos);
            
            for (unsigned int i = 0; i < speciesButtons->size(); i++) {
                speciesButtons->at(i)->draw();
            }
        } else {
            positionSlidersForGroups();
        }

        for (unsigned int i = 0; i < speciesGroupButtons->size(); i++) {
            speciesGroupButtons->at(i)->draw();
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

void MyQGLWidget::drawExperimental() {
    glPushMatrix();
        glTranslatef(paddingLeft, paddingBottom, 0);
        for (unsigned int i = 0; i < numberManagers; i++) {
            if (plotManagers[i]->getDisplay()) {
                plotManagers[i]->draw(size().rwidth() - paddingRight - paddingLeft, size().rheight() - paddingTop - paddingBottom);
            }
        }
    glPopMatrix();

    positionSlidersForSpecies();
}

void MyQGLWidget::drawToPick() {
    for (unsigned int i = 0; i < numberManagers; i++) {
        if (plotManagers[i]->getDisplay()) {
            plotManagers[i]->drawToPick();
        }
    }
}

void MyQGLWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (managerPanel->empty()) {
        return;
    }

    float x = event->x();
    float y = size().rheight() - event->y();

    bool mouseReleased = false;

    if (event->button() == Qt::LeftButton) {
        if (mode == NORMAL) {
            mouseReleased = mouseReleaseButtonsNormal(x, y);
        } else if (mode == EXPERIMENTAL || mode == DYN_ANIM_ARCS_ONLY) {
            mouseReleased = mouseReleaseButtonsExperimental(x, y);
        } else if (mode == PRESENTATION) {
            mouseReleased = mouseReleaseButtonsPresentation(x, y);
        }
        
        if (!mouseReleased) {
            for (unsigned int i = 0; i < sliders->size(); i++) {
                if (sliders->at(i)->mouseReleased(x, y)) {
                    break;
                }
            }
        }
    }

    updateGL();
}

bool MyQGLWidget::mousePressButtonsPresentation(float x, float y) {
    if (bgView->mousePressed(x, y)) {
        return true;
    }

    if (managerSmallMult->getDisplay()) {
        if (bgArcConditions->mousePressed(x, y)) {
            return true;
        }
    }
    
    if (managerSmallMult->getDisplay() || managerPanel->getDisplay()) {
        for (unsigned int i = 0; i < sliderButtons->size(); i++) {
            if (sliderButtons->at(i)->mousePressed(x, y)) {
                return true;
            }
        }
    } else {
        if (mousePressButtonsUncertainty(x, y)) {
            return true;
        }
    }

    return false;
}

bool MyQGLWidget::mouseReleaseButtonsPresentation(float x, float y) {
    if (mouseReleaseBgView(x, y)) {
        return true;
    }

    if (managerSmallMult->getDisplay()) {
        if (mouseReleaseBgArcConditions(x, y)) {
            return true;
        }
    }
    
    if (managerSmallMult->getDisplay() || managerPanel->getDisplay()) {
        if (resetAllButton->mouseReleased(x, y)) {
            resetAllSliders();
            return true;
        }
        if (baselineButton->mouseReleased(x, y)) {
            setBaseline();
            return true;
        } 

        if (mouseReleaseButtonsForSliders(x, y)) {
            return true;
        }
    } else {
        if (mouseReleaseButtonsUncertainty(x, y)) {
            return true;
        }
    }

    return false;
}

bool MyQGLWidget::mouseReleaseButtonsExperimental(float x, float y) {
    return mouseReleaseButtonsForSliders(x, y);
}


bool MyQGLWidget::mouseReleaseButtonsUncertainty(float x, float y) {
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

    if (bgUncertainty->getActive(0)) {
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
    
    return false;
}

bool MyQGLWidget::mouseReleaseBgArcConditions(float x, float y) {
    if (bgArcConditions->mouseReleased(x, y)) {
        int releasedIndex = bgArcConditions->getReleasedIndex();

        if (releasedIndex == 0) {      
            experimentConditionA();
        } else if (releasedIndex == 1) {
            experimentConditionB();
        } else if (releasedIndex == 2) { 
            experimentConditionC();
        } else if (releasedIndex == 3) {    
            experimentConditionD();
        }
            
        return true;
    }

    return false;
}

bool MyQGLWidget::mouseReleaseButtonsForSliders(float x, float y) {
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

bool MyQGLWidget::mouseReleaseBgView(float x, float y) {
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

    return false;
}

bool MyQGLWidget::mouseReleaseButtonsNormal(float x, float y) {
    if (mouseReleaseBgView(x, y)) {
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

            if (bgArcStyle->getActive(0)) {
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
        }
        
        if (toggleChartsButton->mouseReleased(x, y)) {
            toggleCharts();
            return true;
        }            
        if (toggleAbsButton->mouseReleased(x, y)) {
            toggleAbsoluteSizes();
            return true;
        }
    }

    if (managerMC->getDisplay()) {
        if (mouseReleaseButtonsUncertainty(x, y)) {
            return true;
        }
    } else {
        return mouseReleaseButtonsForSliders(x, y);
    }

    return false;
}

void MyQGLWidget::resetAllSliders() {
    if (managerPanel->empty()) {
        return;
    }

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
    /*std::cout << guildName << " ";
    std::cout << value;
    std::cout << "\n";*/
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
    
    bool mousePressed = false;

    if (event->button() == Qt::LeftButton) {
        if (mode == NORMAL) {
            mousePressed = mousePressButtonsNormal(x, y);
        } else if (mode == EXPERIMENTAL || mode == DYN_ANIM_ARCS_ONLY) {
            mousePressed = mousePressButtonsExperimental(x, y);
        } else if (mode == PRESENTATION) {
            mousePressed = mousePressButtonsPresentation(x, y);
        }

        if (!mousePressed) {
            bool sliderPressed = mousePressSliders(x, y);
        }
    }

    updateGL();
}

bool MyQGLWidget::mousePressButtonsExperimental(float x, float y) {
    for (unsigned int i = 0; i < sliderButtons->size(); i++) {
        if (sliderButtons->at(i)->mousePressed(x, y)) {
            return true;
        }
    }

    return false;
}

bool MyQGLWidget::mousePressButtonsNormal(float x, float y) {
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

            if (bgArcStyle->getActive(0)) {
                if (bgArcAnimate->mousePressed(x, y)) {
                    return true;
                }
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
        if (mousePressButtonsUncertainty(x, y)) {
            return true;
        }
    }

    return false;
}

bool MyQGLWidget::mousePressButtonsUncertainty(float x, float y) {
    for (unsigned int i = 0; i < monteCarloButtons->size(); i++) {
        if (monteCarloButtons->at(i)->mousePressed(x, y)) {
            return true;
        }
    }

    if (bgUncertainty->mousePressed(x, y)) {
        return true;
    }

    if (bgUncertainty->getActive(0)) {
        if (bgUncertaintyStats->mousePressed(x, y)) {
            return true;
        }

        if (bgUncertaintyLine->mousePressed(x, y)) {
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

    bool buttonMoved = false;
    
    if (mode == NORMAL) {
        buttonMoved = mouseMoveButtonsNormal(x, y);
    } else if (mode == EXPERIMENTAL || mode == DYN_ANIM_ARCS_ONLY) { 
        buttonMoved = mouseMoveButtonsExperimental(x, y);
    } else if (mode == PRESENTATION) {
        buttonMoved = mouseMoveButtonsPresentation(x, y);
    }

    if (!buttonMoved) {
        bool sliderMoved = mouseMoveSliders(x, y);

        if (!sliderMoved) {
            mouseMovePickables(x, y);
        }
    }

    updateGL();
}

bool MyQGLWidget::mouseMoveButtonsPresentation(float x, float y) {
    if (bgView->mouseMoved(x, y)) {
        return true;
    }
    
    if (managerSmallMult->getDisplay()) {
        if (bgArcConditions->mouseMoved(x, y)) {
            return true;
        }
    }
    
    if (managerSmallMult->getDisplay() || managerPanel->getDisplay()) {
        if (mouseMoveSliderButtons(x, y)) {
            return true;
        }
    } else {
        if (mouseMoveButtonsUncertainty(x, y)) {
            return true;
        }
    }

    return false;
}

bool MyQGLWidget::mouseMoveButtonsExperimental(float x, float y) {   
    if (mouseMoveSliderButtons(x, y)) {
        return true;
    }

    return false;
}

bool MyQGLWidget::mouseMoveSliderButtons(float x, float y) {
    for (unsigned int i = 0; i < sliderButtons->size(); i++) {
        if (sliderButtons->at(i)->mouseMoved(x, y)) {
            return true;
        }
    }

    return false;
}

bool MyQGLWidget::mouseMoveButtonsNormal(float x, float y) {
    if (bgView->mouseMoved(x, y)) {
        return true;
    }

    if (managerSmallMult->getDisplay() || managerPanel->getDisplay()) {
        if (mouseMoveButtonHelper(speciesGroupButtons, x, y)) {
            return true;
        }

        if (bgChange->mouseMoved(x, y)) {
            return true;
        }        
    }

    if (managerSmallMult->getDisplay()) {        
        if (mouseMoveButtonHelper(speciesButtons, x, y)) {
            return true;
        }


        if (bgArc->mouseMoved(x, y)) {
            return true;
        }        

        if (bgArc->getActive(3)) {
            if (bgArcStyle->mouseMoved(x, y)) {
                return true;
            }            

            if (bgArcStyle->getActive(0)) {
                if (bgArcAnimate->mouseMoved(x, y)) {
                    return true;
                }
            }
        }
    }

    if (managerMC->getDisplay()) {
        if (mouseMoveButtonsUncertainty(x, y)) {
            return true;
        }
    }

    return false;
}

bool MyQGLWidget::mouseMoveButtonsUncertainty(float x, float y) {
    if (mouseMoveButtonHelper(monteCarloButtons, x, y)) {
        return true;
    }

    if (bgUncertainty->mouseMoved(x, y)) {
        return true;
    }

    if (bgUncertainty->getActive(0)) {
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
        for (unsigned int i = 0; i < numberManagers; i++) {
            if (plotManagers[i]->getDisplay()) {
                std::vector<Pickable *> *p = plotManagers[i]->getPickables();
                allPickables->insert(allPickables->end(), p->begin(), p->end());
            }
        }

		std::vector<Link *> *allLinks = NULL;
		if (managerSmallMult->getDisplay()) {
			allLinks = managerSmallMult->getLinks();

			if (allLinks != NULL) {
				allPickables->insert(allPickables->end(), allLinks->begin(), allLinks->end());
			}
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
    for (unsigned int i = 0; i < numberManagers; i++) {
        plotManagers[i]->updateCharts(model, mainWindow);
    }

    if (splinesUninitialized) {
        managerSmallMult->initializeSplines(mainWindow, sliders, labelSuffix);
        splinesUninitialized = false;      

        if (mode == PRESENTATION || mode == EXPERIMENTAL) {
            experimentConditionA();
        } else if (mode == DYN_ANIM_ARCS_ONLY) {
            experimentConditionD();
        }
    }

    updateGL();    
}


void MyQGLWidget::capturePreviousValues() {
    for (unsigned int i = 0; i < numberManagers; i++) {
        plotManagers[i]->capturePreviousValues();
    }
}

void MyQGLWidget::initialize() {
    this->show();

    if (mode == EXPERIMENTAL) {
        evalWidget->show();
    }

    resize(1200, 800);

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
    std::vector<float> values;
    values.push_back(0);
    values.push_back(0.25);
    values.push_back(0.5);
    values.push_back(0.75);
    values.push_back(1.0);
    values.push_back(1.5);
    values.push_back(2.0);
    values.push_back(3);
    values.push_back(4);

    for (int i = 0; i < guilds.size(); i++) {
        std::string guild = guilds.at(i).toStdString();
        
        ChangeSlider *slider = new ChangeSlider(guild + labelSuffix, values, 4);
        
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
    
    runMCButton = new Button("Run Simulation");
    runMCButton->setHeight(buttonHeight);
    runMCButton->setWidth(buttonWidth);
    monteCarloButtons->push_back(runMCButton);
    
    std::vector<std::string> viewLabels;
    viewLabels.push_back("Four Panel");
    viewLabels.push_back("Small Mult.");
    viewLabels.push_back("Uncertainty");
    bgView = new ButtonGroup("View", viewLabels, 1);

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

    std::vector<std::string> condType;
    condType.push_back("A");
    condType.push_back("B");
    condType.push_back("C");
    condType.push_back("D");
    bgArcConditions = new ButtonGroup("Arc Condition", condType, 0);

    displayBySpecies();
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
    if (managerSmallMult->empty()) {
        return;
    }   

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
            float y = middleChart->getY() + middleChart->getOffsetY() + middleChart->getInnerHeight() / 2;
            sliders->at(i)->setLocation(65, y);
        } else {
            int a = indices[indices.size() / 2];
            int b = indices[indices.size() / 2 - 1];

            LineChart *chartA = charts->at(a);
            LineChart *chartB = charts->at(b);
            float yA = chartA->getY() + chartA->getOffsetY() + chartA->getInnerHeight() / 2;
            float yB = chartB->getY() + chartB->getOffsetY() + chartB->getInnerHeight() / 2;
            sliders->at(i)->setLocation(65, (yA + yB) / 2);
        }

        sliders->at(i)->setWidth(220);
        sliders->at(i)->setLabelFontHeight(10);
        sliders->at(i)->setTitleFontHeight(12);
        
        Color *c=Color::getEvenlyDistributedColor(sliders->size(), i);
        sliders->at(i)->setMainColor(new Color(*c));
        sliders->at(i)->setCursorColor(new Color(*c));
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
        sliders->at(i)->setMainColor(new Color(Color::gray));
        sliders->at(i)->setCursorColor(new Color(Color::gray));
    }
}

void MyQGLWidget::positionSlidersForGroups() {  
    if (managerPanel->empty()) {
        return;
    }   

    for (unsigned int i = 0; i < sliders->size(); i++) {
        MultiSpeciesLineChart *chart = managerPanel->getChartAt(i);
        float width = min(chart->getInnerWidth() * 0.65, 220);
        sliders->at(i)->setLocation(chart->getX() + 55, chart->getY() - 30);
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
        } else {
            managerSmallMult->displayChartsOff();
        }

        updateGL();
    }
}

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

void MyQGLWidget::experimentConditionA() {
    if (!managerSmallMult->empty()) {
        managerSmallMult->displayNoArcs();
    }
}

void MyQGLWidget::experimentConditionB() {
    if (!managerSmallMult->empty()) {
        managerSmallMult->displayBothArcs();
        managerSmallMult->displayArcsDynamicallyOff();
    }
}

void MyQGLWidget::experimentConditionC() {     
    if (!managerSmallMult->empty()) {
        managerSmallMult->displayBothArcs();
        managerSmallMult->displayArcsDynamicallyOn();
        managerSmallMult->arcsAnimatedOff();
    }
}

void MyQGLWidget::experimentConditionD() {     
    if (!managerSmallMult->empty()) {
        managerSmallMult->displayBothArcs();
        managerSmallMult->displayArcsDynamicallyOn();
        managerSmallMult->arcsAnimatedOn();
    }
}
