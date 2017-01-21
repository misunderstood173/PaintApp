#include "size.h"
#include "ui_size.h"

Size::Size(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Size)
{
    ui->setupUi(this);

    setFixedSize(size());
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->editWidth->setValidator(new QIntValidator(640, 1920, this));
    ui->editHeight->setValidator(new QIntValidator(380, 1080, this));
}

Size::~Size()
{
    delete ui;
}

int Size::getWidth()
{
    return ui->editWidth->text().toInt();
}

int Size::getHeight()
{
    return ui->editHeight->text().toInt();
}
