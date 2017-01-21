#include "bezier.h"

Bezier::Bezier(QPixmap *pixmap, QPen pen):Shape(pixmap, pen)
{
    nrPoints = 0;
}

bool Bezier::IsComplete()
{
    if(nrPoints == 0) return TRUE;
    return FALSE;
}

QString Bezier::getClassName()
{
    return "Bezier";
}

QString Bezier::getBaseClassName()
{
    return "Shape";
}

void Bezier::mouseReleaseEvent(QPoint pos)
{
    if(nrPoints  == 1) bezier[nrPoints++] = pos;
    else if(nrPoints == 4) nrPoints = 0;
}

void Bezier::mousePressEvent(QPoint pos)
{
    if(nrPoints == 0)
    {
        Shape::mousePressEvent(pos);
        bezier[0] = pos;
    }
    nrPoints++;
    Shape::mouseMoveEvent(pos);
}

void Bezier::PaintShape(QRect R)
{
    if(nrPoints == 1) painter->drawLine(R.topLeft(), R.bottomRight());
    else if(nrPoints == 3)
    {
        bezier[2] = R.bottomRight();
        QPainterPath bezierPath;
        bezierPath.moveTo(bezier[0]);
        bezierPath.quadTo(bezier[2], bezier[1]);
        painter->drawPath(bezierPath);
    }
    else if(nrPoints == 4)
    {
        bezier[3] = R.bottomRight();
        QPainterPath bezierPath;
        bezierPath.moveTo(bezier[0]);
        bezierPath.cubicTo(bezier[2], bezier[3], bezier[1]);
        painter->drawPath(bezierPath);
    }
}
