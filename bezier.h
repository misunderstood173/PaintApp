#ifndef BEZIER_H
#define BEZIER_H

#include "shape.h"

class Bezier: public Shape
{
public:
    Bezier(QPixmap *pixmap, QPen pen);
    void mouseReleaseEvent(QPoint pos);
    void mousePressEvent(QPoint pos);
    QString getClassName();
    QString getBaseClassName();
    bool IsComplete();
protected:
    void PaintShape(QRect R);
    QPoint bezier[4];
    int nrPoints;
};

#endif // BEZIER_H
