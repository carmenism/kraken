#include "InterSpeciesArcCollection.h"
#include "InterSpeciesArc.h"
#include "InteractionArc.h"
#include "PredationArc.h"
#include "PrintText.h"
#include "ColorLegend.h"
#include <QtOpenGL>
#include <iostream>

extern int timer;

InterSpeciesArcCollection::InterSpeciesArcCollection(PlotManager *pm, std::string title) {
    arcs = new InterSpeciesArcList();
    selected = NULL;
    this->title = title;
    fontHeight = 14;
    this->plotManager = pm;
    legend = NULL;
}

InterSpeciesArcCollection::~InterSpeciesArcCollection() {
    while (!arcs->empty()) {
        InterSpeciesArc *a = arcs->back();
        arcs->pop_back();
        delete a;
    }

    delete arcs;
    delete legend;
}

void InterSpeciesArcCollection::setTitleLocation(float x, float y) {
    titleX = x;
    titleY = y;
}

void InterSpeciesArcCollection::findSelected() {
    selected = NULL;

    if (legend != NULL) {
        legend->draw();
    } else {
        glColor4f(0, 0, 0, 1);
        PrintText::drawStrokeText(title, titleX, titleY, fontHeight, HORIZ_LEFT, VERT_BOTTOM, false, 0);
    }

    //std::cerr << timer << "\n";

    for (unsigned int i = 0; i < arcs->size(); i++) {
        if (arcs->at(i)->getSelected()){
            selected = arcs->at(i);
        }
    }
}

void InterSpeciesArcCollection::draw() {
	if (selected != NULL) {
        for (unsigned int i = 0; i < arcs->size(); i++) {
            if (!arcs->at(i)->getSelected()){
                arcs->at(i)->drawFaded();
            }
        }
        
        selected->draw();
	} else {
		for (unsigned int i = 0; i < arcs->size(); i++) {
            arcs->at(i)->draw();
        }
	}
}

void InterSpeciesArcCollection::drawToPick() {
    for (unsigned int i = 0; i < arcs->size(); i++) {
        arcs->at(i)->drawToPick();
    }
}

void InterSpeciesArcCollection::addArc(InterSpeciesArc *arc) {
    arcs->push_back(arc);
}

void InterSpeciesArcCollection::addArc(int type, float coeff, SmallMultiple *source, SmallMultiple *recipient, bool setToLeft) {
    InterSpeciesArc *arc;

    if (type == ARC_INTERACTION) {
        arc = new InteractionArc(plotManager, coeff, source, recipient);
    } else {
        arc = new PredationArc(plotManager, coeff, source, recipient);
    }

    if (setToLeft) {
        arc->setArcToLeft();
    }

    addArc(arc);
}

void InterSpeciesArcCollection::setDisplayDynamically(bool d) {
    for (unsigned int i = 0; i < arcs->size(); i++) {
        arcs->at(i)->setDisplayDynamically(d);
    }
}

void InterSpeciesArcCollection::displayDynamicallyOn() {
    setDisplayDynamically(true);
}

void InterSpeciesArcCollection::displayDynamicallyOff() {
    setDisplayDynamically(false);
}

void InterSpeciesArcCollection::setAdjustType(AdjustType t) {
    for (unsigned int i = 0; i < arcs->size(); i++) {
        arcs->at(i)->setAdjustType(t);
    }
}

void InterSpeciesArcCollection::adjustNone() {
    setAdjustType(ADJUST_NONE);
}

void InterSpeciesArcCollection::adjustSmaller() {
    setAdjustType(ADJUST_SMALLER);
}

void InterSpeciesArcCollection::adjustLarger() {
    setAdjustType(ADJUST_LARGER);
}

void InterSpeciesArcCollection::setAdjustPercentage(float p) {
    for (unsigned int i = 0; i < arcs->size(); i++) {
        arcs->at(i)->setAdjustPercentage(p);
    }
}

void InterSpeciesArcCollection::addArcs(InterSpeciesArcCollection *otherArcs) {
    for (unsigned int i = 0; i < otherArcs->arcs->size(); i++) {
        arcs->push_back(otherArcs->arcs->at(i));
    }
}

void InterSpeciesArcCollection::clear() {
    arcs->clear();
}

InterSpeciesArcList *InterSpeciesArcCollection::getVisibleArcs() {
    InterSpeciesArcList *newList = new InterSpeciesArcList();

    for (unsigned int i = 0; i < arcs->size(); i++) {
        if (!arcs->at(i)->getDisplayDynamically()) {
            newList->push_back(arcs->at(i));
        } else if (arcs->at(i)->getDynamicThickness() != 0) {
            newList->push_back(arcs->at(i));
        }
    }

    return newList;
}

bool InterSpeciesArcCollection::isAnimated() {
    if (!arcs->empty()) {
        return arcs->front()->isAnimated();
    }

    return false;
}

bool InterSpeciesArcCollection::getAnimated() {
    return isAnimated();
}

void InterSpeciesArcCollection::setAnimated(bool a) {
    for (unsigned int i = 0; i < arcs->size(); i++) {
        arcs->at(i)->setAnimated(a);
    }
}

void InterSpeciesArcCollection::animatedOn() {
    setAnimated(true);
}

void InterSpeciesArcCollection::animatedOff() {
    setAnimated(false);
}

int InterSpeciesArcCollection::size() {
	return arcs->size();
}

InterSpeciesArc *InterSpeciesArcCollection::at(int i) {
	return arcs->at(i);
}