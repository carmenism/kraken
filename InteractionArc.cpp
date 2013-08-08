#include "InteractionArc.h"

InteractionArc::InteractionArc(float coefficient, SingleSpeciesLineChart *speciesA, SingleSpeciesLineChart *speciesB) 
: BetweenSpeciesArc(coefficient, speciesA, speciesB, " interact with ") {    
    
}

InteractionArc::~InteractionArc() {

}

float InteractionArc::getThickness() {
    return 30 * coefficient;
}
