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
class PlotBySpeciesManager;
class ResetButton;
class Slider;
class SliderButton;
class ToggleButton;
class UndoButton;

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
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    Picker *picker;    
    PlotByGroupManager *managerGroup;
    PlotBySpeciesManager *managerSpecies;
    Button *baselineButton;
    Button *resetAllButton;
    Button *displayGroupButton, *displaySpeciesButton;
    ToggleButton *toggleAbsButton, *toggleChartsButton;
    ToggleButton *togglePredButton, *toggleInterButton;
    ToggleButton *toggleHarvButton;
    
    MS_PROD_MainWindow *mainWindow;

    std::vector<PlotManager *> *plotManagers;
    std::vector<ChangeSlider *> *sliders;
    std::vector<SliderButton *> *sliderButtons;
    std::vector<Button *> *speciesButtons;
    std::vector<Button *> *alwaysDisplayingButtons;

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
};

#endif  /* _MYQGLWIDGET_H */