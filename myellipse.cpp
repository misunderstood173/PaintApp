#include "myellipse.h"

MyEllipse::MyEllipse(QPixmap *pixmap, QPen pen):Shape(pixmap, pen)
{

}

QString MyEllipse::getClassName()
{
    return "MyEllipse";
}

QString MyEllipse::getBaseClassName()
{
    return "Shape";
}

void MyEllipse::PaintShape(QRect R)
{
    painter->drawEllipse(R);
}
