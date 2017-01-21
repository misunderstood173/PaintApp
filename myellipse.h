#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "shape.h"

class MyEllipse : public Shape
{
public:
    MyEllipse(QPixmap *pixmap, QPen pen);
    QString getClassName();
    QString getBaseClassName();
protected:
    void PaintShape(QRect R);
};

#endif // ELLIPSE_H
