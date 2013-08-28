#ifndef MODELFORMPARAMETERS_H
#define MODELFORMPARAMETERS_H

// Model Formulation Parameters

#include <QString>
#include <QDebug>


class ModelFormParameters {

private:
    QString m_GrowthForm;
    QString m_PredationForm;
    QString m_HarvestForm;
    QString m_wgcForm;              // Within Guild Competition
    QString m_bgcForm;              // Between Guild Competition
    double m_RunLength;
    double m_TimeStep;

public:
    ModelFormParameters();

    void setGrowthForm(QString);
    void setPredationForm(QString);
    void setHarvestForm(QString);
    void setwgcForm(QString);
    void setbgcForm(QString);
    void setRunLength(double);
    void setTimeStep(double)
;
    QString getGrowthForm();
    QString getPredationForm();
    QString getHarvestForm();
    QString getwgcForm();
    QString getbgcForm();
    double getRunLength();
    double getTimeStep();
};

#endif // MODELFORMPARAMETERS_H
