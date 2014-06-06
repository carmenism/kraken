#ifndef _MYQGLWIDGET_H
#define _MYQGLWIDGET_H

#include <QtOpenGL/QGLWidget>
#include <vector>

class Button;
class ChangeSlider;
class Model;
class MouseEventItem;
class MS_PROD_MainWindow;
class MultiSpeciesLineChart;
class Parameters;
class Picker;
class PlotManager;
class FourPanelManager;
class SmallMultiplesWithArcsManager;
class MonteCarloPlotManager;
class KrakenMonteCarlo;
class ResetButton;
class Slider;
class SliderButton;
class ToggleButton;
class UndoButton;
class GroupReordering;
class ButtonGroup;
class EvaluationWidget;
class QCloseEvent;

enum Mode {NORMAL, EXPERIMENTAL, PRESENTATION, DYN_ANIM_ARCS_ONLY};

class MyQGLWidget : public QGLWidget {

    Q_OBJECT // must include this if you use Qt signals/slots

public:
    MyQGLWidget(MS_PROD_MainWindow *mainWindow, QWidget *parent = NULL);
    ~MyQGLWidget();

    void updateCharts(Model *model); 

    void initialize();
    void drawToPick();

    void runModel();
    
    void resetAllSliders();

    int getPaddingTop() { return paddingTop; }
    int getPaddingBottom() { return paddingBottom; }
    int getPaddingRight() { return paddingRight; }
    int getPaddingLeft() { return paddingLeft; }   

    void experimentConditionA();
    void experimentConditionB();
    void experimentConditionC();
    void experimentConditionD();
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void timerEvent(QTimerEvent *);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);    
    void closeEvent(QCloseEvent * event);
private:
    Mode mode;
    bool splinesUninitialized;

    int paddingTop;
    int paddingBottom;
    int paddingRight;
    int paddingLeft;

    int timer;
    int timerId;

    int numberManagers;

    EvaluationWidget *evalWidget;

    GroupReordering *gr;
    KrakenMonteCarlo *kmc;
    Picker *picker;    
    FourPanelManager *managerPanel;
    SmallMultiplesWithArcsManager *managerSmallMult;
    MonteCarloPlotManager *managerMC;    
    Button *runMCButton, *baselineButton, *resetAllButton;
    ButtonGroup *bgView, *bgChange, *bgArc, *bgArcStyle, *bgArcAnimate;
    ButtonGroup *bgUncertainty, *bgUncertaintyStats, *bgUncertaintyLine;
    ToggleButton *toggleAbsButton, *toggleChartsButton;
    //ToggleButton *toggleHarvButton;
    
    ButtonGroup* bgArcConditions;
    
    MS_PROD_MainWindow *mainWindow;

    PlotManager **plotManagers;
    std::vector<ChangeSlider *> *sliders;
    std::vector<SliderButton *> *sliderButtons;
    std::vector<Button *> *speciesButtons;
    std::vector<Button *> *speciesGroupButtons;
    std::vector<Button *> *monteCarloButtons;

    std::string labelSuffix;

    bool mouseReleaseButtonsNormal(float x, float y);
    bool mouseReleaseButtonsExperimental(float x, float y);
    bool mouseReleaseButtonsPresentation(float x, float y);
    bool mouseReleaseButtonsForSliders(float x, float y);
    
    bool mouseReleaseBgArcConditions(float x, float y);
    bool mouseReleaseButtonsUncertainty(float x, float y);
    bool mouseReleaseBgView(float x, float y);

    bool mousePressButtonsNormal(float x, float y);
    bool mousePressButtonsExperimental(float x, float y);
    bool mousePressButtonsPresentation(float x, float y);
    bool mousePressSliders(float x, float y);
    bool mousePressButtonsUncertainty(float x, float y);

    bool mouseMoveButtonsNormal(float x, float y);
    bool mouseMoveButtonsExperimental(float x, float y);
    bool mouseMoveButtonsPresentation(float x, float y);
    bool mouseMoveSliders(float x, float y);
    bool mouseMoveSliderButtons(float x, float y);
    bool mouseMoveButtonsUncertainty(float x, float y);
    void mouseMovePickables(int x, int y);

    void setBaseline();

    void capturePreviousValues();

    void toggleAbsoluteSizes();
    void toggleCharts();
    void toggleHarvest();

    void displayByGroup();
    void displayBySpecies();
    void displayMonteCarlo();

    void positionSlidersForSpecies();
    void positionSlidersForGroups();
    void positionSliderButtons();

    void setSlidersBaseline();
    void updateEffortToSlider(Slider *slider);
    void setEffort(float value, std::string guildName);

    void setSliderFontSizes();

    void deleteButtonList(std::vector<Button *> *list);
    bool mouseMoveButtonHelper(std::vector<Button *> *list, float x, float y);

    void displayGhostOff();
    void displayGhostAsLine();
    void displayGhostAsBlend();

    void drawNormal();
    void drawExperimental();
    void drawPresentation();
};

#endif  /* _MYQGLWIDGET_H */