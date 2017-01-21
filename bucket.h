#ifndef BUCKET_H
#define BUCKET_H

#include "tool.h"

class Bucket:public Tool
{
public:
    Bucket(QPixmap *pixmap, QPen pen);
    void mouseMoveEvent(QPoint pos);
    void mousePressEvent(QPoint pos);
    void mouseReleaseEvent(QPoint pos);
    QString getClassName();
    QString getBaseClassName();
private:
    QImage image;
    QColor targetColor;
    QColor replacementColor;
    void Fill(QPoint p);
};

#endif // BUCKET_H
