#include "bucket.h"

Bucket::Bucket(QPixmap *pixmap, QPen pen):Tool(pixmap, pen)
{

}

void Bucket::mouseMoveEvent(QPoint pos)
{

}

void Bucket::mousePressEvent(QPoint pos)
{
    complete = FALSE;
    image = pixmap->toImage();
    targetColor = image.pixelColor(pos);
    replacementColor = painter->pen().color();
    Fill(pos);

    QPen previousPen = painter->pen();
    painter->end();

    *(pixmap) = QPixmap::fromImage(image);

    painter->begin(pixmap);
    painter->setPen(previousPen);
    complete = TRUE;
}

void Bucket::mouseReleaseEvent(QPoint pos)
{
    complete = TRUE;
}

QString Bucket::getClassName()
{
    return "Bucket";
}

QString Bucket::getBaseClassName()
{
    return "Tool";
}

void Bucket::Fill(QPoint p)
{   /*Flood-fill (node, target-color, replacement-color):
      1. If target-color is equal to replacement-color, return.
      2. If color of node is not equal to target-color, return.
      3. Set Q to the empty queue.
      4. Add node to the end of Q.
      5. While Q is not empty:
      6.     Set n equal to the first element of Q.
      7.     Remove first element from Q.
      8.     If color of n is equal to target-color:
      9.         Set the color of n to replacement-color.
     10.         Add west node to end of Q if color of west is equal to target-color.
     11.         Add east node to end of Q if color of east is equal to target-color.
     12.         Add north node to end of Q if color of north is equal to target-color.
     13.         Add south node to end of Q if color of south is equal to target-color.
     14. Return.*/
    QPoint n, tempP;
    if(targetColor == replacementColor) return;
    if(image.pixelColor(p) != targetColor) return;
    QQueue<QPoint> Q;
    Q.push_back(p);
    while(!Q.empty()){
        n = Q.front();
        Q.pop_front();
        if(image.pixelColor(n) == targetColor){
            image.setPixelColor(n, replacementColor);
            tempP = n + QPoint(-1,  0);
            if(image.rect().contains(tempP) && image.pixelColor(tempP) == targetColor) Q.push_back(tempP);
            tempP = n + QPoint(+1,  0);
            if(image.rect().contains(tempP) && image.pixelColor(tempP) == targetColor) Q.push_back(tempP);
            tempP = n + QPoint( 0, -1);
            if(image.rect().contains(tempP) && image.pixelColor(tempP) == targetColor) Q.push_back(tempP);
            tempP = n + QPoint( 0, +1);
            if(image.rect().contains(tempP) && image.pixelColor(tempP) == targetColor) Q.push_back(tempP);
        }
    }
}
