#ifndef HEXFUNCTION_H
#define HEXFUNCTION_H

#include <QObject>

class hexFunction /*: public QObject*/
{
//    Q_OBJECT
public:
//    explicit hexFunction(QObject *parent = nullptr);
    explicit hexFunction();

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

    bool operator ==(hexFunction b);

//    void operator =(hexFunction b);

signals:

};

#endif // HEXFUNCTION_H
