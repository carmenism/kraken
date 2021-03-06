#include "InteractionArc.h"
#include "Color.h"

InteractionArc::InteractionArc(PlotManager *pm, float coefficient, SmallMultiple *source, SmallMultiple *recipient) 
: InterSpeciesArc(pm, coefficient, source, recipient, " compete with ") {    
    color = new Color(Color::skyblue);
}

InteractionArc::~InteractionArc() {

}

float InteractionArc::getDynamicConstant() {
    return 0.001;
}

