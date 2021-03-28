#include "mainwindow.h"
#include "formlogin.h"
#include <QTextCodec>

#include <QApplication>

int main(int argc, char *argv[])
{

    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    QApplication a(argc, argv);
//    QString function = ""; // функция для минимизации


    MainWindow w;
//    w.setMinimumWidth(564);
//    w.setMaximumWidth(564);
//    w.setMinimumHeight(796);
//    w.setMaximumHeight(796);
    w.setWindowTitle("Минимизация логических функций, заданных векторным способом, методом Куайна Мак-Класски");
    FormLogin f(w);
    f.show();
//    w.show();

    return a.exec();
}
