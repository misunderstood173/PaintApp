#include "tool.h"

Tool::Tool()
{
    complete = FALSE;
    pixmap = NULL;
    lastPoint = NULL;
    painter = NULL;
}

Tool::Tool(QPixmap *pixmap, QPen pen)
{
    this->pixmap = pixmap;
    this->pen = pen;
    painter = new QPainter(pixmap);
    painter->setPen(pen);
    lastPoint = new QPoint;
    complete = TRUE;
}

Tool::~Tool()
{
    delete painter;
    delete lastPoint;
}

QPen Tool::getPen()
{
    return pen;
}

void Tool::SetPen(QPen pen)
{
    this->pen = pen;
    if(painter) painter->setPen(pen);
}

void Tool::endPaint()
{
    if(painter) delete painter;
    painter = NULL;
}

void Tool::beginPaint(QPixmap *pixmap, QPen pen)
{
    this->pixmap = pixmap;
    this->pen = pen;
    if(painter) delete painter;
    painter = new QPainter(pixmap);
    painter->setPen(pen);
}

bool Tool::IsComplete()
{
    return complete;
}

