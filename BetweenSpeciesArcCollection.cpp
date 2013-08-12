#include "BetweenSpeciesArcCollection.h"
#include "BetweenSpeciesArc.h"
#include "InteractionArc.h"
#include "PredationArc.h"

BetweenSpeciesArcCollection::BetweenSpeciesArcCollection() {
    arcs = new BetweenSpeciesArcList();
    selected = NULL;
}

BetweenSpeciesArcCollection::~BetweenSpeciesArcCollection() {
    while (!arcs->empty()) {
        BetweenSpeciesArc *a = arcs->back();
        arcs->pop_back();
        delete a;
    }

    delete arcs;
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

void BetweenSpeciesArcCollection::addArc(int type, float coeff, SingleSpeciesLineChart *speciesA, SingleSpeciesLineChart *speciesB, Color *c, bool setToLeft) {
    BetweenSpeciesArc *arc;

    if (type == ARC_INTERACTION) {
        arc = new InteractionArc(coeff, speciesA, speciesB);
    } else {
        arc = new PredationArc(coeff, speciesA, speciesB);
    }

    arc->setColor(c);

    if (setToLeft) {
        arc->setArcToLeft();
    }

    addArc(arc);
}