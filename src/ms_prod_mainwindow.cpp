#include "ms_prod_mainwindow.h"
#include "ui_ms_prod_mainwindow.h"
#include "MyQGLWidget.h"
#include "LineChart.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringList>
#include <QList>
#include <QMessageBox>
#include <QVariant>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

MS_PROD_MainWindow::MS_PROD_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MS_PROD_MainWindow)
{
    ui->setupUi(this);
    connect(ui->pb_Load, SIGNAL(clicked()), this, SLOT(pb_Load_Clicked()));
    connect(ui->pb_Run, SIGNAL(clicked()), this, SLOT(pb_Run_Clicked()));
    connect(ui->pb_fit, SIGNAL(clicked()), this, SLOT(pb_Fit_Clicked()));

    Parameters paramObj;
    TimeSeriesObservations tsObj;
    ModelFormParameters modelFormParamObj;
    MeasureOfFitness measureOfFitnessObj;
    QStringList lineList;
}


MS_PROD_MainWindow::~MS_PROD_MainWindow()
{
    //delete glWidget;
    delete ui;
}

void MS_PROD_MainWindow::closeEvent(QCloseEvent *event) {
    glWidget->close();
    //delete glWidget;
}

void MS_PROD_MainWindow::pb_Load_Clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open MS-PROD Parameter File"), "/", tr("CSV files (*.csv)"));

    loadFromParameterFile(fileName);

    QString fileName_TS = QFileDialog::getOpenFileName(this, tr("Open TimeSeries Parameter File"), "/", tr("CSV files (*.csv)"));
    loadFromTimeSeriesFile(fileName_TS);
 }


void MS_PROD_MainWindow::loadFromParameterFile(QString fileName) {
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream parameterFileStream(&file);
    while(!parameterFileStream.atEnd())
    {
        QString line;
        line = parameterFileStream.readLine();
        lineList = line.split(",");
        QString parameterName = lineList.at(0);
        // Convert parameter name to upper case to allow case insensitivity
        parameterName = parameterName.toUpper();
        parameterName = parameterName.trimmed();
        qDebug() << parameterName;
        if (parameterName == "SYSTEM CARRYING CAPACITY") {
            setSystemK();
        } else if (parameterName == "SPECIES") {
            setSpecies();
        } else if (parameterName == "SPECIES K") {
            setSpeciesK();
        } else if (parameterName == "SPECIES K MIN") {
            setSpeciesMinK();
        } else if (parameterName == "SPECIES K MAX") {
            setSpeciesMaxK();
        } else if (parameterName == "UNITS") {
            setUnits();
        } else if (parameterName == "GUILDS") {
            setGuilds();
        } else if (parameterName == "GUILD MEMBERSHIP") {
            setGuildMembership();
        } else if (parameterName == "GROWTH RATE") {
            setGrowthRates();
        } else if (parameterName == "GROWTH RATE MIN") {
            setGrowthRatesMin();
        } else if (parameterName == "GROWTH RATE MAX") {
            setGrowthRatesMax();
        } else if (parameterName == "HARVEST RATE") {
            setHarvestRates();
//        } else if (parameterName == "CATCHABILITY") {
//            setCatchabilities();
//        } else if (parameterName == "EFFORT") {
//            setEffort();
        } else if (parameterName == "CATCHABILITY") {
            QList<QList<double> > *catchabilityMatrix = new QList<QList<double> >;
            catchabilityMatrix = initializeTimeSeriesMatrix(parameterFileStream);
            paramObj.setCatchabilityMatrix(*catchabilityMatrix);
        } else if (parameterName == "EFFORT") {
            QList<QList<double> > *effortMatrix = new QList<QList<double> >;
            effortMatrix = initializeTimeSeriesMatrix(parameterFileStream);
            paramObj.setEffortMatrix(*effortMatrix);
        } else if (parameterName == "INITIAL BIOMASS") {
            setInitialBiomass();
        } else if (parameterName == "BETWEEN GUILD COMPETITION COEFFICIENT") {
            QList<QList<double> > *betweenGuildCompMatrix = new QList<QList<double> >;
            QString paramLine;
            QList<QString> guildList = paramObj.getGuildList();
            int numGuilds = guildList.size();
            for (int i = 0; i < numGuilds; i++) {
                paramLine = parameterFileStream.readLine();
//                qDebug() << i << " : " << paramLine;
                QStringList paramLineList = paramLine.split(",");
                paramLineList.removeFirst();
                double parameter = 0.0;
                QString strParameter = "";
                QList<double> *newList = new QList<double>;
                for (int i=0; i < numGuilds; i++) {
                    strParameter = paramLineList.at(i);
                    parameter = strParameter.toDouble();
                    newList->append(parameter);
                }
                betweenGuildCompMatrix->append(*newList);
            }
            paramObj.setBetweenGuildCompMatrix(*betweenGuildCompMatrix);

        } else if ((parameterName == "WITHIN GUILD COMPETITION COEFFICIENT") || (parameterName == "BETWEEN SPECIES COMPETITION COEFFICIENT") || (parameterName == "BETWEEN SPECIES INTERACTION COEFFICIENT")) {
            QList<QList<double> > *withinGuildCompMatrix = new QList<QList<double> >;
            withinGuildCompMatrix = initializeParamMatrix(parameterFileStream);
            paramObj.setWithinGuildCompMatrix(*withinGuildCompMatrix);

        } else if ((parameterName == "WITHIN GUILD COMPETITION COEFFICIENT MIN") || (parameterName == "BETWEEN SPECIES COMPETITION COEFFICIENT MIN") || (parameterName == "BETWEEN SPECIES INTERACTION COEFFICIENT MIN")) {
            QList<QList<double> > *withinGuildCompMinMatrix = new QList<QList<double> >;
            withinGuildCompMinMatrix = initializeParamMatrix(parameterFileStream);
            paramObj.setWithinGuildCompMinMatrix(*withinGuildCompMinMatrix);

        } else if ((parameterName == "WITHIN GUILD COMPETITION COEFFICIENT MAX") || (parameterName == "BETWEEN SPECIES COMPETITION COEFFICIENT MAX") || (parameterName == "BETWEEN SPECIES INTERACTION COEFFICIENT MAX")) {
            QList<QList<double> > *withinGuildCompMaxMatrix = new QList<QList<double> >;
            withinGuildCompMaxMatrix = initializeParamMatrix(parameterFileStream);
            paramObj.setWithinGuildCompMaxMatrix(*withinGuildCompMaxMatrix);

        } else if (parameterName == "PREDATION LOSS RATES") {
            QList<QList<double> > *predationMatrix = new QList<QList<double> >;
            predationMatrix = initializeParamMatrix(parameterFileStream);
            paramObj.setPredationMatrix(*predationMatrix);

        } else if (parameterName == "PREDATION LOSS RATES MIN") {
            QList<QList<double> > *predationLossRatesMinMatrix = new QList<QList<double> >;
            predationLossRatesMinMatrix = initializeParamMatrix(parameterFileStream);
            paramObj.setPredationLossRatesMinMatrix(*predationLossRatesMinMatrix);

        } else if (parameterName == "PREDATION LOSS RATES MAX") {
            QList<QList<double> > *predationLossRatesMaxMatrix = new QList<QList<double> >;
            predationLossRatesMaxMatrix = initializeParamMatrix(parameterFileStream);
            paramObj.setPredationLossRatesMaxMatrix(*predationLossRatesMaxMatrix);

        } else if (parameterName == "HANDLING TIMES") {
            QList<QList<double> > *handlingTimeMatrix = new QList<QList<double> >;
            handlingTimeMatrix = initializeParamMatrix(parameterFileStream);
            paramObj.setHandlingTimeMatrix(*handlingTimeMatrix);

        } else if (parameterName == "TYPE III B COEFFICIENTS") {
            QList<QList<double> > *bCoefMatrix = new QList<QList<double> >;
            bCoefMatrix = initializeParamMatrix(parameterFileStream);
            paramObj.setbCoefMatrix(*bCoefMatrix);

        } else if (parameterName == "SPATIAL OVERLAP") {
            QList<QList<double> > *spatialOverlapMatrix = new QList<QList<double> >;
            spatialOverlapMatrix = initializeParamMatrix(parameterFileStream);
            paramObj.setSpatialOverlapMatrix(*spatialOverlapMatrix);

        } else if (parameterName == "CATCH") {
            QList<QList<double> > *catchMatrix = new QList<QList<double> >;
            catchMatrix = initializeTimeSeriesMatrix(parameterFileStream);
            paramObj.setCatchMatrix(*catchMatrix);
        } else if (parameterName == "PELAGIC") {
            setPelProportionList();
        } else if (parameterName == "DEMERSAL") {
            setDemProportionList();
        } else if (parameterName == "GROWTH FORM") {
            setGrowthMode();
        } else if (parameterName == "PREDATION FORM") {
            setPredationMode();
        } else if (parameterName == "HARVEST FORM") {
            setHarvestMode();
        } else if (parameterName == "WITHIN GUILD COMPETITION FORM") {
            setwgcMode();
        } else if (parameterName == "BETWEEN GUILD COMPETITION FORM") {
            setbgcMode();
        } else if (parameterName == "RUN LENGTH") {
            setRunLength();
        } else if (parameterName == "TIME STEP") {
            setTimeStep();
        } else if (parameterName == "GA GENERATIONS") {
            set_GA_generations();
        } else if (parameterName == "GA POPULATION SIZE") {
            set_GA_populationSize();
        } else if (parameterName == "GA MUTATION RATE") {
            set_GA_mutationRate();
        } else if (parameterName != "") {
            qDebug() << "Missing Parameter Value: " << parameterName;
        }
    }
    file.close();
    paramObj.setLoadedTrue();
    glWidget->initializeSliders();

    // Log outputs to screen
    MS_PROD_MainWindow::logResults();
}


void MS_PROD_MainWindow::loadFromTimeSeriesFile(QString fileNameTS) {
    QFile file(fileNameTS);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream tsFileStream(&file);
    int numSpecies = paramObj.getNumberOfSpecies();

    while(!tsFileStream.atEnd())
    {
        QString tsLine;
        tsLine = tsFileStream.readLine();
        lineList = tsLine.split(",");
        QString tsName = lineList.at(0);
        // Convert parameter name to upper case to allow case insensitivity
        tsName = tsName.toUpper();
        tsName = tsName.trimmed();
        if (tsName == "BIOMASS") {
            for (int i=0; i < numSpecies; i++) {
                tsLine = tsFileStream.readLine();
                QStringList tsLineList = tsLine.split(",");
                tsObj.setBiomass(tsLineList);
            }
        } else if (tsName == "CATCH") {
            for (int i=0; i < numSpecies; i++) {
                tsLine = tsFileStream.readLine();
                QStringList tsLineList = tsLine.split(",");
                tsObj.setCatch(tsLineList);
            }
        } else if (tsName == "BIOMASS MULTIPLIER") {
            for (int i=0; i < numSpecies; i++) {
                tsLine = tsFileStream.readLine();
                QStringList tsLineList = tsLine.split(",");
                tsObj.setBiomassMultiplier(tsLineList);
            }
        } else {
        qDebug() << "Missing Parameter Value: " << tsName;
        }
    }
    file.close();

    tsObj.outputBiomass();
    tsObj.outputCatch();
    tsObj.outputBiomassMultiplier();
}


QList<QList<double> >* MS_PROD_MainWindow::initializeParamMatrix(QTextStream & paramFileStream) {

    QList<QList<double> > *paramMatrix = new QList<QList<double> >;
    QString paramLine;
    int numSpecies = paramObj.getNumberOfSpecies();
    for (int i = 0; i < numSpecies; i++) {
        paramLine = paramFileStream.readLine();
//        qDebug() << i << " : " << paramLine;
        QStringList paramLineList = paramLine.split(",");
        paramLineList.removeFirst();
        paramLineList.removeAll("");
        double parameter = 0.0;
        QString strParameter = "";
        QList<double> *newList = new QList<double>;
        for (int i=0; i < numSpecies; i++) {
            strParameter = paramLineList.at(i);
            parameter = strParameter.toDouble();
            newList->append(parameter);
        }
        paramMatrix->append(*newList);
    }
    return paramMatrix;
}


QList<QList<double> >* MS_PROD_MainWindow::initializeTimeSeriesMatrix(QTextStream & paramFileStream) {

    QList<QList<double> > *paramMatrix = new QList<QList<double> >;
    QString paramLine;
    int numSpecies = paramObj.getNumberOfSpecies();
    for (int i = 0; i < numSpecies; i++) {
        paramLine = paramFileStream.readLine();
        qDebug() << i << " : " << paramLine;
        QStringList paramLineList = paramLine.split(",");
        paramLineList.removeFirst();
        double parameter = 0.0;
        QString strParameter = "";
        QList<double> *newList = new QList<double>;
        int catchVectorSize = paramLineList.size();
        for (int i=0; i < catchVectorSize; i++) {
            strParameter = paramLineList.at(i);
            parameter = strParameter.toDouble();
            newList->append(parameter);
        }
        paramMatrix->append(*newList);
    }
    return paramMatrix;
}


void MS_PROD_MainWindow::logResults() {

    // Print various parameters to screen after parameter file created
//    QStringList strSpeciesList = paramObj.getSpeciesList();
//    qDebug() << strSpeciesList;

//    paramObj.printGrowthRateList();

  //  paramObj.printInitialBiomassList();

//    paramObj.printWithinGuildCompMatrix();

    //paramObj.printBetweenGuildCompMatrix();

    //paramObj.printPredationMatrix();

    //paramObj.printSpatialOverlapMatrix();

    //paramObj.printPelagicProportionList();

    //paramObj.printDemersalProportionList();

//    paramObj.printGuildList();

//    paramObj.printSpeciesGuildMembershipHash();

//    paramObj.printSpeciesKList();

}


void MS_PROD_MainWindow::setSystemK() {

    QString strSystemK = lineList.at(1);
    double systemK = strSystemK.toDouble();
    paramObj.setSystemK(systemK);
    double sysK = paramObj.getSystemK();
    QString sSysK = convertDoubleToString(sysK);
    ui->leSystemK->setText(sSysK);
}


void MS_PROD_MainWindow::setUnits() {

    QString strUnits = lineList.at(1);
    paramObj.setBiomassUnits(strUnits);
    ui->leUnits->setText(strUnits);
}


QList<double> MS_PROD_MainWindow::returnParameterList() {

    lineList.removeFirst();
    lineList.removeAll("");
    int lineListSize = lineList.size();
    double parameter = 0.0;
    QList<double> parameterList;
    for (int i = 0; i < lineListSize; i++) {
        parameter = lineList.at(i).toDouble();
        parameterList.append(parameter);
    }
    return parameterList;
}


void MS_PROD_MainWindow::setSpecies() {

    lineList.removeFirst();
    lineList.removeAll("");
    paramObj.setSpeciesList(lineList);
}


void MS_PROD_MainWindow::setSpeciesK() {

    QList<double> speciesKList = returnParameterList();
    paramObj.setSpeciesKList(speciesKList);
}


void MS_PROD_MainWindow::setSpeciesMinK() {

    QList<double> speciesMinKList = returnParameterList();
    paramObj.setSpeciesK_MinList(speciesMinKList);
}


void MS_PROD_MainWindow::setSpeciesMaxK() {

    QList<double> speciesMaxKList = returnParameterList();
    paramObj.setSpeciesK_MaxList(speciesMaxKList);
}


void MS_PROD_MainWindow::setHarvestRates() {

    QList<double> harvestRateList = returnParameterList();
    paramObj.setHarvestRateList(harvestRateList);
}

void MS_PROD_MainWindow::setGrowthRates() {

    QList<double> growthRateList = returnParameterList();
    paramObj.setGrowthRates(growthRateList);
}


void MS_PROD_MainWindow::setGrowthRatesMin() {

    QList<double> growthRateMinList = returnParameterList();
    paramObj.setGrowthRatesMin(growthRateMinList);
}


void MS_PROD_MainWindow::setGrowthRatesMax() {

    QList<double> growthRateMaxList = returnParameterList();
    paramObj.setGrowthRatesMax(growthRateMaxList);
}


void MS_PROD_MainWindow::setCatchabilities() {

    QList<double> catchabilitiesList = returnParameterList();
    paramObj.setCatchabilitiesList(catchabilitiesList);
}


void MS_PROD_MainWindow::setEffort() {

    QList<double> effortList = returnParameterList();
    paramObj.setEffortList(effortList);
}


void MS_PROD_MainWindow::setRunLength() {

    QString strRunLength = lineList.at(1);
    double runLength = strRunLength.toDouble();
    modelFormParamObj.setRunLength(runLength);
}

void MS_PROD_MainWindow::setTimeStep() {

    QString strTimeStep = lineList.at(1);
    double timeStep = strTimeStep.toDouble();
    modelFormParamObj.setTimeStep(timeStep);
}


void MS_PROD_MainWindow::set_GA_generations() {

    QString strGenerations = lineList.at(1);
    int generations = strGenerations.toInt();
    paramObj.set_GA_generations(generations);
}


void MS_PROD_MainWindow::set_GA_populationSize() {

    QString strPopSize = lineList.at(1);
    int populationSize = strPopSize.toInt();
    paramObj.set_GA_populationSize(populationSize);
}


void MS_PROD_MainWindow::set_GA_mutationRate() {

    QString strMutRate = lineList.at(1);
    double mutationRate = strMutRate.toDouble();
    paramObj.set_GA_mutationRate(mutationRate);
}


void MS_PROD_MainWindow::setInitialBiomass() {

    QList<double> initialBiomassList = returnParameterList();
    paramObj.setInitialBiomassList(initialBiomassList);

}


void MS_PROD_MainWindow::setPelProportionList() {

    QList<double> pelProportionList = returnParameterList();
    paramObj.setPelagicProportionList(pelProportionList);
}

void MS_PROD_MainWindow::setDemProportionList() {

    QList<double> demProportionList = returnParameterList();
    paramObj.setDemersalProportionList(demProportionList);
}


void MS_PROD_MainWindow::setGuilds() {

    lineList.removeFirst();
    // Remove blank values
    int lineListSize = lineList.size();
    for (int i = lineListSize - 1; i >= 0; i--) {
        if (lineList.at(i) == "") {
            lineList.removeAt(i);
        }
    }
    paramObj.setGuildList(lineList);
}


void MS_PROD_MainWindow::setGuildMembership() {

    lineList.removeFirst();
    int lineListSize = lineList.size();
    for (int i = lineListSize - 1; i >= 0; i--) {
        if (lineList.at(i) == "") {
            lineList.removeAt(i);
        }
    }
    paramObj.setSpeciesGuildMembershipHash(lineList);
}

void MS_PROD_MainWindow::setGrowthMode() {

    QString value = lineList.at(1);
    value = value.toUpper();
    value = value.trimmed();

    modelFormParamObj.setGrowthForm(value);
}

void MS_PROD_MainWindow::setHarvestMode() {

    QString value = lineList.at(1);
    value = value.toUpper();
    value = value.trimmed();

    modelFormParamObj.setHarvestForm(value);

    if (value == "H") {
        ui->rb_Expl_Form_F->setChecked(1);
    } else if (value == "QE") {
        ui->rb_Expl_Form_qE->setChecked(1);
    } else if (value == "CATCH") {

    }
}

void MS_PROD_MainWindow::setPredationMode() {

    QString value = lineList.at(1);
    value = value.toUpper();
    value = value.trimmed();

    modelFormParamObj.setPredationForm(value);
//    qDebug() << value << "Predation";
    if (value == "TYPE I") {
        ui->rb_Pred_Form_Type1->setChecked(1);
    } else if (value == "TYPE II") {
      ui->rb_Pred_Form_Type2->setChecked(1);
    } else if (value == "TYPE III") {
        ui->rb_Pred_Form_Type3->setChecked(1);
    }
}

void MS_PROD_MainWindow::setwgcMode() {

    QString value = lineList.at(1);
    value = value.toUpper();
    value = value.trimmed();

    modelFormParamObj.setwgcForm(value);
    if (value == "NULL") {
        ui->cb_ModelOpts_SC->setCheckState(Qt::Unchecked);
    } else {
        ui->cb_ModelOpts_SC->setCheckState(Qt::Checked);
    }
}

void MS_PROD_MainWindow::setbgcMode() {

    QString value = lineList.at(1);
    value = value.toUpper();
    value = value.trimmed();

    modelFormParamObj.setbgcForm(value);
    if (value == "NULL") {
        ui->cb_modelOpts_GC->setCheckState(Qt::Unchecked);
    } else {
        ui->cb_modelOpts_GC->setCheckState(Qt::Checked);
    }
}


QString MS_PROD_MainWindow::convertDoubleToString(double number) {
    QVariant newVariant(number);
    QString newString = newVariant.toString();
    return newString;
}

void MS_PROD_MainWindow::pb_Run_Clicked() {
    testFramework();
    if (!paramObj.loaded()) {
        pb_Load_Clicked();
    }
    runModel();

}

void MS_PROD_MainWindow::runModel(bool updateCharts) {

    Parameters* ptr_paramObj = &paramObj;
    ModelFormParameters* ptr_modelParamObj = &modelFormParamObj;
    TimeSeriesObservations* ptr_tsObj = &tsObj;
    //QString saveFilename = QFileDialog::getSaveFileName(this, QObject::tr("Save as..."), QObject::tr(""), QObject::tr("CSV (*.csv)"));
    ptr_paramObj->setSaveFilename("out.csv");//(saveFilename);
    Model model(ptr_paramObj, ptr_modelParamObj);
    model.runModel();
    //QMessageBox::information(this, tr("Kraken 1.1"), tr("Run Completed"));

    if (updateCharts) {
        glWidget->updateCharts(&model);
    }
}


void MS_PROD_MainWindow::pb_Fit_Clicked() {

    QString estimationType;
    if (!paramObj.loaded()) {
        pb_Load_Clicked();
    }
    Parameters* ptr_paramObj = &paramObj;
    TimeSeriesObservations* ptr_tsObj = &tsObj;
    ModelFormParameters* ptr_modelParamObj = &modelFormParamObj;
    if (ui->rb_Fitting_GA->isChecked()) {
        estimationType = "GENETIC ALGORITHM";
        ui->textBrowser->setText(estimationType);
        ptr_paramObj->initializeChromosomeLists();

        QString saveFilename = QFileDialog::getSaveFileName(this, QObject::tr("Save as... (Full Output)"), QObject::tr(""), QObject::tr("CSV (*.csv)"));
        QString saveFilenameBest = QFileDialog::getSaveFileName(this, QObject::tr("Save as... (Best Parameters"), QObject::tr(""), QObject::tr("CSV (*.csv)"));
        GA_Estimator o_GA_Estimator(ptr_paramObj, ptr_tsObj, ptr_modelParamObj);
        o_GA_Estimator.setSaveFilenames(saveFilename, saveFilenameBest);
        o_GA_Estimator.estimateParameters();
    } else if (ui->rb_Fitting_LSE->isChecked()) {
        estimationType = "Least Squares";
    } else if (ui->rb_Fitting_ML->isChecked()) {
        estimationType = "Maximum Likelihood";
    }
}


void MS_PROD_MainWindow::testFramework() {

//    testLooping();
}



void MS_PROD_MainWindow::testLooping() {

    // Test code for batching range of harvest rates/efforts/etc - will be completely redone for
    // final version

    QList<QString> groupList;
    QList<double> minValList;
    QList<double> maxValList;
    QList<double> stepList;
    QList<double> harvestRateList;
    groupList.append("A");
    groupList.append("B");
    groupList.append("C");
    minValList.append(0.3);
    minValList.append(0.5);
    minValList.append(0.2);
    maxValList.append(0.3);
    maxValList.append(0.5);
    maxValList.append(0.2);
    stepList.append(0.1);
    stepList.append(0.1);
    stepList.append(0.1);
    harvestRateList.append(0.3);
    harvestRateList.append(0.5);
    harvestRateList.append(0.2);

    QFile testFile("C:\\Documents and Settings\\rgamble\\Desktop\\MS-PROD_QT\\MS-PROD_QT\\testHarvestRates.prm");
    if (!testFile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append )) {
        qDebug() << "Failed to open file.";
    }

    QTextStream out(&testFile);

    int numGroups = groupList.size();
    int index = numGroups-1;

    // Set up first group
    double rateStep = stepList.at(index);
    for (double curRate = minValList.at(index); curRate <= maxValList.at(index); curRate += rateStep) {
        out << curRate << endl;
    }
    testFile.close();

    index--;
    //    int initIndex = index;
    while (index >= 0) {
        qDebug() << index;
        QString filename1 = "C:\\Documents and Settings\\rgamble\\Desktop\\MS-PROD_QT\\MS-PROD_QT\\testHarvestRates.prm";
        QFile testFile2(filename1);
        if (!testFile2.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Failed to open testfile.";
        }
        QString tempFilename = "C:\\Documents and Settings\\rgamble\\Desktop\\MS-PROD_QT\\MS-PROD_QT\\tempHarvestRates.prm";
        QFile tempFile(tempFilename);
        if (!tempFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
            qDebug() << "Failed to open tempfile.";
        }
        QTextStream in(&testFile2);
        QTextStream out2(&tempFile);
        QString line;
        while (!in.atEnd()) {
            line = in.readLine();
            int rateStepInt = stepList.at(index) * 100;
            int maxValInt = maxValList.at(index) * 100;

            for (int curRateInt = minValList.at(index) * 100; curRateInt <= maxValList.at(index) * 100; curRateInt += rateStepInt) {
                 out2 << curRateInt / 100.0 << "," << line << endl;
            }
        }
        index--;
        testFile2.remove();
        testFile2.close();

        if (!tempFile.rename(filename1)) {
            qDebug() << "Failed to rename...";
        }
        tempFile.close();
    }
}
