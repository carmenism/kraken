#include "ModelFormParameters.h"


ModelFormParameters::ModelFormParameters() {

    m_GrowthForm = "Logistic";
    m_PredationForm = "Type I";
    m_HarvestForm = "H";
    m_wgcForm = "MS-PROD";
    m_bgcForm = "MS-PROD";
    m_RunLength = 30.0;
    m_TimeStep = 1.0;
}


void ModelFormParameters::setGrowthForm(QString growthForm) {

    if ((growthForm == "LOGISTIC") || (growthForm == "LOGISTIC-STOCHASTIC") || (growthForm == "LINEAR")) {
        m_GrowthForm = growthForm;
    } else {
        qDebug() << "Incorrect Model Growth Form: " << growthForm;
    }
}

void ModelFormParameters::setHarvestForm(QString harvestForm) {

    if ((harvestForm == "H") || (harvestForm == "QE") || (harvestForm == "CATCH")) {
        m_HarvestForm = harvestForm;
    } else {
        qDebug() << "Incorrect Model Harvest Form: " << harvestForm;
    }
}


void ModelFormParameters::setPredationForm(QString predationForm) {

    if ((predationForm == "TYPE I") || (predationForm == "TYPE II") || (predationForm == "TYPE III")) {
        m_PredationForm = predationForm;
    } else {
        qDebug() << "Incorrect Model Predation Form: " << predationForm;
    }
}

void ModelFormParameters::setwgcForm(QString wgcForm) {

    if ((wgcForm == "MS-PROD") || (wgcForm == "NO_K") || (wgcForm == "NULL")) {
        m_wgcForm = wgcForm;
    } else {
        qDebug() << "Incorrect Model Within Guild Competition Form: " << wgcForm;
    }
}

void ModelFormParameters::setbgcForm(QString bgcForm) {

    if ((bgcForm == "MS-PROD") || (bgcForm == "NULL")) {
        m_bgcForm = bgcForm;
    } else {
        qDebug() << "Incorrect Model Within Guild Competition Form: " << bgcForm;
    }
}


void ModelFormParameters::setRunLength(double runLength) {

    m_RunLength = runLength;
}


void ModelFormParameters::setTimeStep(double timeStep) {

    m_TimeStep = timeStep;
}


QString ModelFormParameters::getGrowthForm() {

    return m_GrowthForm;
}

QString ModelFormParameters::getHarvestForm() {

    return m_HarvestForm;
}

QString ModelFormParameters::getPredationForm() {

    return m_PredationForm;
}

QString ModelFormParameters::getwgcForm() {

    return m_wgcForm;
}

QString ModelFormParameters::getbgcForm() {

    return m_bgcForm;
}

double ModelFormParameters::getRunLength() {

    return m_RunLength;
}

double ModelFormParameters::getTimeStep() {

    return m_TimeStep;
}
