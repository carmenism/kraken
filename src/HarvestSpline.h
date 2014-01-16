#ifndef HARVESTSPLINE_H_
#define HARVESTSPLINE_H_

class SmallMultiple;
class ChangeSlider;

#include "SplineCurve.h"

class HarvestSpline : public SplineCurve {
public:
    HarvestSpline(ChangeSlider *slider, SmallMultiple *chart);
    ~HarvestSpline();

    virtual void construct();
protected:
    ChangeSlider *slider;
    SmallMultiple *chart;
};

#endif /*HARVESTSPLINE_H_*/