#ifndef _DISPLAYABLE_H
#define _DISPLAYABLE_H

class Displayable {
public:
    Displayable() { display = true; }
    ~Displayable() {}

    bool getDisplay() { return display; }
    void setDisplay(bool d) { display = d; }
    void displayOn() { display = true; }
    void displayOff() { display = false; }
protected:
    bool display;
};

#endif /* _DISPLAYABLE_H */