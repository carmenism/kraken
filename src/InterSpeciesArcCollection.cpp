#include "InterSpeciesArcCollection.h"
#include "InterSpeciesArc.h"
#include "InteractionArc.h"
#include "PredationArc.h"
#include "PrintText.h"
#include <QtOpenGL>
#include <iostream>

extern int timer;

InterSpeciesArcCollection::InterSpeciesArcCollection(PlotManager *pm, std::string title) {
    arcs = new InterSpeciesArcList();
    selected = NULL;
    this->title = title;
    fontHeight = 14;
    this->plotManager = pm;
}

InterSpeciesArcCollection::~InterSpeciesArcCollection() {
    while (!arcs->empty()) {
        InterSpeciesArc *a = arcs->back();
        arcs->pop_back();
        delete a;
    }

    delete arcs;
}

void InterSpeciesArcCollection::setTitleLocation(float x, float y) {
    titleX = x;
    titleY = y;
}

void InterSpeciesArcCollection::draw() {
    selected = NULL;

    //std::cerr << timer << "\n";

    for (unsigned int i = 0; i < arcs->size(); i++) {
        if (arcs->at(i)->getSelected()){
            selected = arcs->at(i);
        }
    }

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

    glColor4f(0, 0, 0, 1);
    PrintText::drawStrokeText(title, titleX, titleY, fontHeight, HORIZ_RIGHT, VERT_TOP, false, 0);
}

void InterSpeciesArcCollection::drawSelected() {
    if (selected != NULL) {
        selected->drawSelected();
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

void InterSpeciesArcCollection::addArc(int type, float coeff, SmallMultiple *source, SmallMultiple *recipient, bool setToLeft, Color *c) {
    InterSpeciesArc *arc;

    if (type == ARC_INTERACTION) {
        arc = new InteractionArc(plotManager, coeff, source, recipient);
    } else {
        arc = new PredationArc(plotManager, coeff, source, recipient);
    }

    if (c != NULL) {
        arc->setColor(c);
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

void InterSpeciesArcCollection::setAdjustType(int t) {
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
        } else if (arcs->at(i)->getDynamicThickness() > 1) {
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