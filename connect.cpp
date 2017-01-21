#include "connect.h"
#include "ui_connect.h"

Connect::Connect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Connect)
{
    ui->setupUi(this);
    setFixedSize(size());
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->editPort->setValidator(new QIntValidator(0, 65534, this));
    ui->editRemoteIP->setInputMask( "000.000.000.000" );
}

Connect::~Connect()
{
    delete ui;
}

quint16 Connect::getPort()
{
    return ui->editPort->text().toInt();
}

QHostAddress Connect::getRemoteIP()
{
    return QHostAddress(ui->editRemoteIP->text());
}

