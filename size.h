#ifndef SIZE_H
#define SIZE_H

#include <QDialog>

namespace Ui {
class Size;
}

class Size : public QDialog
{
    Q_OBJECT

public:
    explicit Size(QWidget *parent = 0);
    ~Size();
    int getWidth();
    int getHeight();

private:
    Ui::Size *ui;
};

#endif // SIZE_H
