#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //prevents qt.network.ssl: QSslSocket: cannot resolve SSLv2_client_method
//    qputenv("QT_LOGGING_RULES", "qt.network.ssl.warning=false");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
