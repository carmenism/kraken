#include "InteractionArc.h"
#include "Color.h"

InteractionArc::InteractionArc(PlotManager *pm, float coefficient, SingleSpeciesLineChart *speciesA, SingleSpeciesLineChart *speciesB) 
: BetweenSpeciesArc(pm, coefficient, speciesA, speciesB, " interact with ") {    
    color = &Color::skyblue;
}

InteractionArc::~InteractionArc() {

}


