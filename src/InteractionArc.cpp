#include "InteractionArc.h"
#include "Color.h"

InteractionArc::InteractionArc(PlotManager *pm, float coefficient, SingleSpeciesLineChart *speciesA, SingleSpeciesLineChart *speciesB) 
: BetweenSpeciesArc(pm, coefficient, speciesA, speciesB, " interact with ") {    
    color = &Color::gray;
}

InteractionArc::~InteractionArc() {

}

float InteractionArc::getThickness() {
    return 30 * coefficient;
}
