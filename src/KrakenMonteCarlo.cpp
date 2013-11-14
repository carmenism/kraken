#include "KrakenMonteCarlo.h"
#include "Parameters.h"
#include "InteractionMatrix.h"
#include "MS_PROD_MainWindow.h"
#include "MonteCarloPlotManager.h"

KrakenMonteCarlo::KrakenMonteCarlo(MS_PROD_MainWindow *mw, MonteCarloPlotManager *pm) {
    plotManager = pm;
    mainWindow = mw;
    parameters = mw->getParameters();
    jitter = 0.1;
}

KrakenMonteCarlo::~KrakenMonteCarlo() {
    
}

void KrakenMonteCarlo::run() {
    plotManager->updateCharts(NULL, mainWindow);

    saveValues();

    for (int i = 0; i < numberSimulations; i++) {
        generateJitteredValues();
        mainWindow->runModel(false);
        plotManager->addValues(i, mainWindow, true);
    }

    resetValues();
    mainWindow->runModel(false);
    plotManager->addValues(numberSimulations, mainWindow, false);

    plotManager->removeMarkers();

    plotManager->updateStatistics();
}

void KrakenMonteCarlo::saveValues() {
    origInitBio = parameters->getInitialBiomassList();
    origGrowth = parameters->getGrowthRateList();
    origPredation = parameters->getPredationMatrix();
    origInteraction = parameters->getWithinGuildCompMatrix();
    origCatchability = parameters->getCatchabilitiesList();//->getCatchabilityMatrix()->getMatrix();
}

void KrakenMonteCarlo::resetValues() {
    parameters->setPredationMatrix(origPredation);
    parameters->setWithinGuildCompMatrix(origInteraction);
    parameters->setInitialBiomassList(origInitBio);
    parameters->setGrowthRates(origGrowth);
    parameters->setCatchabilitiesList(origCatchability);
}

void KrakenMonteCarlo::generateJitteredValues() {
    int n = 5;
    parameters->setInitialBiomassList(makeJitteredList(origInitBio, jitter, n));

    parameters->setPredationMatrix(makeJitteredMatrixWithinRange(origPredation, jitter, 0, 1, n));
    parameters->setWithinGuildCompMatrix(makeJitteredMatrixWithinRange(origInteraction, jitter, 0, 1, n));
    parameters->setGrowthRates(makeJitteredListWithinRange(origGrowth, jitter, 0, 1, n));
    parameters->setCatchabilitiesList(makeJitteredListWithinRange(origCatchability, jitter, 0, 1, n));
}