#include "BetweenSpeciesArcCollection.h"
#include "BetweenSpeciesArc.h"
#include "InteractionArc.h"
#include "PredationArc.h"
#include "PrintText.h"
#include <QtOpenGL>

BetweenSpeciesArcCollection::BetweenSpeciesArcCollection(PlotManager *pm, std::string title) {
    arcs = new BetweenSpeciesArcList();
    selected = NULL;
    this->title = title;
    fontHeight = 14;
    this->plotManager = pm;
}

BetweenSpeciesArcCollection::~BetweenSpeciesArcCollection() {
    while (!arcs->empty()) {
        BetweenSpeciesArc *a = arcs->back();
        arcs->pop_back();
        delete a;
    }

    delete arcs;
}

void BetweenSpeciesArcCollection::setTitleLocation(float x, float y) {
    titleX = x;
    titleY = y;
}

void BetweenSpeciesArcCollection::draw() {
    selected = NULL;

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

void BetweenSpeciesArcCollection::drawSelected() {
    if (selected != NULL) {
        selected->drawSelected();
    }
}

void BetweenSpeciesArcCollection::drawToPick() {
    for (unsigned int i = 0; i < arcs->size(); i++) {
        arcs->at(i)->drawToPick();
    }
}

void BetweenSpeciesArcCollection::addArc(BetweenSpeciesArc *arc) {
    arcs->push_back(arc);
}

void BetweenSpeciesArcCollection::addArc(int type, float coeff, SingleSpeciesLineChart *speciesA, SingleSpeciesLineChart *speciesB, bool setToLeft, Color *c) {
    BetweenSpeciesArc *arc;

    if (type == ARC_INTERACTION) {
        arc = new InteractionArc(plotManager, coeff, speciesA, speciesB);
    } else {
        arc = new PredationArc(plotManager, coeff, speciesA, speciesB);
    }

    if (c != NULL) {
        arc->setColor(c);
    }

    if (setToLeft) {
        arc->setArcToLeft();
    }

    addArc(arc);
}

void BetweenSpeciesArcCollection::setDisplayDynamically(bool d) {
    for (unsigned int i = 0; i < arcs->size(); i++) {
        arcs->at(i)->setDisplayDynamically(d);
    }
}

void BetweenSpeciesArcCollection::displayDynamicallyOn() {
    setDisplayDynamically(true);
}

void BetweenSpeciesArcCollection::displayDynamicallyOff() {
    setDisplayDynamically(false);
}

void BetweenSpeciesArcCollection::setAdjustType(int t) {
    for (unsigned int i = 0; i < arcs->size(); i++) {
        arcs->at(i)->setAdjustType(t);
    }
}

void BetweenSpeciesArcCollection::adjustNone() {
    setAdjustType(ADJUST_NONE);
}

void BetweenSpeciesArcCollection::adjustSmaller() {
    setAdjustType(ADJUST_SMALLER);
}

void BetweenSpeciesArcCollection::adjustLarger() {
    setAdjustType(ADJUST_LARGER);
}

void BetweenSpeciesArcCollection::setAdjustPercentage(float p) {
    for (unsigned int i = 0; i < arcs->size(); i++) {
        arcs->at(i)->setAdjustPercentage(p);
    }
}

void BetweenSpeciesArcCollection::addArcs(BetweenSpeciesArcCollection *otherArcs) {
    for (unsigned int i = 0; i < otherArcs->arcs->size(); i++) {
        arcs->push_back(otherArcs->arcs->at(i));
    }
}

void BetweenSpeciesArcCollection::clear() {
    arcs->clear();
}

BetweenSpeciesArcList *BetweenSpeciesArcCollection::getVisibleArcs() {
    BetweenSpeciesArcList *newList = new BetweenSpeciesArcList();

    for (unsigned int i = 0; i < arcs->size(); i++) {
        if (!arcs->at(i)->getDisplayDynamically()) {
            newList->push_back(arcs->at(i));
        } else if (arcs->at(i)->getDynamicThickness() > 1) {
            newList->push_back(arcs->at(i));
        }
    }

    return newList;
}