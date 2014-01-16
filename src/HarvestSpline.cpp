#include "HarvestSpline.h"
#include "Color.h"
#include "Point.h"
#include "Slider.h"
#include "SmallMultiple.h"

HarvestSpline::HarvestSpline(Slider *slider, SmallMultiple *chart)
: SplineCurve(100, new Point(0, 0), new Point(0, 0), new Point(0, 0), new Point(0, 0)) {
    this->slider = slider;
    this->chart = chart;

    //setColor(new Color(0.5, 0.5, 0.5, 0.5));
    setColor(new Color(0.9, 0.9, 0, 0.45));
}

HarvestSpline::~HarvestSpline() {

}

void HarvestSpline::construct() {
    float newWidth = slider->getValue() * 5.25;

    float sliderX = slider->getX() + slider->getWidth();
    float sliderY = slider->getY() + slider->getHeight() / 2.0;
    
    float chartX = chart->getXLocation();
    float chartY = chart->getYLocation() + chart->getHeight() / 2.0;

    if (getWidth() != newWidth 
        || pointA->x != sliderX || pointA->y != sliderY
        || pointB->x != chartX || pointB->y != chartY) {
        setWidth(newWidth);
            
        pointA->setValues(sliderX, sliderY);
        pointB->setValues(chartX, chartY);
        
        float diffX = chartX - sliderX;

        controlA->setValues(sliderX + diffX / 3.0, sliderY);
        controlB->setValues(sliderX + 2 * diffX / 3.0, chartY);

        SplineCurve::construct();
    }
}