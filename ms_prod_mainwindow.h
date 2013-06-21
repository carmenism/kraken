#ifndef MS_PROD_MAINWINDOW_H
#define MS_PROD_MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include "Parameters.h"
#include "TimeSeriesObservations.h"
#include "MeasureOfFitness.h"
#include "ModelFormParameters.h"
#include "GrowthModuleLogistic.h"
#include "PredationModuleTypeI.h"
#include "PredationModuleTypeII.h"
#include "PredationModuleTypeIII.h"
#include "HarvestModuleEffortQ.h"
#include "HarvestModuleExploitRate.h"
#include "Model.h"
#include "GA_Estimator.h"

class MyQGLWidget;

namespace Ui {
    class MS_PROD_MainWindow;
}

class MS_PROD_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MS_PROD_MainWindow(QWidget *parent = 0);
    ~MS_PROD_MainWindow();

    void setGLWidget(MyQGLWidget *w) { glWidget = w; }
private:
    Ui::MS_PROD_MainWindow *ui;

    MyQGLWidget *glWidget;

    Parameters paramObj;
    TimeSeriesObservations tsObj;
    ModelFormParameters modelFormParamObj;
    MeasureOfFitness measureOfFitnessObj;
    QStringList lineList;

    void loadFromParameterFile(QString);
    void loadFromTimeSeriesFile(QString);

    void setSystemK();
    void setSpecies();
    void setSpeciesK();
    void setSpeciesMinK();
    void setSpeciesMaxK();
    void setUnits();
    void setHarvestRates();
    void setGrowthRates();
    void setGrowthRatesMin();
    void setGrowthRatesMax();
    void setInitialBiomass();
    void setCatchabilities();
    void setEffort();
    void setPelProportionList();
    void setDemProportionList();
    void setGuilds();
    void setGuildMembership();
    void setGrowthMode();
    void setHarvestMode();
    void setPredationMode();
    void setwgcMode();
    void setbgcMode();
    void setRunLength();
    void setTimeStep();
    void set_GA_generations();
    void set_GA_populationSize();
    void set_GA_mutationRate();

    void logResults();
    void runModel();
    void testFramework();
    void testLooping();

    QString convertDoubleToString(double);
    QList<double> returnParameterList();
    QList< QList <double> >* initializeParamMatrix(QTextStream &);
    QList< QList <double> >* initializeTimeSeriesMatrix(QTextStream &);

private slots:
    void pb_Load_Clicked();
    void pb_Run_Clicked();
    void pb_Fit_Clicked();
};

#endif // MS_PROD_MAINWINDOW_H
