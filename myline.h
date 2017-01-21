#ifndef MYLINE_H
#define MYLINE_H

#include "shape.h"

class MyLine: public Shape
{
public:
    MyLine(QPixmap *pixmap, QPen pen);
    QString getClassName();
    QString getBaseClassName();
protected:
    void PaintShape(QRect R);
};

#endif // MYLINE_H
