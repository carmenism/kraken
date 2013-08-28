#include "GA_Estimator.h"


GA_Estimator::GA_Estimator() {

    initialize_GA_params();
    initializeOutputFile();
}



GA_Estimator::GA_Estimator(Parameters *ptr_paramObj, TimeSeriesObservations* ptr_tsObservationsObj, ModelFormParameters* ptr_modelParametersObj) {

    m_timeSeriesPtr = ptr_tsObservationsObj;
    m_paramsPtr = ptr_paramObj;
    m_paramsPtr->clearBiomassMatrix();
    m_modelFormPtr = ptr_modelParametersObj;

    initialize_GA_params();
    initializeOutputFile();
}


GA_Estimator::~GA_Estimator() {

    m_outfile.close();
}


void GA_Estimator::initialize_GA_params() {

    m_generation = 0;

    m_populationSize = m_paramsPtr->get_GA_populationSize();
    m_mutationRate = m_paramsPtr->get_GA_mutationRate();
    m_maxGenerations = m_paramsPtr->get_GA_generations();

}


void GA_Estimator::setSaveFilenames(QString filename, QString filenamebest) {

    m_OutputFilename = filename;
    m_OutputFilenameBest = filenamebest;
}


void GA_Estimator::estimateParameters() {


    // Create population of randomly created individual chromosomes including all parameters to be estimated
    createPopulation();
    qDebug() << "Generation #" << m_generation;
    calculateFitness();
//    qDebug() << "Calculating Fitness...";
    eliminateUnfit();
//    qDebug() << "Eliminating Unfit...";
    calculateSelectionProbabilities();
//    qDebug() << "Calculating Selection Probabilities...";
    outputResults();
//    qDebug() << "Outputting results...";

    for (int generation = 0; generation < m_maxGenerations-1; generation++) {
        // Determine next population until fitness below cutoff level
        createNextGeneration();
        qDebug() << "Generation #" << m_generation;

        // Determine fitness of each chromosome
        calculateFitness();

        // Eliminate completely unfit individuals (those with a nan value)
        eliminateUnfit();

        // Create Selection Probabilities
        calculateSelectionProbabilities();

        // Output Results
        appendResults();

    }
}


void GA_Estimator::createPopulation() {

    MTRand randObj;

    // Get parameters to estimate

    QList<double> chromosomeList = m_paramsPtr->getChromosomeList();
    int chromosomeSize = chromosomeList.size();
    // Create population of randomly created individual chromosomes which include all parameters to be estimated
    double paramValue;
    double diff;
    double max;
    double min;
    for (int i = 0; i < m_populationSize; i++) {
        Parameters paramChromosome = *m_paramsPtr;
        QList<double> newChromosome;
        for (int j = 0; j < chromosomeSize; j++) {
//            paramValue = randObj.rand(m_paramValueTop);
            min = m_paramsPtr->getChromosomeMin(j);
            max = m_paramsPtr->getChromosomeMax(j);
            if (min == max) {
                newChromosome.append(min);
            } else {
                diff = max - min;
                paramValue = randObj.rand(diff) + min;
                newChromosome.append(paramValue);
//                qDebug() << "Max: " << max << " ; Min: " << min << " ; ParamValue: " << paramValue;
            }
        }
        paramChromosome.setChromosomeList(newChromosome);
        m_chromosomeParametersList.append(paramChromosome);
    }

}


void GA_Estimator::calculateFitness() {

    int popSize = m_chromosomeParametersList.size();
//    qDebug() << "Number of models: " << popSize;
    double fitness = 0.0;
    double RMSE = 0.0;

    m_chromosomeFitnessList.clear();
    m_chromosomeRMSElist.clear();
//    qDebug() << "After clearing fitness list";
    for (int i = 0; i < popSize; i++) {
        Parameters tempParam = m_chromosomeParametersList.at(i);
        Parameters* tempParamPtr = &tempParam;
        Model model(tempParamPtr, m_modelFormPtr);
        model.runModel_noOutput();
//        qDebug() << "Model #" << i;
        m_measureOfFitnessObj.calculateMeasuresOfFitness(tempParamPtr,m_timeSeriesPtr);
//        qDebug() << "Calculating fitness for Model #" << i;

        fitness = m_measureOfFitnessObj.getRMSE_modified();
        RMSE = m_measureOfFitnessObj.getRMSE();
        m_chromosomeFitnessList.append(fitness);
        m_chromosomeRMSElist.append(RMSE);
//        qDebug() << "Chromosme #" << i << " fitness: " << m_chromosomeFitnessList.at(i);
    }

}


void GA_Estimator::eliminateUnfit() {

    int popSize = m_chromosomeParametersList.size();

    QList<Parameters> validChromosomesList;
    QList<double> validChromosomesFitnessList;

    for (int i = 0; i < popSize; i++) {
        if (!isnan_local(m_chromosomeFitnessList.at(i)) && !isinf_local(m_chromosomeFitnessList.at(i))) {
            Parameters tempParam = m_chromosomeParametersList.at(i);
            double tempFitness = m_chromosomeFitnessList.at(i);
            validChromosomesList.append(tempParam);
            validChromosomesFitnessList.append(tempFitness);
        }
    }

    m_chromosomeParametersList = validChromosomesList;
    m_chromosomeFitnessList = validChromosomesFitnessList;
}


void GA_Estimator::calculateSelectionProbabilities() {

    int popSize = m_chromosomeParametersList.size();

    // Reverse the fitnesses so that larger values are more fit.  Needed for calculation purposes
    QList<double> reversedChromosomesFitnessList;

    // Determine max value
    double maxValue = 0.0;
    double currentValue = 0.0;
    for (int i = 0; i < popSize; i++) {
        currentValue = m_chromosomeFitnessList.at(i);
        if (currentValue > maxValue) {
            maxValue = currentValue;
        }
    }

    // Perform reversal of fitnesses and keep track of the total sum of the reversed fitness values
    double reversedValue = 0.0;
    double sumOfFitness = 0.0;
    for (int i = 0; i < popSize; i++) {
        currentValue = m_chromosomeFitnessList.at(i);
        reversedValue = maxValue / currentValue;
        reversedChromosomesFitnessList.append(reversedValue);
        sumOfFitness += reversedValue;
    }

    // Calculate selection probabilities
    m_chromosomeSelectionProbabilityList.clear();
    double selectionProbability = 0.0;
    for (int i = 0; i < popSize; i++) {
        selectionProbability = reversedChromosomesFitnessList.at(i) / sumOfFitness;
        m_chromosomeSelectionProbabilityList.append(selectionProbability);
    }

    // Create Roulette Wheel
    m_rouletteWheelList.clear();
    int numberOfTickets = 0;
    for (int i = 0; i < popSize; i++) {
        numberOfTickets = m_chromosomeSelectionProbabilityList.at(i) * 100;
        for (int n = 0; n < numberOfTickets; n++) {
            m_rouletteWheelList.append(i);
        }
    }

}


void GA_Estimator::createNextGeneration() {


    // Increase generation number
    m_generation++;

    rouletteSelection();

}


void GA_Estimator::tournamentSelection() {

    MTRand randObj;
    int tournamentSize = 10;
    int popSize = m_chromosomeParametersList.size();

    int firstParentIndex;
    int secondParentIndex;
    int potParentIndex;

    for (int i = 0; i < m_populationSize; i++) {
       firstParentIndex = randObj.rand(popSize);
        for (int p = 1; p < tournamentSize; p++) {
            potParentIndex = randObj.rand(popSize);
            if (m_chromosomeFitnessList.at(potParentIndex) > m_chromosomeFitnessList.at(firstParentIndex)) {
                firstParentIndex = potParentIndex;
            }
        }

        secondParentIndex = randObj.rand(popSize);
        for (int p = 1; p < tournamentSize; p++) {
            potParentIndex = randObj.rand(popSize);
            if (m_chromosomeFitnessList.at(potParentIndex) > m_chromosomeFitnessList.at(firstParentIndex)) {
                secondParentIndex = potParentIndex;
            }
        }
    }

//    m_chromosomeParametersList.clear();
//    m_chromosomeParametersList = nextGenChromosomeParametersList;

}


void GA_Estimator::rouletteSelection() {

    MTRand randObj;

    long numOfTickets = m_rouletteWheelList.size();
//    qDebug() << numOfTickets;

    int firstTicket;
    int secondTicket;
    int firstParentIndex;
    int secondParentIndex;

    Parameters firstParent;
    Parameters secondParent;
    Parameters firstChild;

    QList<double> firstParentList;
    QList<double> secondParentList;
    QList<double> firstChildList;

    double testForMutationValue;

    QList<Parameters> nextGenChromosomeParametersList;
    for (int i = 0; i < m_populationSize; i++) {
        firstTicket = randObj.rand(numOfTickets);
        secondTicket = randObj.rand(numOfTickets);
        firstParentIndex = m_rouletteWheelList.at(firstTicket);
        secondParentIndex = m_rouletteWheelList.at(secondTicket);

        firstParent = m_chromosomeParametersList.at(firstParentIndex);
        secondParent = m_chromosomeParametersList.at(secondParentIndex);
        firstParentList = firstParent.getChromosomeList();
        secondParentList = secondParent.getChromosomeList();

        int chromosomeSize = firstParentList.size();
        double newParamValue = 0.0;
        double min = 0.0;
        double max = 0.0;
        double diff = 0.0;

        // Reset child
        firstChildList.clear();

        // For each parameter in the chromosome, recombine parents into new offspring
        for (int c = 0; c < chromosomeSize; c++) {
            min = m_paramsPtr->getChromosomeMin(c);
            max = m_paramsPtr->getChromosomeMax(c);
            if (min == max) {
                firstChildList.append(min);
            } else {
                // Check for mutation
                testForMutationValue = randObj.rand() * 100;
    //            qDebug() << "Random #: " << testForMutationValue;
                if (testForMutationValue <= m_mutationRate) {
    //                qDebug() << "Child # " << i << " MUTATING";
                    diff = max - min;
                    newParamValue = randObj.rand(diff) + min;
                // If no mutation, new value is combination of parents
                } else {
                    newParamValue = (firstParentList.at(c) + secondParentList.at(c)) / 2;
                }
            firstChildList.append(newParamValue);
            }
        }
        firstChild = *m_paramsPtr;
        firstChild.setChromosomeList(firstChildList);
        nextGenChromosomeParametersList.append(firstChild);
//        Parameters tempParam = nextGenChromosomeParametersList.at(i);
//        QList<double> childList = tempParam.getSpeciesKList();


  //      qDebug() << "Parent 1 Index = " << firstParentIndex << " : Parent 2 Index = " << secondParentIndex;
  //      qDebug() << firstParentList;
  //      qDebug() << secondParentList;
  //      qDebug() << childList;

    }

    m_chromosomeParametersList.clear();
    m_chromosomeParametersList = nextGenChromosomeParametersList;
}


void GA_Estimator::initializeOutputFile() {

//    m_outfile = QFile("/home/rgamble/Desktop/KRAKEN/GA_output.csv");
//    if (!m_outfile.open(QIODevice::WriteOnly | QIODevice::Text)) {
//         qDebug() << "GA output file NOT OPENED";
//    }
//    m_textStreamOut(&m_outfile);
//    m_textStreamOut << "Generation,Chromosome,RMSE,Selection Probability";
//    double numParameters = m_paramsPtr->getNumberOfSpecies();
//    QString paramString;
//    QString newParamString;
//    for (int i = 0; i < numParameters; i++) {
//        newParamString.setNum(i+1);
//        paramString = "Parameter ";
//        paramString.append(newParamString);
//        m_textStreamOut << paramString << ",";
//    }
//    m_textStreamOut << "\n";

}

void GA_Estimator::outputResults() {

    QFile outfile(m_OutputFilename);
    if (!outfile.open(QIODevice::WriteOnly | QIODevice::Text))
         qDebug() << "GA output file NOT OPENED";
    QTextStream textStreamOut(&outfile);
    QFile outfileBest(m_OutputFilenameBest);
    if (!outfileBest.open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug() << "GA Best output file NOT OPENED";
    QTextStream textStreamOutBest(&outfileBest);

    textStreamOut << "Generation,Chromosome,Fitness,RMSE,Selection Probability,";
    textStreamOutBest << "Generation,Chromosome,Fitness,RMSE,Selection Probability,";
    double numParameters = m_paramsPtr->getChromosomeListSize();
    QString paramString;
    QString newParamString;
    for (int i = 0; i < numParameters; i++) {
        newParamString.setNum(i+1);
        paramString = "Parameter ";
        paramString.append(newParamString);
        textStreamOut << paramString << ",";
        textStreamOutBest << paramString << ",";
    }
    textStreamOut << "\n";
    textStreamOutBest << "\n";

    int popSize = m_chromosomeParametersList.size();
//    qDebug() << "Population Size: " << popSize;
    int chromosomeSize;

    QList<double> chromosomeList;
    double bestFitness = 1000000000000000.0;
    int bestFitnessIndex;
    for (int i = 0; i < popSize; i++) {
        textStreamOut << m_generation << "," << i << "," << m_chromosomeFitnessList.at(i) << "," << m_chromosomeRMSElist.at(i) << "," << m_chromosomeSelectionProbabilityList.at(i) << ",";
        Parameters tempParam = m_chromosomeParametersList.at(i);
        chromosomeList = tempParam.getChromosomeList();
        chromosomeSize = chromosomeList.size();
        for (int j = 0; j < chromosomeSize; j++) {
            textStreamOut << chromosomeList.at(j) << ",";
        }
        textStreamOut << "\n";

        if (m_chromosomeFitnessList.at(i) < bestFitness) {
            bestFitnessIndex = i;
            bestFitness = m_chromosomeFitnessList.at(i);
        }
    }
    textStreamOutBest << m_generation << "," << bestFitnessIndex << "," << m_chromosomeFitnessList.at(bestFitnessIndex) << "," << m_chromosomeRMSElist.at(bestFitnessIndex) << "," << m_chromosomeSelectionProbabilityList.at(bestFitnessIndex) << ",";
    Parameters tempParam = m_chromosomeParametersList.at(bestFitnessIndex);
    chromosomeList = tempParam.getChromosomeList();
    chromosomeSize = chromosomeList.size();
    for (int j = 0; j < chromosomeSize; j++) {
        textStreamOutBest << chromosomeList.at(j) << ",";
    }
    textStreamOutBest << "\n";
    outfile.close();
    outfileBest.close();
}


void GA_Estimator::appendResults() {

    QFile outfile(m_OutputFilename);
    if (!outfile.open(QIODevice::Append | QIODevice::Text))
         qDebug() << "GA output file NOT OPENED";
    QTextStream textStreamOut(&outfile);
    QFile outfileBest(m_OutputFilenameBest);
    if (!outfileBest.open(QIODevice::Append | QIODevice::Text))
        qDebug() << "GA Best output file NOT OPENED";
    QTextStream textStreamOutBest(&outfileBest);

    int popSize = m_chromosomeParametersList.size();
    int chromosomeSize;

    QList<double> chromosomeList;
    double bestFitness = 1000000000000000.0;
    int bestFitnessIndex;
    for (int i = 0; i < popSize; i++) {
        textStreamOut << m_generation << "," << i << "," << m_chromosomeFitnessList.at(i) << "," << m_chromosomeRMSElist.at(i) << "," << m_chromosomeSelectionProbabilityList.at(i) << ",";

        if (m_chromosomeFitnessList.at(i) < bestFitness) {
            bestFitnessIndex = i;
            bestFitness = m_chromosomeFitnessList.at(i);
        }
        Parameters tempParam = m_chromosomeParametersList.at(i);
        chromosomeList = tempParam.getChromosomeList();
        chromosomeSize = chromosomeList.size();
        for (int j = 0; j < chromosomeSize; j++) {
            textStreamOut << chromosomeList.at(j) << ",";
        }
        textStreamOut << "\n";
    }
    textStreamOutBest << m_generation << "," << bestFitnessIndex << "," << m_chromosomeFitnessList.at(bestFitnessIndex) << "," << m_chromosomeRMSElist.at(bestFitnessIndex) << "," << m_chromosomeSelectionProbabilityList.at(bestFitnessIndex) << ",";
    Parameters tempParam = m_chromosomeParametersList.at(bestFitnessIndex);
    chromosomeList = tempParam.getChromosomeList();
    chromosomeSize = chromosomeList.size();
    for (int j = 0; j < chromosomeSize; j++) {
        textStreamOutBest << chromosomeList.at(j) << ",";
    }
    textStreamOutBest << "\n";
    outfile.close();
    outfileBest.close();

}
