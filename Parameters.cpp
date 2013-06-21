#include "Parameters.h"
#include "ModelComponent.h"
#include "LineGraph.h"
#include "MyQGLWidget.h"

Parameters::Parameters() {
    m_BiomassUnits = "mt";
    m_SystemK = 0;
    m_loaded = false;
    QList<QList<double> >* initPredMatrix = new QList<QList<double> >;
    QList<double>* initSpeciesKList = new QList<double>;
    setPredationMatrix(*initPredMatrix);
    m_SpeciesKList = *initSpeciesKList;
}


bool Parameters::loaded() {

    return m_loaded;
}


void Parameters::setLoadedTrue() {

    m_loaded = true;
}


QString Parameters::getBiomassUnits() {
    return m_BiomassUnits;
}


void Parameters::setBiomassUnits(QString value) {
    m_BiomassUnits = value;
}


double Parameters::getSystemK() {
    return m_SystemK;
}


void Parameters::setSystemK(double value) {
    m_SystemK = value;
}


int Parameters::getNumberOfSpecies() {

    return m_SpeciesList.size();
}


QStringList Parameters::getSpeciesList() {
    return m_SpeciesList;
}


void Parameters::setSpeciesList(QStringList list) {
    m_SpeciesList = list;
}


QString Parameters::getSpeciesName(int i) {

    return m_SpeciesList.at(i);
}


QList<double> Parameters::getSpeciesKList() {
    return m_SpeciesKList;
}


void Parameters::setSpeciesKList(QList<double> list) {
    m_SpeciesKList = list;
}


void Parameters::printSpeciesKList() {
    double value;
//     qDebug() << "Species K: ";
    foreach (value, m_SpeciesKList) {
//         qDebug() << value;
    }
}


QList<double> Parameters::getSpeciesK_MinList() {
    return m_SpeciesK_MinList;
}


void Parameters::setSpeciesK_MinList(QList<double> list) {
    m_SpeciesK_MinList = list;
}


void Parameters::printSpeciesK_MinList() {
    double value;
//     qDebug() << "Species K (Min): ";
    foreach (value, m_SpeciesK_MinList) {
//         qDebug() << value;
    }
}


QList<double> Parameters::getSpeciesK_MaxList() {
    return m_SpeciesK_MaxList;
}


void Parameters::setSpeciesK_MaxList(QList<double> list) {
    m_SpeciesK_MaxList = list;
}


void Parameters::printSpeciesK_MaxList() {
    double value;
//     qDebug() << "Species K (Max): ";
    foreach (value, m_SpeciesK_MaxList) {
//         qDebug() << value;
    }
}


QList<double> Parameters::getChromosomeList() {

    initializeChromosomeLists();

    QList<double> chromosomeList = m_ChromosomeList;
    return chromosomeList;
}


double Parameters::getChromosomeMin(int i) {

    double min = m_ChromosomeMinList.at(i);
    return min;
}


double Parameters::getChromosomeMax(int i) {

    double max = m_ChromosomeMaxList.at(i);
    return max;
}


void Parameters::initializeChromosomeLists() {

    initializeChromosomeList();
    initializeChromosomeMinList();
    initializeChromosomeMaxList();
}


void Parameters::initializeChromosomeList() {

    QList<double> chromosomeList = getGrowthRateList();
    QList<double> speciesK_List = getSpeciesKList();

    double value;

    int speciesK_ListSize = speciesK_List.size();

    if (speciesK_ListSize > 0) {
        for (int i = 0; i < speciesK_ListSize; i++) {
            value = speciesK_List.at(i);
            chromosomeList.append(value);
        }
    }

    int x, y = 0;
    Int_Matrix withinGuildCompetitionList = getWithinGuildCompMatrix();
    x = withinGuildCompetitionList.size();
    if (x > 0) {
        y = withinGuildCompetitionList.at(0).size();
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                value = withinGuildCompetitionList.at(i).at(j);
                chromosomeList.append(value);
            }
        }
    }

    Int_Matrix predationLossesList = getPredationMatrix();
    x = predationLossesList.size();
    if (x > 0) {
        y = predationLossesList.at(0).size();
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                value = predationLossesList.at(i).at(j);
                chromosomeList.append(value);
            }
        }
    }
    m_ChromosomeList = chromosomeList;
}


void Parameters::initializeChromosomeMinList() {

    QList<double> chromosomeMinList = getGrowthRateMinList();
    QList<double> speciesK_MinList = getSpeciesK_MinList();

    double value;

    int speciesK_MinListSize = speciesK_MinList.size();
    if (speciesK_MinListSize > 0) {
        for (int i = 0; i < speciesK_MinListSize; i++) {
           value = speciesK_MinList.at(i);
            chromosomeMinList.append(value);
        }
    }

    int x, y = 0;

    Int_Matrix withinGuildCompetitionMinList = getWithinGuildCompMinMatrix();
    x = withinGuildCompetitionMinList.size();

    if (x > 0) {
        y = withinGuildCompetitionMinList.at(0).size();
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                value = withinGuildCompetitionMinList.at(i).at(j);
                chromosomeMinList.append(value);
            }
        }
    }

    Int_Matrix predationLossRatesMinList = getPredationLossRatesMinMatrix();
    x = predationLossRatesMinList.size();

    if (x > 0) {
        y = predationLossRatesMinList.at(0).size();
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                value = predationLossRatesMinList.at(i).at(j);
                chromosomeMinList.append(value);
            }
        }
    }

    m_ChromosomeMinList = chromosomeMinList;
}


void Parameters::initializeChromosomeMaxList() {

    QList<double> chromosomeMaxList = getGrowthRateMaxList();
    QList<double> speciesK_MaxList = getSpeciesK_MaxList();

    double value;

    int speciesK_MaxListSize = speciesK_MaxList.size();

    if (speciesK_MaxListSize > 0) {
        for (int i = 0; i < speciesK_MaxListSize; i++) {
            value = speciesK_MaxList.at(i);
            chromosomeMaxList.append(value);
        }
    }

    int x, y = 0;

    Int_Matrix withinGuildCompetitionMaxList = getWithinGuildCompMaxMatrix();
    x = withinGuildCompetitionMaxList.size();

    if (x > 0) {
        y = withinGuildCompetitionMaxList.at(0).size();
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                value = withinGuildCompetitionMaxList.at(i).at(j);
                chromosomeMaxList.append(value);
            }
        }
    }

    Int_Matrix predationLossRatesMaxList = getPredationLossRatesMaxMatrix();
    x = predationLossRatesMaxList.size();

    if (x > 0) {
        y = predationLossRatesMaxList.at(0).size();
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                value = predationLossRatesMaxList.at(i).at(j);
                chromosomeMaxList.append(value);
            }
        }
    }

    m_ChromosomeMaxList = chromosomeMaxList;
}


void Parameters::setChromosomeList(QList<double> chromosomeList) {

    int index = 0;
    int growthRateListSize = m_GrowthRateList.size();
    m_GrowthRateList.clear();
    double paramValue;
    for (int i = index; i < growthRateListSize; i++) {
        paramValue = chromosomeList.at(index);
        m_GrowthRateList.append(paramValue);
        index++;
    }

    int speciesK_ListSize = m_SpeciesKList.size();

    if (speciesK_ListSize > 0) {
        m_SpeciesKList.clear();
        for (int i = 0; i < speciesK_ListSize; i++) {
            paramValue = chromosomeList.at(index);
            m_SpeciesKList.append(paramValue);
            index++;
        }
    }

    int x, y = 0;

    Int_Matrix withinGuildCompetitionMatrix = getWithinGuildCompMatrix();
    x = withinGuildCompetitionMatrix.size();

    if (x > 0) {
        y = withinGuildCompetitionMatrix.at(0).size();

        m_WithinGuildCompMatrix.clearMatrix();
        Int_Matrix *wgcParamMatrix = new Int_Matrix;

        for (int i = 0; i < x; i++) {
            QList<double> *newList = new QList<double>;
            for (int j = 0; j < y; j++) {
//              qDebug() << "i = " << i << " : j = " << j;
                paramValue = chromosomeList.at(index);
//              qDebug() << paramValue;
                newList->append(paramValue);
                index++;
            }
            wgcParamMatrix->append(*newList);
        }
        setWithinGuildCompMatrix(*wgcParamMatrix);
    }

    Int_Matrix predationMatrix = getPredationMatrix();
    x = predationMatrix.size();

    if (x > 0) {
        y = predationMatrix.at(0).size();

        m_PredationLossesMatrix.clearMatrix();
        Int_Matrix *predParamMatrix = new Int_Matrix;

        for (int i = 0; i < x; i++) {
            QList<double> *newList = new QList<double>;
            for (int j = 0; j < y; j++) {
//                qDebug() << "i = " << i << " : j = " << j;
                paramValue = chromosomeList.at(index);
//              qDebug() << paramValue;
                newList->append(paramValue);
                index++;
            }
            predParamMatrix->append(*newList);
        }
        setPredationMatrix(*predParamMatrix);
    }

}


int Parameters::getChromosomeListSize() {

    return m_ChromosomeList.size();
}


void Parameters::setGuildList(QStringList list) {

    m_GuildList = list;

    // Initialize guild biomasses to 0.0
    int Bg = 0.0;
    QString guildName = "";
    int numberOfGuilds = getNumberOfGuilds();

    for (int g = 0; g < numberOfGuilds; g++) {
        guildName = m_GuildList.at(g);
        m_GuildBiomassHash.insert(guildName, Bg);
    }
}


double Parameters::getGuildBiomass(QString guildName) {

    return m_GuildBiomassHash.value(guildName);
}


QStringList Parameters::getGuildList() {
    return m_GuildList;
}


void Parameters::printGuildList() {

//     qDebug() << m_GuildList;
}


QString Parameters::getGuildName(int g) {

    return m_GuildList.at(g);
}


int Parameters::getNumberOfGuilds() {

    return m_GuildList.size();
}


QHash<QString, QString> Parameters::getSpeciesGuildMembershipHash() {
    return m_SpeciesGuildMembershipHash;
}


void Parameters::setSpeciesGuildMembershipHash(QStringList guildList) {

    int numSpecies = m_SpeciesList.size();
    for (int i = 0; i < numSpecies; i++) {
        m_SpeciesGuildMembershipHash.insert(m_SpeciesList.at(i), guildList.at(i));
    }
}


QString Parameters::getGuildMembership(QString speciesName) {

    return m_SpeciesGuildMembershipHash.value(speciesName);
}


void Parameters::printSpeciesGuildMembershipHash() {

//    QList<QString> speciesList = m_SpeciesGuildMembershipHash.uniqueKeys();
    int numSpecies = m_SpeciesList.size();
    QString speciesName = "";
    for (int i = 0; i < numSpecies; i++) {
        speciesName = m_SpeciesList.at(i);
        // qDebug() << speciesName << " : " << m_SpeciesGuildMembershipHash.value(speciesName);
    }
}


void Parameters::setSaveFilename(QString filename) {

    m_OutputFilename = filename;
}


QList<QList<double> > Parameters::getBiomassMatrix() {
    return m_BiomassMatrix.getMatrix();
}


void Parameters::setBiomassMatrix(QList<QList<double> > matrix) {
    m_BiomassMatrix.setMatrix(matrix);
}


void Parameters::clearBiomassMatrix() {
    QList<QList<double> > *biomassMatrix = new QList<QList<double> >;
    int speciesIndex;
    double value;
    foreach (value, m_InitialBiomassList) {
        QList<double> *newList = new QList<double>;
        newList->append(value);
        biomassMatrix->append(*newList);
        speciesIndex++;
    }
    setBiomassMatrix(*biomassMatrix);
}


void Parameters::printBiomassMatrix() {

    m_BiomassMatrix.printMatrix(m_SpeciesList);

}


double Parameters::getBiomass(const int speciesIndex, const int time) {

    return m_BiomassMatrix.getMatrixValue(speciesIndex, time);
}


double Parameters::getBiomass(const QString speciesName, const int time) {

    int speciesListSize = m_SpeciesList.size();
    for (int i = 0; i < speciesListSize; i++) {
        QString nameFromSpeciesList = getSpeciesName(i);
        if (speciesName == nameFromSpeciesList) {
//            qDebug() << "Species Name: " << speciesName << " ; Index: " << i;
            return m_BiomassMatrix.getMatrixValue(i,time);
        }
    }
}


void Parameters::outputResults(const QList<ModelComponent *> modelComponentList) {

    QFile outFile(m_OutputFilename);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
         qDebug() << "Output file NOT OPENED";
    QTextStream outputStream(&outFile);

    outputStream << "Time Step,";
    QString species;

    foreach (species, m_SpeciesList) {
        outputStream << species << " (Biomass),";
    }
    outputStream << "\n";
    QList<QList<double> > outputBiomassMatrix = m_BiomassMatrix.getMatrix();
    int x = m_SpeciesList.size();
    int y = outputBiomassMatrix.at(0).size();
    for (int i = 0; i < y; i++) {
        outputStream << i << ",";
        for (int j = 0; j < x; j++) {
            outputStream << outputBiomassMatrix.at(j).at(i) << ",";
        }
        outputStream << "\n";
    }

    outputStream << "\n";

    ModelComponent* component;
    QList<QString> speciesList = m_SpeciesList;
    foreach (component, modelComponentList) {
        component->outputResults(outputStream, speciesList);
    }

    LineGraph *g = LineGraph::createGraph(outputBiomassMatrix);
    MyQGLWidget::setGraph(g);
}


void Parameters::updateBiomass(int speciesIndex, int timeStep, double dN) {

    double prevBiomass = m_BiomassMatrix.getMatrixValue(speciesIndex, timeStep);
    double newBiomass = prevBiomass + dN;
    if (newBiomass < 0) {
        newBiomass = 0;
    }
//    qDebug() << "Prev Biomass: " << prevBiomass << "    dN: " << dN << "    New Biomass: " << newBiomass;
    m_BiomassMatrix.addMatrixValue(speciesIndex, newBiomass);
}



QList<QList<double> > Parameters::getWithinGuildCompMatrix() {
    return m_WithinGuildCompMatrix.getMatrix();
}


void Parameters::setWithinGuildCompMatrix(QList<QList<double> > matrix) {
    m_WithinGuildCompMatrix.setMatrix(matrix);
}


void Parameters::printWithinGuildCompMatrix() {

    m_WithinGuildCompMatrix.printMatrix(m_SpeciesList);

}


double Parameters::getWithinGuildCompParam(int i, int j) {

    return m_WithinGuildCompMatrix.getMatrixValue(i,j);
}


QList<QList<double> > Parameters::getWithinGuildCompMinMatrix() {
    return m_WithinGuildCompMinMatrix.getMatrix();
}


void Parameters::setWithinGuildCompMinMatrix(QList<QList<double> > matrix) {
    m_WithinGuildCompMinMatrix.setMatrix(matrix);
}


void Parameters::printWithinGuildCompMinMatrix() {

    m_WithinGuildCompMinMatrix.printMatrix(m_SpeciesList);

}


QList<QList<double> > Parameters::getWithinGuildCompMaxMatrix() {
    return m_WithinGuildCompMaxMatrix.getMatrix();
}


void Parameters::setWithinGuildCompMaxMatrix(QList<QList<double> > matrix) {
    m_WithinGuildCompMaxMatrix.setMatrix(matrix);
}


void Parameters::printWithinGuildCompMaxMatrix() {

    m_WithinGuildCompMaxMatrix.printMatrix(m_SpeciesList);

}


QList<QList<double> > Parameters::getPredationLossRatesMinMatrix() {
    return m_PredationLossRatesMinMatrix.getMatrix();
}


void Parameters::setPredationLossRatesMinMatrix(QList<QList<double> > matrix) {
    m_PredationLossRatesMinMatrix.setMatrix(matrix);
}


void Parameters::printPredationLossRatesMinMatrix() {

    m_PredationLossRatesMinMatrix.printMatrix(m_SpeciesList);
}


QList<QList<double> > Parameters::getPredationLossRatesMaxMatrix() {
    return m_PredationLossRatesMaxMatrix.getMatrix();
}


void Parameters::setPredationLossRatesMaxMatrix(QList<QList<double> > matrix) {
    m_PredationLossRatesMaxMatrix.setMatrix(matrix);
}


void Parameters::printPredationLossRatesMaxMatrix() {

    m_PredationLossRatesMaxMatrix.printMatrix(m_SpeciesList);
}


QList<QList<double> > Parameters::getBetweenGuildCompMatrix() {
    return m_BetweenGuildCompMatrix.getMatrix();
}


void Parameters::setBetweenGuildCompMatrix(QList<QList<double> > matrix) {
    m_BetweenGuildCompMatrix.setMatrix(matrix);
}


void Parameters::printBetweenGuildCompMatrix() {

    m_BetweenGuildCompMatrix.printMatrix(m_GuildList);

}


double Parameters::getBetweenGuildCompParam(int i, int j) {

    return m_BetweenGuildCompMatrix.getMatrixValue(i,j);
}


QList<QList<double> > Parameters::getPredationMatrix() {
    return m_PredationMatrix.getMatrix();
}


void Parameters::setPredationMatrix(QList<QList<double> > matrix) {
    m_PredationMatrix.setMatrix(matrix);
}


void Parameters::printPredationMatrix() {

    m_PredationMatrix.printMatrix(m_SpeciesList);

}


double Parameters::getAvailability(int i, int j) {

    return m_PredationMatrix.getMatrixValue(i,j);
}


QList<QList<double> > Parameters::getHandlingTimeMatrix() {
    return m_HandlingTimeMatrix.getMatrix();
}


void Parameters::setHandlingTimeMatrix(QList<QList<double> > matrix) {
    m_HandlingTimeMatrix.setMatrix(matrix);
}


void Parameters::printHandlingTimeMatrix() {

    m_HandlingTimeMatrix.printMatrix(m_SpeciesList);

}


double Parameters::getHandlingTime(int i, int j) {

    return m_HandlingTimeMatrix.getMatrixValue(i,j);
}


QList<QList<double> > Parameters::getbCoefMatrix() {
    return m_bCoefMatrix.getMatrix();
}


void Parameters::setbCoefMatrix(QList<QList<double> > matrix) {
    m_bCoefMatrix.setMatrix(matrix);
}


void Parameters::printbCoefMatrix() {

    m_bCoefMatrix.printMatrix(m_SpeciesList);

}


double Parameters::getbCoef(int i, int j) {

    return m_bCoefMatrix.getMatrixValue(i,j);
}


QList<QList<double> > Parameters::getSpatialOverlapMatrix() {
    return m_SpatialOverlapMatrix.getMatrix();
}


void Parameters::setSpatialOverlapMatrix(QList<QList<double> > matrix) {
    m_SpatialOverlapMatrix.setMatrix(matrix);
}


void Parameters::printSpatialOverlapMatrix() {


    m_SpatialOverlapMatrix.printMatrix(m_SpeciesList);

}


double Parameters::getSpatialOverlap(int i, int j) {

    return m_SpatialOverlapMatrix.getMatrixValue(i,j);
}


void Parameters::setGrowthRates(QList<double> list) {

    m_GrowthRateList = list;
}


double Parameters::getGrowthRate(const int speciesIndex) {

    return m_GrowthRateList.at(speciesIndex);
}


QList<double> Parameters::getGrowthRateList() {

    return m_GrowthRateList;
}


void Parameters::setGrowthRatesMin(QList<double> list) {

    m_GrowthRateMinList = list;
}


QList<double> Parameters::getGrowthRateMinList() {

    return m_GrowthRateMinList;
}


void Parameters::setGrowthRatesMax(QList<double> list) {

    m_GrowthRateMaxList = list;
}


QList<double> Parameters::getGrowthRateMaxList() {

    return m_GrowthRateMaxList;
}


void Parameters::setHarvestRateList(QList<double> list) {

    m_HarvestRateList = list;
}


QList<double> Parameters::getHarvestRateList() {
    return m_HarvestRateList;
}


void Parameters::printHarvestRateList() {

    int x = m_HarvestRateList.size();
    // qDebug() << "Harvest Rates:";
    for (int i = 0; i < x; i++){
        // qDebug() << m_HarvestRateList.at(i);
    }
}


double Parameters::getHarvestRate(const int speciesIndex) {

    double harvestRate = m_HarvestRateList.at(speciesIndex);
    return harvestRate;
}


void Parameters::setCatchabilitiesList(QList<double> list) {
    m_CatchabilitiesList = list;
}


QList<double> Parameters::getCatchabilitiesList() {
    return m_CatchabilitiesList;
}


void Parameters::printCatchabilitiesList() {

    int x = m_CatchabilitiesList.size();
    // qDebug() << "Catchabilities:";
    for (int i = 0; i < x; i++){
        // qDebug() << m_CatchabilitiesList.at(i);
    }
}


double Parameters::getCatchability(const int speciesIndex) {

    double q = m_CatchabilitiesList.at(speciesIndex);
    return q;
}


double Parameters::getCatchability(const int speciesIndex, const int time) {

    return m_CatchabilityMatrix.getMatrixValue(speciesIndex, time);
}


void Parameters::setCatchabilityMatrix(QList<QList<double> > matrix) {
    m_CatchabilityMatrix.setMatrix(matrix);
}


void Parameters::setEffortList(QList<double> list) {
    m_EffortList = list;
}


QList<double> Parameters::getEffortList() {
    return m_EffortList;
}


void Parameters::printEffortList() {

    int x = m_EffortList.size();
    // qDebug() << "Efforts:";
    for (int i = 0; i < x; i++){
        // qDebug() << m_EffortList.at(i);
    }
}


double Parameters::getEffort(const int speciesIndex) {

    double effort = m_EffortList.at(speciesIndex);
    return effort;
}


double Parameters::getEffort(const int speciesIndex, const int time) {

    return m_EffortMatrix.getMatrixValue(speciesIndex, time);
}


void Parameters::setEffortMatrix(QList<QList<double> > matrix) {
    m_EffortMatrix.setMatrix(matrix);
}


void Parameters::setCatchMatrix(QList<QList<double> > matrix) {
    m_CatchMatrix.setMatrix(matrix);
}


double Parameters::getCatch(const int speciesIndex, const int time) {

    return m_CatchMatrix.getMatrixValue(speciesIndex, time);
}



void Parameters::setInitialBiomassList(QList<double> list) {
    m_InitialBiomassList = list;

    QList<QList<double> > *biomassMatrix = new QList<QList<double> >;
    int speciesIndex;
    double value;
    foreach (value, m_InitialBiomassList) {
        QList<double> *newList = new QList<double>;
        newList->append(value);
        biomassMatrix->append(*newList);
        speciesIndex++;
    }
    setBiomassMatrix(*biomassMatrix);
}


QList<double> Parameters::getInitialBiomassList() {
    return m_InitialBiomassList;
}


void Parameters::printInitialBiomassList() {
    double value;
    // qDebug() << "Initial Biomass: ";
    foreach (value, m_InitialBiomassList) {
        // qDebug() << value;
    }
}


void Parameters::setPelagicProportionList(QList<double> list) {
    m_PelagicProportionList = list;
}


QList<double> Parameters::getPelagicProportionList() {
    return m_PelagicProportionList;
}


void Parameters::printPelagicProportionList() {
    double value;
    // qDebug() << "Pelagic Proportion: ";
    foreach (value, m_PelagicProportionList) {
        // qDebug() << value;
    }
}


void Parameters::setDemersalProportionList(QList<double> list) {
    m_DemersalProportionList = list;
}


QList<double> Parameters::getDemersalProportionList() {
    return m_DemersalProportionList;
}


void Parameters::printDemersalProportionList() {
    double value;
    // qDebug() << "Demersal Proportion: ";
    foreach (value, m_DemersalProportionList) {
        // qDebug() << value;
    }
}


double Parameters::getSpeciesK(const int speciesIndex) {

    double speciesK = m_SpeciesKList.at(speciesIndex);
    return speciesK;
}


void Parameters::initializeListsAndTS(const int timestep) {

    // Set biomasses for guilds
    setGuildBiomass(timestep);

    // Creates lists for harvest, q's, and effort

    // Reads in TS for stochasticity, climate, fitting
}



//void Parameters::initializeOutputs() {

//    initializeHarvestLossesMatrix();
//    initializePredationLossesMatrix();
//    initializeCompetitionWithinGroupLossesMatrix();
//}


void Parameters::setGuildBiomass(int timestep) {

    QString guildName = "";

    // Initialize guild biomasses to 0
    double Bg = 0.0;
    int numGuilds = getNumberOfGuilds();
    for (int g = 0; g < numGuilds; g++) {
        guildName = m_GuildList.at(g);
        m_GuildBiomassHash.insert(guildName, Bg);
//         qDebug() << guildName << " : " << m_GuildBiomassHash.value(guildName);
    }


    // For each species in species list
    QString speciesName = "";
    double Bi = 0.0;

    int numSpecies = getNumberOfSpecies();
    for (int i = 0; i < numSpecies; i++) {
        speciesName = getSpeciesName(i);
        // Get its biomass
        Bi = getBiomass(i, timestep);
        // Check guild membership of species
        guildName = getGuildMembership(speciesName);
//         qDebug() << speciesName << " : " << guildName << " : " << Bi;
        // Add biomass to the guild's running total
        Bg = m_GuildBiomassHash.value(guildName) + Bi;
        m_GuildBiomassHash.insert(guildName,Bg);
    }

    for (int g = 0; g < numGuilds; g++) {
        guildName = m_GuildList.at(g);
//         qDebug() << guildName << " : " << m_GuildBiomassHash.value(guildName);
    }


}


int Parameters::get_GA_generations() {

    return m_GA_generations;
}


int Parameters::get_GA_populationSize() {

    return m_GA_populationSize;
}


double Parameters::get_GA_mutationRate() {

    return m_GA_mutationRate;
}


void Parameters::set_GA_generations(int generations) {

    m_GA_generations = generations;
}


void Parameters::set_GA_populationSize(int populationSize) {

    m_GA_populationSize = populationSize;
}


void Parameters::set_GA_mutationRate(double mutationRate) {

    m_GA_mutationRate = mutationRate;
}
