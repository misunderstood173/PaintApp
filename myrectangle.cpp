#include "myrectangle.h"

MyRectangle::MyRectangle(QPixmap *pixmap, QPen pen):Shape(pixmap, pen)
{
}

QString MyRectangle::getClassName()
{
    return "MyRectangle";
}

QString MyRectangle::getBaseClassName()
{
    return "Shape";
}

void MyRectangle::PaintShape(QRect R)
{
    painter->drawRect(R);
}
