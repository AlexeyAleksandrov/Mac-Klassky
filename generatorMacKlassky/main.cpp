#include "mainwindow.h"
#include "formloadingvectors.h"
#include <QApplication>
#include <QDebug>
#include <QTextCodec>


// утечки памяти!!! после генерации остаётся 1.7 Гб


int main(int argc, char *argv[])
{

    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    w.on_pushButton_loadFuncs_clicked();
//    if(w.needClose)
//    {
//        w.close();
//        return 0;
//    }
//    QApplication::processEvents();
//    w.loadFunctions();
    return a.exec();
}
