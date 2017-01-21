#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"

class MyRectangle : public Shape
{
public:
    MyRectangle(QPixmap *pixmap, QPen pen);
    QString getClassName();
    QString getBaseClassName();
protected:
    void PaintShape(QRect R);
};

#endif // RECTANGLE_H
