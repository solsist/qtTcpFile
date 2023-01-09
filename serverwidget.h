#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QFileDialog>
#include <QTimer>
#include <QFileInfo>


namespace Ui {
class ServerWidget;
}

class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServerWidget(QWidget *parent = nullptr);
    ~ServerWidget();
    void dealfile();
    Ui::ServerWidget *ui;


private slots:
    void on_ChooseFile_clicked();

    void on_SendFile_clicked();

private:

    QTcpServer *tcpserver;
    QTcpSocket *tcpsocket;
    QTimer timer;

    QFile file;
    QString filename;
    qint64 filesize;
    qint64 sendsize;


};

#endif // SERVERWIDGET_H
