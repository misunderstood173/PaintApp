#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QtCore>
#include <QtGui>
#include <QLabel>
#include <QUdpSocket>

#include "tool.h"

#define APP_NAME "PaintApp"
#define PIXMAP_INITWIDTH 640
#define PIXMAP_INITHEIGHT 380
#define MAX_PEERS 20

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    QLabel currentPoint;
    void SetCurrentPointLabel();
    QPoint GetCurrentPoint();
    Tool *tool;
    Tool *toolRemote[MAX_PEERS];
    QPen pen;
    QPen penRemote[MAX_PEERS];
    QPixmap *pixmap;
    bool validMove;
    bool pixmapChangeAck;
    QString pendingTool;
    QUdpSocket *socket;
    quint16 port;
    QHostAddress remoteIP;
    bool IsAvailable();
    void SendToPeer(QString string);
    void messageBox(QString title, QString message);
    Tool* instantiateTool(QString ClassName, QPixmap *pixmap, QPen pen);
    void reinstantiateTools();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);
private slots:
    void ReadyRead();
    void on_hSliderSize_valueChanged(int value);
    void on_actionConnect_triggered();
    void on_actionLocal_IP_triggered();
    void on_actionDisconnect_triggered();
    void on_actionSize_triggered();
    void on_actionExit_triggered();
    void on_btnClear_pressed();
    void on_btnEraser_pressed();
    void on_btnPencil_pressed();
    void on_btnLine_pressed();
    void on_btnRectangle_pressed();
    void on_btnEllipse_pressed();
    void on_btnBucket_pressed();
    void on_btnBezier_pressed();
};

#endif // MAINWINDOW_H
