#ifndef PENCIL_H
#define PENCIL_H

#include "tool.h"

class Pencil : public Tool
{
public:
    Pencil(QPixmap *pixmap, QPen pen);
    ~Pencil();
    void mouseMoveEvent(QPoint pos);
    void mousePressEvent(QPoint pos);
    void mouseReleaseEvent(QPoint pos);
    virtual QString getClassName();
    virtual QString getBaseClassName();
    virtual bool IsComplete();
};

#endif // PENCIL_H
