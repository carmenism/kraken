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
    static double getRandomd(double min, double max, int n);
  
    static double addJitter(double num, double percentJitter, int n);
    static double addJitterWithinRange(double num, double percentJitter, double min, double max, int n);

    static QList<QList<double>> makeJitteredMatrix(QList<QList<double>> matrix, double percentJitter, int n);
    static QList<QList<double>> makeJitteredMatrixWithinRange(QList<QList<double>> matrix, double percentJitter, double min, double max, int n);
    
    static QList<double> makeJitteredList(QList<double> list, double percentJitter, int n);
    static QList<double> makeJitteredListWithinRange(QList<double> list, double percentJitter, double min, double max, int n);

    static QList<QList<double>> makeRandomMatrix(int r, int c, double min, double max, int n);
    static QList<double> makeRandomList(int length, double min, double max, int n);
};

#endif /* MONTECARLO_H_ */