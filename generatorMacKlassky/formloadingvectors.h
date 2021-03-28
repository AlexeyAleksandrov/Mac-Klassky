#ifndef FORMLOADINGVECTORS_H
#define FORMLOADINGVECTORS_H

#include <QWidget>
#include "mdnfmacklassky.h"
#include "qfile.h"
#include "qtextstream.h"
//#include "mainwindow.h"
#include <QCloseEvent>

namespace Ui {
class FormLoadingVectors;
}

class FormLoadingVectors : public QWidget
{
    Q_OBJECT

public:
    explicit FormLoadingVectors(QWidget *parent = nullptr);
    ~FormLoadingVectors();
    bool onStart();

private:
    Ui::FormLoadingVectors *ui;
    struct functionStruct
    {
        QString func16 = ""; // 16й вид функции
        int varsFuncCount = 0; // количество переменных в векторе
        int used = 0;
        int typeMnf = -1;
        bool canCalculateMknf = false;
        bool canCalculateMdnf = false;
        int varsCountMknf = -1;
        int varsCountMdnf = -1;
        int skleykiCountMknf = -1; // количество склеек
        int skleykiCountMdnf = -1; // количество склеек
        int maxtermsCount = -1; // количество макстермов
        int mintermsCount = -1; // количество минтермов
    };
    QList<functionStruct> *list = nullptr;
    mdnfMacKlassky *mdnf = nullptr;
//    MainWindow *m = nullptr;

    void closeEvent(QCloseEvent *event);
    bool closeVar = false; // переменная означающая закртыие окна

};

#endif // FORMLOADINGVECTORS_H
