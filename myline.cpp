#include "myline.h"

MyLine::MyLine(QPixmap *pixmap, QPen pen):Shape(pixmap, pen)
{

}

QString MyLine::getClassName()
{
    return "MyLine";
}

QString MyLine::getBaseClassName()
{
    return "Shape";
}

void MyLine::PaintShape(QRect R)
{
    painter->drawLine(R.topLeft(), R.bottomRight());
}
