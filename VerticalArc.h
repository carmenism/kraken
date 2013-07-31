#include "CenteredArc.h"

class VerticalArc : public CenteredArc {
public:
    VerticalArc(float x, float topY, float bottomY);
    ~VerticalArc();

    void setArcToLeft();
    void setArcToRight();

    void draw();
private:
    float yA, yB;

    void setCenterLocation();
};