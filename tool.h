#ifndef TOOL_H
#define TOOL_H

#include <QtGui>

class Tool
{
public:
    Tool();
    Tool(QPixmap *pixmap, QPen pen);
    virtual ~Tool();
    virtual void mouseMoveEvent(QPoint pos) = 0;
    virtual void mousePressEvent(QPoint pos) = 0;
    virtual void mouseReleaseEvent(QPoint pos) = 0;
    virtual QPen getPen();
    virtual void SetPen(QPen pen);
    virtual QString getClassName() = 0;
    virtual QString getBaseClassName() = 0;
    virtual void endPaint();
    virtual void beginPaint(QPixmap *pixmap, QPen pen);
    virtual bool IsComplete();
protected:
    bool complete;
    QPixmap *pixmap;
    QPoint *lastPoint;
    QPainter *painter;
    QPen pen;
};

#endif // TOOL_H
