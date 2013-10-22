#ifndef MONTECARLO_H_
#define MONTECARLO_H_

#include <QList>

class MonteCarlo {
public:
    MonteCarlo();
    ~MonteCarlo();
protected:
    int numberSimulations;

    static int getRandomi(int min, int max);
    static double getRandomd(double min, double max);
    
    static double addJitter(double num, double percentJitter);
    static double addJitterWithinRange(double num, double percentJitter, double min, double max);

    static QList<QList<double>> makeJitteredMatrix(QList<QList<double>> matrix, double percentJitter);
    static QList<QList<double>> makeJitteredMatrixWithinRange(QList<QList<double>> matrix, double percentJitter, double min, double max);
    
    static QList<double> makeJitteredList(QList<double> list, double percentJitter);
    static QList<double> makeJitteredListWithinRange(QList<double> list, double percentJitter, double min, double max);

    static QList<QList<double>> makeRandomMatrix(int r, int c, double min, double max);
    static QList<double> makeRandomList(int length, double min, double max);
};

#endif /* MONTECARLO_H_ */