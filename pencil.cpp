#include "pencil.h"

Pencil::Pencil(QPixmap *pixmap, QPen pen):Tool(pixmap, pen)
{
}

Pencil::~Pencil()
{
}


void Pencil::mouseMoveEvent(QPoint pos)
{
    if(lastPoint->isNull()) *lastPoint = pos;
    painter->drawLine(*lastPoint, pos);
    *lastPoint = pos;
}

void Pencil::mousePressEvent(QPoint pos)
{
    *lastPoint = pos;
    painter->drawPoint(*lastPoint);
}

void Pencil::mouseReleaseEvent(QPoint pos)
{

}

QString Pencil::getClassName()
{
    return "Pencil";
}

QString Pencil::getBaseClassName()
{
    return "Tool";
}

bool Pencil::IsComplete()
{
    return TRUE;
}
