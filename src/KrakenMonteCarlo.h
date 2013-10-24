#ifndef KRAKENMONTECARLO_H_
#define KRAKENMONTECARLO_H_

#include <QList>
#include "MonteCarlo.h"

class MS_PROD_MainWindow;
class Parameters;
class MonteCarloPlotManager;

class KrakenMonteCarlo : public MonteCarlo {
public:
    KrakenMonteCarlo(MS_PROD_MainWindow *mw, MonteCarloPlotManager *pm);
    ~KrakenMonteCarlo();

    void run();
protected:
    double jitter;
    MS_PROD_MainWindow *mainWindow;
    Parameters *parameters;
    MonteCarloPlotManager *plotManager;

    QList<QList<double>> origPredation;
    QList<QList<double>> origInteraction;
    QList<double> origCatchability;
    QList<double> origInitBio;
    //QList<double> origEffort;
    QList<double> origGrowth;

    void saveValues();
    void resetValues();
    void generateJitteredValues();
};

#endif /* KRAKENMONTECARLO_H_ */