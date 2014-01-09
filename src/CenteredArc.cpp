#include "CenteredArc.h"
#include "Color.h"
#include <math.h>

extern int timer;

CenteredArc::CenteredArc() : Point(0, 0), Pickable() {
    radius = 10;
    thickness = 1;
    startAngle = 3.0 * M_PI / 2.0;
    arcAngle = M_PI;
    color = new Color(0, 0, 0, 0.25);
    num_segments = 360;//CW_CODE

    fadingAlpha = false;
    startAlpha = 0.6;
    finalAlpha = 0.15;

    //CW_CODE
	unsigned int grey;
	int i,j;
	double a;

	xArc = new float[num_segments];
	yArc = new float[num_segments];
	for(i=0;i<num_segments;++i)
	{
		a = M_PI*float(i)/(num_segments-1);
		xArc[i] = sin(a);
		yArc[i] = -cos(a);
	}

	for(i=0;i<64;++i)
		for(j=0;j<64;++j)
		{
			grey = (sin(4.0*3.14159*float(j)/64.0)+1.0)*128.0;

			checker[i][j][0] = grey;
			checker[i][j][1] = grey;
			checker[i][j][2] = grey;
			checker[i][j][3] = 125;		
		}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texName);

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64,64,0, GL_RGBA, GL_UNSIGNED_BYTE, checker);

	glDisable(GL_TEXTURE_2D);
}

CenteredArc::~CenteredArc() {
    delete color;
}

CenteredArc::CenteredArc(float radius, float centerX, float centerY, float startAngle, float arcAngle) 
: Point(centerX, centerY) {
    this->radius = radius;
    this->startAngle = startAngle;
    this->arcAngle = arcAngle;
}

void CenteredArc::draw() {
    glPushMatrix();
        glTranslatef(x, y, 0);

        float startA = startAlpha;
        float finalA = finalAlpha;

        if (!fadingAlpha) {
            startA = color->a;
            finalA = color->a;
        }

        if (fabs(thickness) < 3) {
            drawLineArc(radius, thickness, startAngle, arcAngle, color, startA, finalA);
        } else {
            drawPolygonArc(radius, thickness, startAngle, arcAngle, color, startA, finalA);
        }
        
    glPopMatrix();
}

void CenteredArc::drawLineArc(float x, float y, float radius, float thickness, float startAngle, float arcAngle, Color *color, float startAlpha, float finalAlpha) {
    glPushMatrix();
        glTranslatef(x, y, 0);
        drawLineArc(radius, thickness, startAngle, arcAngle, color, startAlpha, finalAlpha);        
    glPopMatrix();
}

void CenteredArc::drawPolygonArc(float x, float y, float radius, float thickness, float startAngle, float arcAngle, Color *color, float startAlpha, float finalAlpha) {
    glPushMatrix();
        glTranslatef(x, y, 0);
        drawPolygonArc(radius, thickness, startAngle, arcAngle, color, startAlpha, finalAlpha);        
    glPopMatrix();
}

void CenteredArc::drawToPick() {
    glPushMatrix();
        glTranslatef(x, y, 0);


        if (thickness < 5) {
            drawToPickAsLineStrips();
        } else {
            drawToPickAsPolygons();
        }
        
    glPopMatrix();
}

void CenteredArc::drawToPickAsLineStrips() {
    glEnable(GL_POLYGON_SMOOTH);
    float num_segments = 360.0;

    float theta = arcAngle / num_segments; 
    float tangetial_factor = tanf(theta);//calculate the tangential factor 

    float radial_factor = cosf(theta);//calculate the radial factor 

    float xx = radius * cosf(startAngle);//we start at angle = 0 
    float yy = radius * sinf(startAngle); 

    glLineWidth(thickness);

    float prevXX = -1;
    float prevYY = -1;

    for(int ii = 0; ii < num_segments; ii++) 
    { 
        glColor3ub(pickR, pickG, pickB);
        if (prevXX != -1) {
            glBegin(GL_LINE_STRIP);
                glVertex2f(prevXX, prevYY);
                glVertex2f(xx, yy);
            glEnd();
        }

        //calculate the tangential vector 
        //remember, the radial vector is (x, y) 
        //to get the tangential vector we flip those coordinates and negate one of them 

        float tx = -yy; 
        float ty = xx; 

        prevXX = xx;
        prevYY = yy;

        //add the tangential vector 

        xx += tx * tangetial_factor; 
        yy += ty * tangetial_factor; 

        //correct using the radial factor 

        xx *= radial_factor; 
        yy *= radial_factor; 
    }

    glLineWidth(1);
    glDisable(GL_POLYGON_SMOOTH);
}

void CenteredArc::drawToPickAsPolygons() {
    float radiusOuter = radius + thickness / 2;
    float radiusInner = radius - thickness / 2;
    float theta = arcAngle / num_segments; 
    float tangetial_factor = tanf(theta);//calculate the tangential factor 

    float radial_factor = cosf(theta);//calculate the radial factor 
	
    float xxOuter = radiusOuter * cosf(startAngle);//we start at angle = 0 
    float yyOuter = radiusOuter * sinf(startAngle);
    float xxInner = radiusInner * cosf(startAngle);//we start at angle = 0 
    float yyInner = radiusInner * sinf(startAngle);  
    
    float prevXXOuter = -1;
    float prevYYOuter = -1;
    float prevXXInner = -1;
    float prevYYInner = -1;

    glPolygonMode(GL_FRONT, GL_FILL);
    for(int ii = 0; ii < num_segments; ii++) 
    { 
        glColor3ub(pickR, pickG, pickB);
        if (prevXXOuter != -1) {
            glBegin(GL_POLYGON);
                glVertex2f(prevXXOuter, prevYYOuter);
                glVertex2f(xxOuter,     yyOuter);
                glVertex2f(xxInner,     yyInner);
                glVertex2f(prevXXInner, prevYYInner);
            glEnd();
        }
		
        //calculate the tangential vector 
        //remember, the radial vector is (x, y) 
        //to get the tangential vector we flip those coordinates and negate one of them 

        float txOuter = -yyOuter; 
        float tyOuter =  xxOuter; 
        float txInner = -yyInner; 
        float tyInner =  xxInner; 
        
        //add the tangential vector 

        prevXXOuter = xxOuter;
        prevYYOuter = yyOuter;
        prevXXInner = xxInner;
        prevYYInner = yyInner;

        xxOuter += txOuter * tangetial_factor; 
        yyOuter += tyOuter * tangetial_factor; 
        xxInner += txInner * tangetial_factor; 
        yyInner += tyInner * tangetial_factor; 
        
        //correct using the radial factor 
        xxOuter *= radial_factor; 
        yyOuter *= radial_factor; 
        xxInner *= radial_factor; 
        yyInner *= radial_factor; 
    }
}

void CenteredArc::drawSelected() {
    if (selected) {
        float oldStart = startAlpha;
        float oldFinal = finalAlpha;
        startAlpha = 0.4;
        finalAlpha = 0.0;
        draw();
        startAlpha = oldStart;
        finalAlpha = oldFinal;
    }
}

void CenteredArc::drawLineArc(float radius, float thickness, float startAngle, float arcAngle, Color *color, float startAlpha, float finalAlpha) {
    glEnable(GL_POLYGON_SMOOTH);
    float theta = arcAngle / num_segments; 
    float tangetial_factor = tanf(theta);//calculate the tangential factor 

    float radial_factor = cosf(theta);//calculate the radial factor 

    float diff = startAlpha - finalAlpha;
    float alphaStep = diff / num_segments;

    float xx = radius * cosf(startAngle);//we start at angle = 0 
    float yy = radius * sinf(startAngle); 

    glLineWidth(thickness);

    glBegin(GL_LINE_STRIP);

    for(int ii = 0; ii < num_segments; ii++) { 
        glColor4f( color->r, color->g, color->b, startAlpha - ii * alphaStep );
        glVertex2f(xx, yy);
        
        //calculate the tangential vector 
        //remember, the radial vector is (x, y) 
        //to get the tangential vector we flip those coordinates and negate one of them 

        float tx = -yy; 
        float ty = xx; 
        
        //add the tangential vector 

        xx += tx * tangetial_factor; 
        yy += ty * tangetial_factor; 

        //correct using the radial factor 

        xx *= radial_factor; 
        yy *= radial_factor; 
    }

    glEnd();

    glLineWidth(1);
    glDisable(GL_POLYGON_SMOOTH);
}

void CenteredArc::drawPolygonArc(float radius, float thickness, float startAngle, float arcAngle, Color *color, float startAlpha, float finalAlpha) {
    float radiusOuter = radius + thickness / 2;
    float radiusInner = radius - thickness / 2;
    float theta = arcAngle / num_segments; 
    float tangetial_factor = tanf(theta);//calculate the tangential factor 

    float radial_factor = cosf(theta);//calculate the radial factor 
	
    float xxOuter = radiusOuter * cosf(startAngle);//we start at angle = 0 
    float yyOuter = radiusOuter * sinf(startAngle);
    float xxInner = radiusInner * cosf(startAngle);//we start at angle = 0 
    float yyInner = radiusInner * sinf(startAngle);  

    float diff = startAlpha - finalAlpha;
    float alphaStep = diff / num_segments;
    
    //CW_CODE

	float timeval;
	timeval = 1.0 - float(timer%num_segments)/float(num_segments);

	float tc;

	glEnable(GL_TEXTURE_2D);
    glPolygonMode(GL_FRONT, GL_FILL);

	glBegin(GL_TRIANGLE_STRIP);
    for(int ii = 0; ii < num_segments; ii++) { 
        glColor4f( color->r, color->g, color->b, startAlpha - ii * alphaStep);

        tc = 1.5*(float(ii)/num_segments+timeval);

        //CW_CODE 
        //*
            glTexCoord2f(tc, 0.3);
            glVertex2f(xxOuter, yyOuter);
            glTexCoord2f(tc,0.3);
            glVertex2f(xxInner,yyInner);		
        //glEnd();
        //glDisable(GL_TEXTURE_2D);
        //*/
        //calculate the tangential vector 
        //remember, the radial vector is (x, y) 
        //to get the tangential vector we flip those coordinates and negate one of them 

        float txOuter = -yyOuter; 
        float tyOuter =  xxOuter; 
        float txInner = -yyInner; 
        float tyInner =  xxInner; 

        //add the tangential vector 
        xxOuter += txOuter * tangetial_factor; 
        yyOuter += tyOuter * tangetial_factor; 
        xxInner += txInner * tangetial_factor; 
        yyInner += tyInner * tangetial_factor; 

        //correct using the radial factor 
        xxOuter *= radial_factor; 
        yyOuter *= radial_factor; 
        xxInner *= radial_factor; 
        yyInner *= radial_factor; 
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);

    // CW_CODE now draw +s

    glLineWidth(2.0);
    int t;

    glColor3f(0.0,0.0,0.0);
    for(int i=0;i<5;++i) {
        t = num_segments-(timer+num_segments/5*i)%num_segments-1;
        glPushMatrix();
            if(startAngle > 2) glRotatef(180.0,0.0,0.0,1.0);

            glTranslatef(xArc[t]*radius,yArc[t]*radius,0.0);
            glBegin(GL_LINES); 
                glVertex2f(5.0,0.0);	 
                glVertex2f(-5.0,0.0);

                if(thickness < 0) {
                    glVertex2f(0.0,-5.0);
                    glVertex2f(0.0,5.0);	
                }		
            glEnd();
        glPopMatrix();
    }
}