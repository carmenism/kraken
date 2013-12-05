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
class PlotByGroupManager;
class PlotBySpeciesWithArcsManager;
class MonteCarloPlotManager;
class KrakenMonteCarlo;
class ResetButton;
class Slider;
class SliderButton;
class ToggleButton;
class UndoButton;
class GroupReordering;

typedef std::vector<MultiSpeciesLineChart *> MSLineChartList;
typedef std::vector<MultiSpeciesLineChart *>::const_iterator MSLineChartIterator;

#define FOREACH_MSLINECHART(it, msLineChartList) \
    for(MSLineChartIterator it = msLineChartList.begin(); it != msLineChartList.end(); ++it)
#define FOREACH_MSLINECHARTP(it, msLineChartList) \
    for(MSLineChartIterator it = msLineChartList->begin(); it != msLineChartList->end(); ++it)

class MyQGLWidget : public QGLWidget {

    Q_OBJECT // must include this if you use Qt signals/slots

public:
    MyQGLWidget(MS_PROD_MainWindow *mainWindow, QWidget *parent = NULL);
    ~MyQGLWidget();

    void updateCharts(Model *model); 

    void initialize();
    void drawToPick();

    void runModel();

    int getPaddingTop() { return paddingTop; }
    int getPaddingBottom() { return paddingBottom; }
    int getPaddingRight() { return paddingRight; }
    int getPaddingLeft() { return paddingLeft; }
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    int paddingTop;
    int paddingBottom;
    int paddingRight;
    int paddingLeft;

    GroupReordering *gr;
    KrakenMonteCarlo *kmc;
    Picker *picker;    
    PlotByGroupManager *managerGroup;
    PlotBySpeciesWithArcsManager *managerSpecies;
    MonteCarloPlotManager *managerMC;
    Button *runMCButton;
    Button *baselineButton;
    Button *resetAllButton;
    Button *changeLineButton, *changeBlendButton, *changeOffButton;
    Button *displayGroupButton, *displaySpeciesButton, *displayMCButton;
    ToggleButton *toggleAbsButton, *toggleChartsButton;
    ToggleButton *togglePredButton, *toggleInterButton;
    ToggleButton *toggleHarvButton;
    ToggleButton *toggleArcsDynamicButton;
    
    Button *streaksButton;
    Button *boxPlotsButton;
    Button *errorBandsButton;
    Button *errorBarsButton;
    
    MS_PROD_MainWindow *mainWindow;

    std::vector<PlotManager *> *plotManagers;
    std::vector<ChangeSlider *> *sliders;
    std::vector<SliderButton *> *sliderButtons;
    std::vector<Button *> *speciesButtons;
    std::vector<Button *> *displayButtons;
    std::vector<Button *> *speciesGroupButtons;
    std::vector<Button *> *monteCarloButtons;

    std::string labelSuffix;

    bool mouseReleaseButtons(float x, float y);

    bool mousePressButtons(float x, float y);
    bool mousePressSliders(float x, float y);

    bool mouseMoveButtons(float x, float y);
    bool mouseMoveSliders(float x, float y);
    void mouseMovePickables(int x, int y);

    void setBaseline();

    void captureLastValues();

    void toggleAbsoluteSizes();
    void toggleCharts();
    void toggleHarvest();

    void displayByGroup();
    void displayBySpecies();
    void displayMonteCarlo();

    void togglePredation();
    void toggleInteraction();

    void positionSlidersForSpecies();
    void positionSlidersForGroups();
    void positionSliderButtons();

    void setSlidersBaseline();
    void resetAllSliders();
    void updateEffortToSlider(Slider *slider);
    void setEffort(float value, std::string guildName);

    void setSliderFontSizes();

    void deleteButtonList(std::vector<Button *> *list);
    Button *mouseMoveButtonHelper(std::vector<Button *> *list, float x, float y);

    void monteCarloStreaks();
    void monteCarloBoxPlots();
    void monteCarloErrorBands();
    void monteCarloErrorBars();

    void toggleDynamicArcs();

    void displayGhostOff();
    void displayGhostAsLine();
    void displayGhostAsBlend();

    void drawBox(float x, float y, float w, float h);
};

#endif  /* _MYQGLWIDGET_H */