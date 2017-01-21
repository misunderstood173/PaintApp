#include "eraser.h"

Eraser::Eraser(QPixmap *pixmap, QPen pen):Pencil(pixmap, pen)
{
    pen.setColor(Qt::white);
    painter->setPen(pen);
}

Eraser::~Eraser()
{
}

void Eraser::SetPen(QPen pen)
{
    pen.setColor(Qt::white);
    Pencil::SetPen(pen);
}

QString Eraser::getClassName()
{
    return "Eraser";
}

QString Eraser::getBaseClassName()
{
    return "Pencil";
}

void Eraser::beginPaint(QPixmap *pixmap, QPen pen)
{
    pen.setColor(Qt::white);
    Pencil::beginPaint(pixmap, pen);
}
