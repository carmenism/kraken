#include "PredationArc.h"

PredationArc::PredationArc(float coefficient, SingleSpeciesLineChart *speciesA, SingleSpeciesLineChart *speciesB) 
: BetweenSpeciesArc(coefficient, speciesA, speciesB, " predate on ") {    
    
}

PredationArc::~PredationArc() {

}

float PredationArc::getThickness() {
    return 30 * (coefficient * 250000);
}