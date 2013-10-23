#ifndef _GROUPREORDERING_H
#define _GROUPREORDERING_H

#include <QList>

class MS_PROD_MainWindow;
class QStringList;

class GroupReordering {
public:
    GroupReordering(MS_PROD_MainWindow *mw);
    ~GroupReordering();

    QStringList *getNewLabels() { return newLabels; }
    QList<QList<double> *> *getNewTimeSeriesMatrix(QList<QList<double>> matrix);
    QList<QList<double> *> *getNewSquareMatrix(QList<QList<double>> matrix);
private:
    MS_PROD_MainWindow *mainWindow;
    QList<int> *oldIndices;
    QStringList *newLabels;
    
    QStringList *determineNewLabels(QStringList labels);
    QList<int> *determineNewOrder(QStringList labels);
};

#endif  /* _GROUPREORDERING_H */