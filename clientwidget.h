#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QFile>
#include <QFileDialog>
#include "serverwidget.h"
namespace Ui {
class ClientWidget;
}

class ClientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientWidget(ServerWidget *sw,QWidget *parent = nullptr);
    ~ClientWidget();
    ServerWidget *sw;


private slots:
    void on_SavePath_clicked();

    void on_Connect_clicked();

private:

    Ui::ClientWidget *ui;
    QTcpSocket *tcpsocket;
    QString savepath;
    QFile file;
    QString filename;
    qint64 filesize;
    qint64 receivesize;

    bool isHead;


};

#endif // CLIENTWIDGET_H
