#ifndef SHAPE_H
#define SHAPE_H

#include "tool.h"

class Shape: public Tool
{
public:
    Shape(QPixmap *pixmap, QPen pen);
    ~Shape();
    virtual void mouseMoveEvent(QPoint pos);
    virtual void mousePressEvent(QPoint pos);
    virtual void mouseReleaseEvent(QPoint pos);
    virtual QString getClassName();
    virtual QString getBaseClassName();
    virtual bool IsComplete();
protected:
    QPixmap originalPixmap;
    QRect r;
    virtual void PaintShape(QRect R) = 0;
};

#endif // SHAPE_H
