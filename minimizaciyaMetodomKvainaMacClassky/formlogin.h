#ifndef FORMLOGIN_H
#define FORMLOGIN_H

#include <QWidget>
#include "qdebug.h"
#include "qfile.h"
#include "qtextstream.h"
#include "qmessagebox.h"
#include "mainwindow.h"
#include "md5crypter.h"
#include <QTimer>

#define STR_SEPARATOR_1 "\r\n"
#define STR_SEPARATOR_2 "\t"
#define FILE_STUDENTS_NAME "students.mirea"
//#define FILE_STUDENT_WORK "mmkwork.mirea"

namespace Ui {
class FormLogin;
}

class FormLogin : public QWidget
{
    Q_OBJECT

public:
    explicit FormLogin(MainWindow &w, QWidget *parent = nullptr);
    ~FormLogin();

private slots:
    void on_pushButton_accept_clicked();

    void on_comboBox_group_currentIndexChanged(const QString &arg1);

    void on_comboBox_student_currentIndexChanged(const QString &arg1);

    void on_pushButton_load_clicked();

    void closeWindow();

private:
    Ui::FormLogin *ui;

    struct studentStruct
    {
        QString name = ""; // имя
        QString group = ""; // группа
        QString function = ""; // функция
    };
    QList<studentStruct> *students = nullptr;
    QStringList groups; // список групп
    QString currentFunction = ""; // текущая функция, которая будет передана для минимизации

    void message(QString text, int type = 0, QString title = "Минимизация логических функций..."); // выдать сообщение
    MainWindow *mw = nullptr; // указатель на окно минимизации

    QTimer *closetimer = NULL; // таймер закрытия окна

};

#endif // FORMLOGIN_H
