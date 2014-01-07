#include "InteractionArc.h"
#include "Color.h"

InteractionArc::InteractionArc(PlotManager *pm, float coefficient, SmallMultiple *source, SmallMultiple *recipient) 
: BetweenSpeciesArc(pm, coefficient, source, recipient, " interact with ") {    
    color = &Color::skyblue;
}

InteractionArc::~InteractionArc() {

}


