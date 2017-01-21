#ifndef ERASER_H
#define ERASER_H

#include "pencil.h"

class Eraser: public Pencil
{
public:
    Eraser(QPixmap *pixmap, QPen pen);
    ~Eraser();
    void SetPen(QPen pen);
    QString getClassName();
    QString getBaseClassName();
    void beginPaint(QPixmap *pixmap, QPen pen);
};

#endif // ERASER_H
