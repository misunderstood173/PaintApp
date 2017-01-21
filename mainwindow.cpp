#include "mainwindow.h"
#include "connect.h"
#include "size.h"
#include "ui_mainwindow.h"

#include <QColorDialog>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QStringList>

#include "pencil.h"
#include "eraser.h"
#include "shape.h"
#include "myellipse.h"
#include "myrectangle.h"
#include "myline.h"
#include "bucket.h"
#include "bezier.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(size());
    this->statusBar()->setSizeGripEnabled(FALSE);

    socket = NULL;
    validMove = TRUE;
    pixmapChangeAck = TRUE;
    pendingTool = "";

    for(int i = 0; i < MAX_PEERS; i++) toolRemote[i] = NULL;

    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    for(int i = 0; i < MAX_PEERS; i++)
    {
        penRemote[i].setCapStyle(Qt::RoundCap);
        penRemote[i].setJoinStyle(Qt::RoundJoin);
    }

    pixmap = new QPixmap(PIXMAP_INITWIDTH, PIXMAP_INITHEIGHT);
    pixmap->fill(Qt::white);
    ui->labelPaint->setPixmap(*pixmap);

    ui->statusBar->addPermanentWidget(&currentPoint);
    currentPoint.setText("   :        ");
    tool = new Pencil(pixmap, pen);

    QPalette palette;
    palette.setColor(QPalette::Window, tool->getPen().color());
    ui->labelColor->setAutoFillBackground(true);
    ui->labelColor->setPalette(palette);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete tool;
    for(int i = 0; i < MAX_PEERS; i++) delete toolRemote[i];
    delete pixmap;
}

void MainWindow::SetCurrentPointLabel()
{
    QPoint p = GetCurrentPoint();
    QString x = QString::number(p.x()), y = QString::number(p.y());
    currentPoint.setText(x.rightJustified(5, ' ') + "   :" + y.rightJustified(5, ' ') + "     ");
}

void MainWindow::SendToPeer(QString string)
{
    QByteArray Data;
    Data.append(string);
    socket->writeDatagram(Data, remoteIP, port);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() != Qt::LeftButton) return;
    QPoint p = GetCurrentPoint();
    if(validMove && IsAvailable())
    {
        if(pendingTool != "")
        {
            tool = instantiateTool(pendingTool, pixmap, pen);
            pendingTool = "";
        }
        tool->mouseMoveEvent(p);
        ui->labelPaint->setPixmap(*pixmap);
        SetCurrentPointLabel();
        if(socket)
        {
            SendToPeer("Draw," + QString::number(pen.width()) + ","
                       + tool->getPen().color().name() + "," + tool->getClassName() + ","
                       + QString::number(p.x()) + "," + QString::number(p.y()) + ",Move");
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() != Qt::LeftButton) return;
    QPoint p = GetCurrentPoint();
    if(!pixmap->rect().contains(p))
    {
        validMove = FALSE;
        if(ui->labelColor->rect().contains(ui->labelColor->mapFromGlobal(QCursor::pos())))
        {
            QColor c = QColorDialog::getColor(Qt::white, this);

            QPalette palette;
            palette.setColor(QPalette::Window, c);
            ui->labelColor->setAutoFillBackground(true);
            ui->labelColor->setPalette(palette);

            pen.setColor(c);
            if(tool) tool->SetPen(pen);
        }
    }
    else
    {
        validMove = TRUE;
        if(IsAvailable())
        {
            if(pendingTool != "")
            {
                tool = instantiateTool(pendingTool, pixmap, pen);
                pendingTool = "";
            }
            tool->mousePressEvent(p);
            ui->labelPaint->setPixmap(*pixmap);
            if(socket)
            {
                SendToPeer("Draw," + QString::number(pen.width()) + ","
                           + tool->getPen().color().name() + "," + tool->getClassName() + ","
                           + QString::number(p.x()) + "," + QString::number(p.y()) + ",Press");
            }
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() != Qt::LeftButton) return;
    QPoint p = GetCurrentPoint();
    if(validMove && IsAvailable())
    {
        if(pendingTool != "")
        {
            tool = instantiateTool(pendingTool, pixmap, pen);
            pendingTool = "";
        }
        tool->mouseReleaseEvent(p);
        ui->labelPaint->setPixmap(*pixmap);

        if(socket)
        {
            SendToPeer("Draw," + QString::number(pen.width()) + ","
                      + tool->getPen().color().name() + "," + tool->getClassName() + ","
                       + QString::number(p.x()) + "," + QString::number(p.y()) + ",Release");
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    bool done = FALSE;
    switch(event->key())
    {
        case (Qt::Key_Plus):
                if(event->modifiers() & Qt::ControlModifier)
                {
                    pen.setWidth(pen.width() + 1);
                    done = TRUE;
                }
                break;
        case (Qt::Key_Minus):
                if(event->modifiers() & Qt::ControlModifier)
                {
                    if(pen.width() > 1)
                    {
                        pen.setWidth(pen.width() - 1);
                        done = TRUE;
                    }
                }
                break;
    }
    if(done)
    {
        ui->hSliderSize->setValue(pen.width());
        if(tool) tool->SetPen(pen);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, APP_NAME,
                                                                    tr("Are you sure?\n"),
                                                                    QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                    QMessageBox::Yes);
        if (resBtn != QMessageBox::Yes) {
            event->ignore();
        } else {
            if(socket)
            {
                SendToPeer("Peer has disconnected");
                delete socket;
            }
            QApplication::quit();
        }
}

void MainWindow::messageBox(QString title, QString message)
{
    QMessageBox *msg = new QMessageBox();
    msg->setAttribute(Qt::WA_DeleteOnClose, true);
    msg->setWindowModality(Qt::NonModal);
    msg->setWindowFlags(msg->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    msg->setWindowTitle(title);
    msg->setText(message);
    msg->show();
}

Tool *MainWindow::instantiateTool(QString ClassName, QPixmap *pixmap, QPen pen)
{
    Tool *newTool = NULL;
    if(ClassName == "Pencil")
        newTool = new Pencil(pixmap, pen);
    else if(ClassName == "MyRectangle")
        newTool = new MyRectangle(pixmap, pen);
    else if(ClassName == "MyLine")
        newTool = new MyLine(pixmap, pen);
    else if(ClassName == "MyEllipse")
        newTool = new MyEllipse(pixmap, pen);
    else if(ClassName == "Eraser")
        newTool = new Eraser(pixmap, pen);
    else if(ClassName == "Bucket")
        newTool = new Bucket(pixmap, pen);
    else if(ClassName == "Bezier")
        newTool = new Bezier(pixmap, pen);
    return newTool;
}

void MainWindow::ReadyRead()
{
    QByteArray string;
    string.resize(socket->pendingDatagramSize());
    socket->readDatagram(string.data(), string.size());
    QString s;
    QString datagram = string;
    int toolID = 0;
    if(datagram.contains('@'))
    {
        QStringList temp = datagram.split('@');
        QStringList::iterator i = temp.begin();
        s = *i;
        toolID = s.toInt();
        if(++i != temp.end()) datagram = *i;
        else return;
    }

    if(datagram == "Connection OK" || datagram == "Server down" || datagram.endsWith("has connected") || datagram == "Connected to server")
    {
        messageBox("Connection", datagram);
        if(pixmapChangeAck == FALSE) pixmapChangeAck = TRUE;
    }
    else if(datagram.endsWith("disconnected"))
    {
        messageBox("Connection", datagram);
        for(int i = 0; i < MAX_PEERS; i++)
            if(toolRemote[i])
            {
                delete toolRemote[i];
                toolRemote[i] = NULL;
            }
        reinstantiateTools();
    }
    else if(datagram == "Clear")
    {
        validMove = FALSE;
        reinstantiateTools();
        pixmap->fill(Qt::white);
        ui->labelPaint->setPixmap(*pixmap);
        SendToPeer("pixmapChangeAck");
    }
    else if(datagram == "pixmapChangeAck")
    {
        pixmapChangeAck = TRUE;
    }
    else if(datagram.startsWith("Size"))
    {
        QSize newSize;
        s = datagram;
        QStringList query = s.split(' ');
        QStringList::iterator it = query.begin();
        if(++it != query.end()) s = *it;
        newSize.setWidth(s.toInt());
        if(++it != query.end()) s = *it;
        newSize.setHeight(s.toInt());

        SendToPeer("pixmapChangeAck");

        if(newSize == pixmap->size()) return;

        validMove = FALSE;
        QSize newWindowSize(geometry().width() + newSize.width() - pixmap->width()
                            , geometry().height() + newSize.height() - pixmap->height());
        resize(newWindowSize);
        setFixedSize(newWindowSize);
        QPixmap p(newSize);
        p.fill(Qt::white);
        if(tool) tool->endPaint();
        delete pixmap;
        pixmap = new QPixmap(p);
        if(tool) tool->beginPaint(pixmap, pen);
        ui->labelPaint->setPixmap(*pixmap);
        reinstantiateTools();

        messageBox("Size changed",
                   "Peer has changed size to " + QString::number(newSize.width())
                   + "x" + QString::number(newSize.height()));

    }
    else if(datagram.startsWith("Draw"))
    {
        if(!pixmapChangeAck) return;
        if(tool && !tool->IsComplete()) return;
        if(tool) tool->endPaint();

        QString s = datagram;
        QStringList query = s.split(',');
        QStringList::iterator it = query.begin();
        if(++it != query.end()) s = *it;
        penRemote[toolID].setWidth(s.toInt());

        if(++it != query.end()) s = *it;
        QColor c(s);
        penRemote[toolID].setColor(c);
        if(toolRemote[toolID]) toolRemote[toolID]->beginPaint(pixmap, penRemote[toolID]);

        if(++it != query.end()) s = *it;
        if((toolRemote[toolID] && toolRemote[toolID]->getClassName() != s) || !toolRemote[toolID])
        {
            if(toolRemote[toolID]) delete toolRemote[toolID];
            toolRemote[toolID] = instantiateTool(s, pixmap, penRemote[toolID]);
        }

        QPoint point;
        if(++it != query.end()) s = *it;
        point.setX(s.toInt());
        if(++it != query.end()) s = *it;
        point.setY(s.toInt());

        if(++it != query.end()) s = *it;

        if(toolRemote[toolID])
        {
            if(s == "Press") toolRemote[toolID]->mousePressEvent(point);
            else if(s == "Move") toolRemote[toolID]->mouseMoveEvent(point);
            else if(s == "Release") toolRemote[toolID]->mouseReleaseEvent(point);
            toolRemote[toolID]->endPaint();
        }

        ui->labelPaint->setPixmap(*pixmap);
        if(toolRemote[toolID]->IsComplete())
            if(tool)
            {
                s = tool->getClassName();
                if(s != "Pencil" && s != "Eraser")
                {
                    delete tool;
                    tool = instantiateTool(s, pixmap, pen);
                }
                else tool->beginPaint(pixmap, pen);
            }
    }
}


QPoint MainWindow::GetCurrentPoint()
{
    return ui->labelPaint->mapFromGlobal(QCursor::pos());
}

bool MainWindow::IsAvailable()
{
    for(int i = 0; i < MAX_PEERS; i++)
        if(toolRemote[i] && !toolRemote[i]->IsComplete())
            return FALSE;
    return TRUE;
}

void MainWindow::on_hSliderSize_valueChanged(int value)
{
    pen.setWidth(value);
    if(tool) tool->SetPen(pen);
    validMove = FALSE;
}

void MainWindow::on_actionConnect_triggered()
{
    Connect conn;
    if(conn.exec()==QDialog::Accepted)
    {
        if(socket)
        {
            SendToPeer("Peer has disconnected");
            delete socket;
        }

        if(pixmap->size() != QSize(PIXMAP_INITWIDTH, PIXMAP_INITHEIGHT))
        {
            QSize newWindowSize(geometry().width() + PIXMAP_INITWIDTH - pixmap->width()
                                , geometry().height() + PIXMAP_INITHEIGHT - pixmap->height());
            resize(newWindowSize);
            setFixedSize(newWindowSize);
            QPixmap p(QSize(PIXMAP_INITWIDTH, PIXMAP_INITHEIGHT));
            p.fill(Qt::white);
            if(tool) tool->endPaint();
            delete pixmap;
            pixmap = new QPixmap(p);
            if(tool) tool->beginPaint(pixmap, pen);
            ui->labelPaint->setPixmap(*pixmap);
        }
        reinstantiateTools();

        port = conn.getPort();
        remoteIP = conn.getRemoteIP();
        socket = new QUdpSocket(this);
        socket->bind(port);
        connect(socket, SIGNAL(readyRead()), this, SLOT(ReadyRead()));

        SendToPeer("Connection OK");
        SendToPeer("Size " + QString::number(PIXMAP_INITWIDTH) + " " + QString::number(PIXMAP_INITHEIGHT));
        SendToPeer("Clear");
        pixmapChangeAck = FALSE;

        pixmap->fill(Qt::white);
        ui->labelPaint->setPixmap(*pixmap);
    }
}

void MainWindow::on_actionLocal_IP_triggered()
{
    QString addressesString;
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
            addressesString += address.toString() + "\n";
    }

    messageBox("Local IP", addressesString);
}

void MainWindow::on_actionDisconnect_triggered()
{
    QString message;
    if(socket)
    {
        SendToPeer("Peer has disconnected");
        delete socket;
        socket = NULL;
        for(int i = 0; i < MAX_PEERS; i++)
            if(toolRemote[i])
            {
                delete toolRemote[i];
                toolRemote[i] = NULL;
            }
        message = "Disconnected";
    }
    else message = "You are not connected";

    messageBox("Connection", message);
}

void MainWindow::reinstantiateTools()
{
    QString classTool;
    if(tool)
    {
        classTool = tool->getClassName();
        delete tool;
    }
    for(int i = 0; i < MAX_PEERS; i++)
        if(toolRemote[i])
        {
            QString classToolRemote = toolRemote[i]->getClassName();
            delete toolRemote[i];
            toolRemote[i] = instantiateTool(classToolRemote, pixmap, penRemote[i]);
            toolRemote[i]->endPaint();
        }
    if(tool) tool = instantiateTool(classTool, pixmap, pen);
}

void MainWindow::on_actionSize_triggered()
{
    Size size;
    if(size.exec()==QDialog::Accepted)
    {
        QSize newSize;
        newSize.setWidth(size.getWidth());
        newSize.setHeight(size.getHeight());

        if(newSize.width() < PIXMAP_INITWIDTH) newSize.setWidth(PIXMAP_INITWIDTH);
        if(newSize.height() < PIXMAP_INITHEIGHT) newSize.setHeight(PIXMAP_INITHEIGHT);

        if(socket)
            SendToPeer("Size " + QString::number(newSize.width()) + " " + QString::number(newSize.height()));

        if(newSize == pixmap->size()) return;
        validMove = FALSE;
        pixmapChangeAck = FALSE;

        QSize newWindowSize(geometry().width() + newSize.width() - pixmap->width()
                            , geometry().height() + newSize.height() - pixmap->height());
        resize(newWindowSize);
        setFixedSize(newWindowSize);
        QPixmap p(newSize);
        p.fill(Qt::white);
        if(tool) tool->endPaint();
        delete pixmap;
        pixmap = new QPixmap(p);
        if(tool) tool->beginPaint(pixmap, pen);
        ui->labelPaint->setPixmap(*pixmap);
        reinstantiateTools();
    }
}

void MainWindow::on_actionExit_triggered()
{
    if(socket) SendToPeer("Peer has disconnected");
    delete socket;
    QApplication::quit();
}

void MainWindow::on_btnClear_pressed()
{
    validMove = FALSE;
    pixmap->fill(Qt::white);
    ui->labelPaint->setPixmap(*pixmap);

    if(socket)
    {
        SendToPeer("Clear");
        pixmapChangeAck = FALSE;
    }

    reinstantiateTools();
}

void MainWindow::on_btnEraser_pressed()
{
    delete tool;
    tool = NULL;
    pendingTool = "Eraser";
    validMove = FALSE;
}

void MainWindow::on_btnPencil_pressed()
{
    delete tool;
    tool = NULL;
    pendingTool = "Pencil";
    validMove = FALSE;
}

void MainWindow::on_btnLine_pressed()
{
    delete tool;
    tool = NULL;
    pendingTool = "MyLine";
    validMove = FALSE;
}

void MainWindow::on_btnRectangle_pressed()
{
    delete tool;
    tool = NULL;
    pendingTool = "MyRectangle";
    validMove = FALSE;
}

void MainWindow::on_btnEllipse_pressed()
{
    delete tool;
    tool = NULL;
    pendingTool = "MyEllipse";
    validMove = FALSE;
}

void MainWindow::on_btnBucket_pressed()
{
    delete tool;
    tool = NULL;
    pendingTool = "Bucket";
    validMove = FALSE;
}

void MainWindow::on_btnBezier_pressed()
{
    delete tool;
    tool = NULL;
    pendingTool = "Bezier";
    validMove = FALSE;
}
