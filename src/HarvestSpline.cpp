#include "HarvestSpline.h"
#include "Color.h"
#include "Point.h"
#include "Slider.h"
#include "SmallMultiple.h"

HarvestSpline::HarvestSpline(Slider *slider, SmallMultiple *chart)
: SplineCurve(100, new Point(0, 0), new Point(0, 0), new Point(0, 0), new Point(0, 0)) {
    this->slider = slider;
    this->chart = chart;

    setColor(new Color(0, 0, 0, 0.65));
}

HarvestSpline::~HarvestSpline() {

}

void HarvestSpline::construct() {
    float sliderX = slider->getX() + slider->getWidth();
    float sliderY = slider->getY() + slider->getHeight() / 2.0;

    pointA->setValues(sliderX, sliderY);
    
    float chartX = chart->getXLocation();
    float chartY = chart->getYLocation() + chart->getHeight() / 2.0;

    pointB->setValues(chartX, chartY);

    float diffX = chartX - sliderX;
    
    controlA->setValues(sliderX + diffX / 3.0, sliderY);
    controlB->setValues((chartX + sliderX) / 2, (chartY + sliderY) / 2);

    SplineCurve::construct();
}