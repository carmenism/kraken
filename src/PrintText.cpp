#include "PrintText.h"
#include <GL/glut.h>

static GLubyte blackBitmap[16384] = {0x00};

int PrintText::inRawGL = 0;
bool PrintText::compiling = false;
int PrintText::dragAmount = 0;
bool PrintText::dragging = false;
int PrintText::lastTextHeight = 0;
int PrintText::lastTextWidth = 0;

void PrintText::drawStrokeText(std::string text, float x, float y, float h, int horizAlign, int vertAlign, bool whiteBg, float rotate) {
    float scale = h / 119.05;

    float horizOff = 0;
    float vertOff = 0;

    switch (horizAlign) {
        case HORIZ_RIGHT:
            horizOff = strokeWidth(text, h);
            break;
        case HORIZ_CENTER:   
            horizOff = strokeWidth(text, h) / 2;
            break;
        case HORIZ_LEFT:
        default:
            break;
    }

    switch (vertAlign) {
        case VERT_TOP:
            vertOff = h;
            break;
        case VERT_CENTER:   
            vertOff = h / 2;
            break;
        case VERT_BOTTOM:
        default:
            break;
    }

    if (rotate == 90 || rotate == -270) {
        float tmp = vertOff;
        vertOff = horizOff;
        horizOff = -tmp;
    }

    if (rotate == -90 || rotate == 270) {
        float tmp = vertOff;
        vertOff = -horizOff;
        horizOff = tmp;
    }

    glEnable(GL_LINE_SMOOTH);

    glPushMatrix();
        glTranslatef(x - horizOff, y - vertOff, 0);
        glRotatef(rotate, 0, 0, 1);
        glScalef(scale, scale, 1);
    
        if (whiteBg) {
            drawWhiteBackground(text, h, 4);
        }

        for (int i = 0; i < text.size(); i++) {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
        }
    glPopMatrix();

    glDisable(GL_LINE_SMOOTH);
}

void PrintText::drawWhiteBackground(std::string label, float fontHeight, float padding) {
    float scale = fontHeight / 119.05;
    float paddingScaled = padding / scale;

    float currentColor[4];
    glGetFloatv(GL_CURRENT_COLOR, currentColor);

    float h = 119.05;
    float w = PrintText::strokeWidth(label, h);

    glPolygonMode(GL_FRONT, GL_FILL);  
    glColor4f(1.0, 1.0, 1.0, 0.85);

    glRectf(-paddingScaled,        -paddingScaled,
            w + paddingScaled, h + paddingScaled);

    glColor4f(currentColor[0], currentColor[1], currentColor[2], currentColor[3]);
}

float PrintText::strokeWidth(std::string text, float h) {
    int length = 0;
    float scale = h / 119.05;

    for (std::string::size_type i = 0; i < text.size(); i++) {
        length += glutStrokeWidth(GLUT_STROKE_ROMAN, text[i]);
    }

    return scale * length;
}

void PrintText::beginRaw2D() {
    if (inRawGL++) {
        return;
    }

    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
}

void PrintText::endRaw2D() {
    if (--inRawGL) {
        return;
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

bool PrintText::beginRaster2D(float rx, float ry, bool needRaw) {
    GLboolean valid;

    if (needRaw) {
        beginRaw2D();
    }

    glRasterPos3f(rx, ry, -1.0);
    
    if (!compiling) {
        glGetBooleanv(GL_CURRENT_RASTER_POSITION_VALID, &valid);
        
        if (!valid) {
            return false;
        }
    }

    return true;
}

void PrintText::endRaster2D() {
    endRaw2D();
}

bool PrintText::printAt(float rx, float ry, std::string text, bool needRaw, GLboolean shadow, void *font, float alpha) {
    int offsetx, offsety, xOffset = 0, yOffset = 0;
    unsigned int length;
    GLboolean valid;

    if (shadow != 2 && text.empty()) {
        return false;
    }
    
    length = text.size();

    //draw the main portion
    valid = beginRaster2D(rx, ry, needRaw);

    if (!valid) {
        return false; 
    }

    if (shadow) {
        glBitmap(0, 0, 0, 0, (float)(xOffset + 1), (float)(yOffset + 1), NULL);
    } else if (xOffset || yOffset) {
        glBitmap(0, 0, 0, 0, (float)xOffset, (float)yOffset, NULL);
    }

    std::string::size_type i;

    for (i = 0; i < length; i++) {
        glutBitmapCharacter(font, text[i]);
    }

    lastTextWidth = printingWidth(text, font) + 1;
    lastTextHeight = printingHeight(font);

    //draw the shadow, if desired.
    if (shadow == 2) { //draw a box 
        if (blackBitmap[0] == 0x00) {
            memset(blackBitmap, 0xFF, 16384);
        }
        
        glPushAttrib(GL_CURRENT_BIT);
            glColor4f(0.0, 0.0, 0.0, 0.5f);
            glRasterPos3f(rx, ry, -1.0);
            glBitmap(lastTextWidth + lastTextHeight, 9 * lastTextHeight / 8,
                (float)(lastTextHeight / 2),
                (float)(lastTextHeight / 4 - yOffset), 0, 0, blackBitmap);
        glPopAttrib();
    } else if (shadow) {
        glPushAttrib(GL_CURRENT_BIT);
        glColor4f(0.0, 0.0, 0.0, alpha);

        for (offsetx = 0; offsetx <= 2; offsetx += 2) {
            for (offsety = 0; offsety <= 2; offsety += 2) {
                glRasterPos3f(rx, ry, -1.0);
                
                if (offsetx != 0 || offsety != 0 || xOffset != 0 || yOffset != 0) {
                    glBitmap(0, 0, 0, 0, (float)(xOffset + offsetx), (float)(yOffset + offsety), NULL);
                }
                
                for (i = 0; i < length; i++) {
                    glutBitmapCharacter(font, text[i]);
                }
            }
        }

        glPopAttrib();
    }

    if (needRaw) {
        endRaster2D();
    }

    return true;
}

bool PrintText::printCenteredAt(float rx, float ry, std::string text, bool needRaw, GLboolean shadow, void *font, float alpha) {
    float nx = rx - printingWidth(text, font) / 2;

    return printAt(nx, ry, text, needRaw, shadow, font, alpha);
}

bool PrintText::printVerticallyCenteredAt(float rx, float ry, std::string text, bool needRaw, GLboolean shadow, void *font, float alpha) {
    float ny = ry - (printingHeight(font) / 4);

    return printAt(rx, ny, text, needRaw, shadow, font, alpha);
}

bool PrintText::printAlignedRightCenteredAt(float rx, float ry, std::string text, bool needRaw, GLboolean shadow, void *font, float alpha) {
    float nx = rx - printingWidth(text, font);
    float ny = ry - (printingHeight(font) / 4);

    return printAt(nx, ny, text, needRaw, shadow, font, alpha);
}

/*
bool PrintText::printCenteredAt(float rx, float ry, std::string text, bool needRaw,
                                GLboolean shadow, void *font, float alpha) {
    float nx = rx - printingWidth(text, font) / (float) GlutWindow::getCurrent()->getWidth();
   
    return printAt(nx, ry, text, needRaw, shadow, font, alpha);
}*/

int PrintText::printingWidth(std::string text, void *font) {
    int length = 0;

    for (std::string::size_type i = 0; i<text.size(); i++) {
        length += glutBitmapWidth(font, text[i]);
    }

    return length;
}

int PrintText::printingHeight(void *font) {
    int height = 17;
                
    if (font == GLUT_BITMAP_9_BY_15) {
        height = 19;
    }

    if (font == GLUT_BITMAP_8_BY_13) {
        height = 17;
    }

    if (font == GLUT_BITMAP_TIMES_ROMAN_10 
        || font == GLUT_BITMAP_HELVETICA_10) {
        height = 14;
    }

    if (font == GLUT_BITMAP_TIMES_ROMAN_24) {
        height = 28;
    }

    if (font == GLUT_BITMAP_HELVETICA_12) {
        height = 16;
    }

    if (font == GLUT_BITMAP_HELVETICA_18) {
        height = 22;
    }

    return height;
}