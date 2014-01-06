#include "InteractionArc.h"
#include "Color.h"

InteractionArc::InteractionArc(PlotManager *pm, float coefficient, SmallMultiple *speciesA, SmallMultiple *speciesB) 
: BetweenSpeciesArc(pm, coefficient, speciesA, speciesB, " interact with ") {    
    color = &Color::skyblue;
}

InteractionArc::~InteractionArc() {

}


