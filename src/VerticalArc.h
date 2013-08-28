#include "CenteredArc.h"

class VerticalArc : public CenteredArc {
public:
    VerticalArc(float x, float topY, float bottomY);
    ~VerticalArc();

    void setArcToLeft();
    void setArcToRight();

    bool arcToRight();

    void draw();
protected:
    float yA, yB;
private:
    void setCenterLocation();
};