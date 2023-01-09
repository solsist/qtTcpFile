#include "clientwidget.h"
#include "ui_clientwidget.h"
#include "ui_serverwidget.h"

ClientWidget::ClientWidget(ServerWidget *s,QWidget *parent) :
    QWidget(parent),sw(s),
    ui(new Ui::ClientWidget)
{
    ui->setupUi(this);
    setWindowTitle("客户端");


    tcpsocket = new QTcpSocket(this);
    isHead =true;
    savepath = "../";


    connect(tcpsocket,&QTcpSocket::connected,
            [=]()
            {
                ui->display->setText("与服务器连接成功");

            }
            );
    static int count = 0;
    connect(tcpsocket,&QTcpSocket::readyRead,
            [=]()
            {
                if(isHead)
                {
                    isHead = false;
                    QString head = QString(tcpsocket->readAll());
                    filename = head.section("##",0,0);
                    filesize = head.section("##",1,1).toLongLong();
                    QString str = QString("文件名:%1,大小:%2").arg(filename).arg(filesize);

                    ui->progressBar->setMinimum(0);
                    ui->progressBar->setMaximum(int(filesize));


                    ui->progressBar->setValue(0);
                    ui->display->append(str);


                    receivesize = 0;

                    QString filepath = savepath + filename;
                    file.setFileName(filepath);
                    bool isopen = file.open(QIODevice::WriteOnly);


                }
                else
                {
                    //char buf[1024*1024]={0};

                    count++;

                    qint64 size = 1024*1024;
                    QByteArray array = tcpsocket->read(size);
                    //qDebug()<< array;
                    if(file.isOpen())
                    {
                        qint64 len = file.write(array);

                        receivesize += len;
                        qDebug()<<"receivesize:"<<receivesize;
                        ui->progressBar->setValue(int(receivesize));
                        if(receivesize == filesize)
                        {
                            ui->display->append("文件传输完成");
                            file.close();
                            tcpsocket->disconnectFromHost();
                            tcpsocket->close();
                            filename.clear();
                            receivesize= 0;
                            isHead =true;
                            sw->ui->ChooseFile->setEnabled(true);
                             qDebug()<<count;
                        }

                    }
                    else
                    {

                        qDebug()<<"打开文件失败";
                    }
               }

            }
            );

}

ClientWidget::~ClientWidget()
{
    delete ui;
}

void ClientWidget::on_SavePath_clicked()
{
    savepath = QFileDialog::getExistingDirectory(this,"select a floder","../")+"/";
}

void ClientWidget::on_Connect_clicked()
{
    QString ip = ui->IpLine->text();
    quint16 port = ui->PortLine->text().toUShort();
    tcpsocket->connectToHost(ip,port);
}
