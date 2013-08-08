#ifndef _MYQGLWIDGET_H
#define _MYQGLWIDGET_H

#include <QtOpenGL/QGLWidget>
#include <vector>

class Button;
class ChangeSlider;
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

    void updateCharts(QList<QList<double>> matrix, QStringList labels); 

    void initializeSliders();
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
    Button *resetAllButton;
    Button *displayGroupButton, *displaySpeciesButton;
    Button *toggleAbsButton, *toggleChartsButton;
    std::vector<SliderButton *> sliderButtons;
    std::vector<Button *> buttons;

    MS_PROD_MainWindow *mainWindow;
    
    PlotByGroupManager *managerGroup;
    PlotBySpeciesManager *managerSpecies;

    std::vector<PlotManager *> plotManagers;

    std::vector<ChangeSlider *> sliders;

    std::string labelSuffix;

    bool mouseReleaseButtons(float x, float y);

    bool mousePressButtons(float x, float y);
    bool mousePressSliders(float x, float y);

    bool mouseMoveButtons(float x, float y);
    bool mouseMoveSliders(float x, float y);
    void mouseMovePickables(int x, int y);

    void captureLastValues();

    void toggleAbsoluteSizes();
    void toggleCharts();

    void displayByGroup();
    void displayBySpecies();

    void positionSlidersForSpecies();
    void positionSlidersForGroups();
    void positionSliderButtons();

    void clearDisplayOfSliders();
    void resetAllSliders();
    void updateEffortToSlider(Slider *slider);
    void setEffort(float value, std::string guildName);
};

#endif  /* _MYQGLWIDGET_H */