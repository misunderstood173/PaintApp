#include "shape.h"

Shape::Shape(QPixmap *pixmap, QPen pen):Tool(pixmap, pen)
{
}

Shape::~Shape()
{
}

void Shape::mouseMoveEvent(QPoint pos)
{
    if(originalPixmap.isNull()) return;
    if(lastPoint->isNull()) *lastPoint = pos;
    complete = FALSE;
    painter->end();

    *(pixmap) = originalPixmap;

    painter->begin(pixmap);
    painter->setPen(pen);

    r = QRect(*lastPoint, pos);
    PaintShape(r);
}

void Shape::mousePressEvent(QPoint pos)
{
    complete = FALSE;
    originalPixmap = *pixmap;
    *lastPoint = pos;
}

void Shape::mouseReleaseEvent(QPoint pos)
{
    complete = TRUE;
}

QString Shape::getClassName()
{
    return "Shape";
}

QString Shape::getBaseClassName()
{
    return "Tool";
}

bool Shape::IsComplete()
{
    return complete;
}
