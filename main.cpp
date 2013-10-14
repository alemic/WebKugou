#include "webkugouwidget.h"
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    WebKugouWidget w;
    w.show();


    return a.exec();
}
