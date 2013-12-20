#include "PredationArc.h"
#include "Color.h"

PredationArc::PredationArc(PlotManager *pm, float coefficient, SingleSpeciesLineChart *speciesA, SingleSpeciesLineChart *speciesB) 
: BetweenSpeciesArc(pm, coefficient, speciesA, speciesB, " predate on ") {    
    color = &Color::orange;
}

PredationArc::~PredationArc() {

}

float PredationArc::getThickness() {
    return 30 * (coefficient * 250000);
}