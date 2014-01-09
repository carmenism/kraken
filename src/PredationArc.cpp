#include "PredationArc.h"
#include "Color.h"

PredationArc::PredationArc(PlotManager *pm, float coefficient, SmallMultiple *source, SmallMultiple *recipient) 
: InterSpeciesArc(pm, coefficient, source, recipient, " predate on ") {    
    color = &Color::orange;
}

PredationArc::~PredationArc() {

}

float PredationArc::getCoefficent() {
    return (coefficient * 250000);
}

float PredationArc::getDynamicConstant() {
    return 100;
}
