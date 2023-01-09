#include "serverwidget.h"
#include "clientwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerWidget w;  
    ClientWidget w2(&w);

    w.show();
    w2.show();

    return a.exec();
}
