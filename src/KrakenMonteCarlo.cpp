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
        // plot model
        plotManager->addJitteredValues(mainWindow);
    }

    resetValues();
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
    parameters->setInitialBiomassList(makeJitteredList(origInitBio, jitter));

    parameters->setPredationMatrix(makeJitteredMatrixWithinRange(origPredation, jitter, 0, 1));
    parameters->setWithinGuildCompMatrix(makeJitteredMatrixWithinRange(origInteraction, jitter, 0, 1));
    parameters->setGrowthRates(makeJitteredListWithinRange(origGrowth, jitter, 0, 1));
    parameters->setCatchabilitiesList(makeJitteredListWithinRange(origCatchability, jitter, 0, 1));
}