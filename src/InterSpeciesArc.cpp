#include "InterSpeciesArc.h"
#include "SmallMultiple.h"
#include <string>

#include "Triangle.h"
#include <QtOpenGL>
#include "PrintText.h"
#include "Color.h"
#include "PlotManager.h"

float *InterSpeciesArc::xArc = new float[NUM_SEGMENTS];
float *InterSpeciesArc::yArc = new float[NUM_SEGMENTS];

extern int timer;

InterSpeciesArc::InterSpeciesArc(PlotManager *pm, float coefficient, SmallMultiple *source, SmallMultiple *recipient, std::string label) 
 {
    radius = 10;
    thickness = 1;
    highlightThickness = 4;
    highlightColor = &Color::white;

    arcToRight = true;
    displayDynamically = false;
    animated = false;
    fadingAlpha = true;

    startAlpha = 0.6;
    finalAlpha = 0.15;
    signAlpha = 0.75;

    //CW_CODE
	for(int i = 0; i < NUM_SEGMENTS; i++) {
		double a = M_PI * float(i) / (NUM_SEGMENTS - 1);
		xArc[i] = sin(a);
		yArc[i] = -cos(a);
	}

    arrowA = new Triangle();
    arrowA->setBorderColor(new Color(0, 0, 0, .95));
    arrowA->setFillColor(new Color(1, 1, 1, .65));
    arrowA->setSize(10, 10);

    arrowB = new Triangle();
    arrowB->setBorderColor(new Color(0, 0, 0, .95));
    arrowB->setFillColor(new Color(1, 1, 1, .65));
    arrowB->setSize(10, 10);
    
    arrowMiddle = new Triangle();
    arrowMiddle->setBorderColor(new Color(0, 0, 0, .95));
    arrowMiddle->setFillColor(new Color(1, 1, 1, .65));
    arrowMiddle->setSize(10, 10);

    //this->yA = yA;
    //this->yB = yB;
    
    this->plotManager = pm;
    this->source = source;
    this->recipient = recipient;
    this->coefficient = coefficient;
    this->betweenSpeciesLabel = label;
    this->fontHeight = 12;

    adjustType = ADJUST_NONE;
    adjustPercentage = 0.4;
}

InterSpeciesArc::~InterSpeciesArc() {
    delete color;
    delete highlightColor;
    delete arrowA;
    delete arrowB;
    delete arrowMiddle;
}

void InterSpeciesArc::drawToPick() {
    setUpForDrawing();

    if (thickness != 0) {
		glPushMatrix();
			glTranslatef(x, y, 0);

			if (!arcToRight) {
				glRotatef(180.0,0.0,0.0,1.0);
			}

			if (fabs(thickness) < 3) {
				drawToPickAsLineStrips();
			} else {
				drawToPickAsPolygons();
			}
		glPopMatrix();
    }
}

void InterSpeciesArc::drawFaded() {
    float oldStart = startAlpha;
    float oldFinal = finalAlpha;
    float oldSignAlpha = signAlpha;
    startAlpha = 0.3;
    finalAlpha = 0.05;
    signAlpha = 0.3;

    float oldArrowBord = arrowA->getBorderColor()->a;
    float oldArrowFill = arrowA->getFillColor()->a;
    arrowA->getBorderColor()->a = startAlpha;
    arrowA->getFillColor()->a = finalAlpha;
    arrowB->getBorderColor()->a = startAlpha;
    arrowB->getFillColor()->a = finalAlpha;
    arrowMiddle->getBorderColor()->a = startAlpha;
    arrowMiddle->getFillColor()->a = finalAlpha;

    draw();

    startAlpha = oldStart;
    finalAlpha = oldFinal;
    signAlpha = oldSignAlpha;

    arrowA->getBorderColor()->a = oldArrowBord;
    arrowA->getFillColor()->a = oldArrowFill;
    arrowB->getBorderColor()->a = oldArrowBord;
    arrowB->getFillColor()->a = oldArrowFill;
    arrowMiddle->getBorderColor()->a = oldArrowBord;
    arrowMiddle->getFillColor()->a = oldArrowFill;
}

void InterSpeciesArc::draw() {
    setUpForDrawing();

    if (thickness != 0) {
		glPushMatrix();
			glTranslatef(x, y, 0);

			float startA = startAlpha;
			float finalA = finalAlpha;               

			if (!fadingAlpha) {
				startA = color->a;
				finalA = color->a;
			}

			if (!arcToRight) {
				glRotatef(180.0,0.0,0.0,1.0);
			}

			if (selected) {
				drawHighlight();
			}

			if (fabs(thickness) < 3) {
				drawLineArc();
			} else {
				drawPolygonArc();
			}

			drawTriangles();

			if (displayDynamically) {
				drawSigns();
			}
		glPopMatrix();
    }
}

void InterSpeciesArc::setUpForDrawing() {
    if (isArcToRight()) {
        this->x = source->getXLocation() + source->getWidth();
    } else {
        this->x = source->getXLocation();// + source->getOffsetX();
    }

    this->yA = source->getYLocation() + source->getOffsetY() + source->getInnerHeight() / 2;
    this->yB = recipient->getYLocation() + recipient->getOffsetY() + recipient->getInnerHeight() / 2;
    
    adjustPositions();             
    determineCenterLocation();
    determineThickness(); 
    positionTriangles();
}

void InterSpeciesArc::determineThickness() {
    if (displayDynamically) {     
        this->setThickness(getDynamicThickness());        
    } else {        
        this->setThickness(min(30, 30 * getCoefficent()));
    }
}

void InterSpeciesArc::adjustPositions() {
    float adjustOffset = (source->getInnerHeight() / 2) * adjustPercentage;

    if (adjustType == ADJUST_LARGER) {
        if (this->yA > this->yB) {
            this->yA = this->yA + adjustOffset;
            this->yB = this->yB - adjustOffset;
        } else {
            this->yA = this->yA - adjustOffset;
            this->yB = this->yB + adjustOffset;
        }
    } else if (adjustType == ADJUST_SMALLER) {
        if (this->yA > this->yB) {
            this->yA = this->yA - adjustOffset;
            this->yB = this->yB + adjustOffset;
        } else {
            this->yA = this->yA + adjustOffset;
            this->yB = this->yB - adjustOffset;
        }
    }
}

float InterSpeciesArc::getDynamicThickness() {
    float coef = getCoefficient();

    float increaseSource = source->getPercentIncreaseOfFinalValue();
    //float increaseRecipient = speciesB->getPercentIncreaseOfFinalValue();
    
    float finalSource = source->getFinalValue();
    float prevSource = source->getPreviousFinalValue();
    float startRecipient = recipient->getStartValue();
    float finalRecipient = recipient->getFinalValue();

    float recipWeight = 100000.0 / (startRecipient + 100000.0);

    float mult = getDynamicConstant();
    float thick = mult * coef * increaseSource * prevSource * recipWeight * 0.3;
    
    float maxThickness = 30;

    if (thick > maxThickness) {
        return maxThickness;
    } else if (thick < -maxThickness) {
        return -maxThickness;
    } else if (thick < 1 && thick > -1) {
        return 0;
    }

    return thick;
}

void InterSpeciesArc::drawSelected() {
    if (selected) {
        setUpForDrawing();

        float oldStart = startAlpha;
        float oldFinal = finalAlpha;
        float oldSignAlpha = signAlpha;
        startAlpha = 0.4;
        finalAlpha = 0.0;
        signAlpha = 1.0;
        draw();
        startAlpha = oldStart;
        finalAlpha = oldFinal;
        signAlpha = oldSignAlpha;        
      
        std::string label = source->getTitle() + betweenSpeciesLabel + recipient->getTitle() + " (" + toStr(coefficient) + ")";

        float xPos = this->x + radius / 2;

        if (!this->isArcToRight()) {
            xPos = this->x - radius / 2;
        }

        float yPos = this->y;

        glLineWidth(1.0);
        glColor4f(0, 0, 0, 1);
        PrintText::drawStrokeText(label, xPos, yPos, fontHeight, HORIZ_CENTER, VERT_CENTER, true);
    }
}

void InterSpeciesArc::determineCenterLocation() {
    radius = fabs(yA - yB) / 2;
    y = min(yA, yB) + radius;
}


void InterSpeciesArc::drawSigns() {
	float grey = 0.2;
	float signA = signAlpha;
	
	if (selected) {
		grey = 0;
		signAlpha = 1.0;
	}

    if (displayDynamically && animated) {
        for (int i = 0; i < 5; i++) {
            int t = NUM_SEGMENTS-(timer+NUM_SEGMENTS/5*i)%NUM_SEGMENTS-1;
            
            drawSign(xArc[t]*radius, yArc[t]*radius, grey, signA, thickness < 0);
        }
    } else {
        int oneSixth = NUM_SEGMENTS / 6;
        drawSign(xArc[oneSixth]*radius, yArc[oneSixth]*radius, grey, signA, thickness < 0);
        drawSign(xArc[2 * oneSixth]*radius, yArc[2 * oneSixth]*radius, grey, signA, thickness < 0);
        drawSign(xArc[4 * oneSixth]*radius, yArc[4 * oneSixth]*radius, grey, signA, thickness < 0);
        drawSign(xArc[5 * oneSixth]*radius, yArc[5 * oneSixth]*radius, grey, signA, thickness < 0);
    }
}

void InterSpeciesArc::drawHighlight() {    
    float radiusOuter = radius + fabs(thickness / 2);
    float radiusInner = radius - fabs(thickness / 2);
    float highlightOuter = radiusOuter + highlightThickness / 2;
    float highlightInner = radiusInner - highlightThickness / 2;

    glLineWidth(highlightThickness);
    glColor4f( highlightColor->r, highlightColor->g, highlightColor->b, highlightColor->a );   
    
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < NUM_SEGMENTS; i++) { 
        glVertex2f(xArc[i] * highlightOuter, yArc[i] * highlightOuter);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < NUM_SEGMENTS; i++) { 
        glVertex2f(xArc[i] * highlightInner, yArc[i] * highlightInner);
    }
    glEnd();

    glLineWidth(1);
}

void InterSpeciesArc::drawTriangles() {
    if (selected) {
        arrowA->draw();
        arrowB->draw();
    } else {
        arrowMiddle->draw();
    }
}

void InterSpeciesArc::positionTriangles() {
    Color *c = this->getColor();

    arrowA->setBorderColor(c);
    arrowB->setBorderColor(c);    
    arrowMiddle->setBorderColor(c);

    arrowA->setBorderWidth(2);
    arrowB->setBorderWidth(2);
    arrowMiddle->setBorderWidth(1.5);

    float size = max(6, fabs(getThickness() / 2));

    int oneFourth = NUM_SEGMENTS / 4;

    arrowA->setLocation(xArc[oneFourth]*radius, yArc[oneFourth]*radius);
    arrowA->setSize(size, size * 2);
    arrowA->setRotation(135);

    arrowB->setLocation(xArc[3 * oneFourth]*radius, yArc[3 * oneFourth]*radius);
    arrowB->setSize(size, size * 2);
    arrowB->setRotation(225);

    arrowMiddle->setLocation(xArc[2 * oneFourth]*radius, yArc[2 * oneFourth]*radius);
    arrowMiddle->setSize(size, size * 2);
    arrowMiddle->setRotation(180);    
}

void InterSpeciesArc::drawLineArc() {
    glEnable(GL_POLYGON_SMOOTH);
    
    float diff = startAlpha - finalAlpha;
    float alphaStep = diff / NUM_SEGMENTS;

    float mult = 1;

    if (!arcToRight) {
        mult = -1;
    }

    glLineWidth(thickness);

    glBegin(GL_LINE_STRIP);

    for(int i = 0; i < NUM_SEGMENTS; i++) { 
        glColor4f( color->r, color->g, color->b, finalAlpha + i * alphaStep );
        glVertex2f(xArc[i] * radius, yArc[i] * radius);
    }

    glEnd();

    glLineWidth(1);
    glDisable(GL_POLYGON_SMOOTH);
}

void InterSpeciesArc::drawPolygonArc() {
    float radiusOuter = radius + thickness / 2;
    float radiusInner = radius - thickness / 2;

    float diff = startAlpha - finalAlpha;
    float alphaStep = diff / NUM_SEGMENTS;
    
    //CW_CODE

	float timeval;
	timeval = 1.0 - float(timer % NUM_SEGMENTS)/float(NUM_SEGMENTS);

	float tc;

    if (displayDynamically && animated) {
	    glEnable(GL_TEXTURE_2D);
    }   
 
    glPolygonMode(GL_FRONT, GL_FILL);

	glBegin(GL_TRIANGLE_STRIP);
    for(int i = 0; i < NUM_SEGMENTS; i++) { 
        glColor4f( color->r, color->g, color->b, finalAlpha + i * alphaStep);

        tc = 1.5*((NUM_SEGMENTS - float(i))/NUM_SEGMENTS+timeval);

        //CW_CODE 
        //*
        if (displayDynamically && animated) {
            glTexCoord2f(tc, 0.3);
        }

        glVertex2f(xArc[i]*radiusOuter,yArc[i]*radiusOuter);

        if (displayDynamically && animated) {
            glTexCoord2f(tc,0.3);
        }
        
        glVertex2f(xArc[i]*radiusInner, yArc[i]*radiusInner);
    }
    glEnd();

    if (displayDynamically && animated) {
        glDisable(GL_TEXTURE_2D);
    }
}

void InterSpeciesArc::drawToPickAsLineStrips() {
    glEnable(GL_POLYGON_SMOOTH);
        
    glLineWidth(thickness);
    glColor3ub(pickR, pickG, pickB);

    glBegin(GL_LINE_STRIP);
        
    for(int i = 0; i < NUM_SEGMENTS; i++) { 
        glVertex2f(xArc[i] * radius, yArc[i] * radius);
    }

    glEnd();

    glLineWidth(1);
    glDisable(GL_POLYGON_SMOOTH);
}

void InterSpeciesArc::drawToPickAsPolygons() {
    float radiusOuter = radius + thickness / 2;
    float radiusInner = radius - thickness / 2;
    
    glPolygonMode(GL_FRONT, GL_FILL);

    glBegin(GL_TRIANGLE_STRIP);
    glColor3ub(pickR, pickG, pickB);

    for (int i = 0; i < NUM_SEGMENTS; i++) { 
		glVertex2f(xArc[i] * radiusOuter, yArc[i] * radiusOuter);        
        glVertex2f(xArc[i] * radiusInner, yArc[i] * radiusInner);
    }

    glEnd();
}

/*void InterSpeciesArc::drawSelected() {
    if (selected) {
        float oldStart = startAlpha;
        float oldFinal = finalAlpha;
        float oldSignAlpha = signAlpha;
        startAlpha = 0.4;
        finalAlpha = 0.0;
        signAlpha = 1.0;
        draw();
        startAlpha = oldStart;
        finalAlpha = oldFinal;
        signAlpha = oldSignAlpha;
    }
}*/