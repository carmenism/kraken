#include "BetweenSpeciesArcCollection.h"
#include "BetweenSpeciesArc.h"
#include "InteractionArc.h"
#include "PredationArc.h"
#include "PrintText.h"
#include <QtOpenGL>

BetweenSpeciesArcCollection::BetweenSpeciesArcCollection(std::string title) {
    arcs = new BetweenSpeciesArcList();
    selected = NULL;
    this->title = title;
    fontHeight = 14;
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