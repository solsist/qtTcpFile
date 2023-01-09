#include "serverwidget.h"
#include "ui_serverwidget.h"

ServerWidget::ServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWidget)
{
    ui->setupUi(this);
    setWindowTitle("服务器(端口号:8080)");

    ui->ChooseFile->setEnabled(false);
    ui->SendFile->setEnabled(false);


    tcpserver = new QTcpServer(this);
    tcpserver->listen(QHostAddress::Any,8080);

    connect(tcpserver,&QTcpServer::newConnection,
            [=]()
            {
                tcpsocket = tcpserver->nextPendingConnection();
                //获取客户端信息
                QString ip = tcpsocket->peerAddress().toString();
                quint16 port = tcpsocket->peerPort();

                QString str = QString("[%1:%2] 与客户端连接成功").arg(ip).arg(port);
                ui->display->setText(str);
                ui->ChooseFile->setEnabled(true);

                connect(&timer,&QTimer::timeout,
                        [=]()
                        {
                            timer.stop();
                            //发送文件
                            dealfile();

                        }
                        );

            }
            );


}

ServerWidget::~ServerWidget()
{
    delete ui;
}

void ServerWidget::on_ChooseFile_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this,"选择文件","../",tr("mp4(*.mp4);; image(*.jpg *.png)"));
    QFileInfo info(filepath);
    filename = info.fileName();
    filesize = info.size();
    qDebug()<<filename<<filesize;

    sendsize = 0;
    file.setFileName(filepath);
    bool isopen = file.open(QIODevice::ReadOnly);
    if(!isopen)
    {
        qDebug()<<"文件打开失败";
        file.close();
    }
    ui->ChooseFile->setEnabled(false);
    ui->SendFile->setEnabled(true);

}
void ServerWidget::on_SendFile_clicked()
{
    ui->SendFile->setEnabled(false);
    QString head = QString("%1##%2").arg(filename).arg(filesize);
    qint64 len = tcpsocket->write(head.toUtf8().data());
    if(len)
    {
        timer.start(20);

    }
    else
    {
        qDebug()<< "头部信息发送失败";
        file.close();
    }


}

void ServerWidget::dealfile()
{
    int count = 0;
    qint64 len = 0;
    do
    {
        char buf[1024*1024] = {0};
        len = 0;
        //if(!file.isReadable())
            //return;
        len = file.read(buf,sizeof(buf));
        qDebug()<<"read:"<<len;
        len = tcpsocket->write(buf,len);

        qDebug()<<"write:"<<len;
        sendsize += len;
        qDebug()<<"sendsize:"<<sendsize;

        count++;
        qDebug()<<count;


    }while(len > 0 );

    if(sendsize == filesize)
    {
        ui->display->append("文件发送完成");
        qDebug()<<"文件发送完成";
        file.close();

    }






}
