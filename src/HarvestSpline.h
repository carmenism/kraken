#ifndef HARVESTSPLINE_H_
#define HARVESTSPLINE_H_

class SmallMultiple;
class Slider;

#include "SplineCurve.h"

class HarvestSpline : public SplineCurve {
public:
    HarvestSpline(Slider *slider, SmallMultiple *chart);
    ~HarvestSpline();

    virtual void construct();
protected:
    Slider *slider;
    SmallMultiple *chart;
};

#endif /*HARVESTSPLINE_H_*/