#include "GroupReordering.h"
#include <QList>
#include <QStringList>
#include "MS_PROD_MainWindow.h"

GroupReordering::GroupReordering(MS_PROD_MainWindow *mw) {
    mainWindow = mw;

    QStringList labels = mainWindow->getParameters()->getSpeciesList();

    oldIndices = determineNewOrder(labels);
    newLabels = determineNewLabels(labels);
}

QList<QList<double> *> *GroupReordering::getNewTimeSeriesMatrix(QList<QList<double>> matrix) {
    QList<QList<double> *> *newMatrix = new QList<QList<double> *>();

    for (int i = 0; i < oldIndices->size(); i++) {
        QList<double> *newRow = new QList<double>();

        for (int j = 0; j < matrix.at(oldIndices->at(i)).size(); j++) {
            newRow->append( matrix.at(oldIndices->at(i)).at(j) );
        }

        newMatrix->append(newRow);
    }

    return newMatrix;
}

QList<QList<double> *> *GroupReordering::getNewSquareMatrix(QList<QList<double>> matrix) {
    QList<QList<double> *> *newMatrix = new QList<QList<double> *>();

    for (int i = 0; i < oldIndices->size(); i++) {
        QList<double> *newRow = new QList<double>();

        for (int j = 0; j < matrix.at(oldIndices->at(i)).size(); j++) {
            newRow->append( matrix.at(oldIndices->at(i)).at(oldIndices->at(j)) );
        }

        newMatrix->append(newRow);
    }

    return newMatrix;
}

QStringList *GroupReordering::determineNewLabels(QStringList labels) {
    QStringList *newList = new QStringList();
    
    for (int i = 0; i < oldIndices->size(); i++) {
        newList->append(labels.at(oldIndices->at(i)));
    }

    return newList;
}

QList<int> *GroupReordering::determineNewOrder(QStringList labels) {
    QList<int> *oldIndices = new QList<int>();

    QStringList guilds = mainWindow->getParameters()->getGuildList();

    for (int g = 0; g < guilds.size(); g++) {
        for (int s = 0; s < labels.size(); s++) {
            QString guild = mainWindow->getParameters()->getGuildMembership(labels.at(s));

            if (QString::compare(guilds.at(g), guild) == 0) {
                oldIndices->append(s);
            }
        }
    }

    return oldIndices;
}