#include "InteractionArc.h"
#include "Color.h"

InteractionArc::InteractionArc(PlotManager *pm, float coefficient, SmallMultiple *source, SmallMultiple *recipient) 
: InterSpeciesArc(pm, coefficient, source, recipient, " interact with ") {    
    color = &Color::skyblue;
}

InteractionArc::~InteractionArc() {

}


