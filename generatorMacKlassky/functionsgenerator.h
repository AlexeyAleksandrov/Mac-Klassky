#ifndef FUNCTIONSGENERATOR_H
#define FUNCTIONSGENERATOR_H

#include "mdnfmacklassky.h"
#include "hexfunction.h"
#include <QList>
#include <QThread>
#include <QMutex>
#include <QObject>
#include <QDebug>

typedef QList<hexFunction> funcList;

class functionsGenerator : public QObject
{
    Q_OBJECT
public:
    functionsGenerator(QObject *parent = nullptr);

    void generate(int i, funcList *list, funcList *list_2, funcList *list_local); // генерирует функцию переводя число из 10й в 16ю систему
    void generateBetween(); // тоже самое, но в диапазоне [n; m)
    void setData(int ffrom, int fto, funcList *flist, funcList *flist_2, funcList *flist_local, int &ffunctionsCalculateCount);

public slots:
    void run();

signals:
    void finish();
    void updateProgressBar();

private:
    QMutex fmutex;
    int from;
    int to;
    funcList *list;
    funcList *list_2;
    funcList *list_local;

    int *functionsCalculateCount = NULL;




};


#define VARS_COUNT_EASY     0
#define VARS_COUNT_MEDIUM   3
#define VARS_COUNT_HARD     4

#define TYPEMF_MKNF 0
#define TYPEMF_MDNF 1
#define TYPEMF_ANY  2

#define SKLEYKY_COUNT_ANY       -1
#define MINTERMS_COUNT_ANY      -1
#define LOGICAL_VARS_COUNT_ANY  -1

#define ALLOW_VARS_COUNT_1  3
#define ALLOW_VARS_COUNT_2  4

#define GENERATE_TYPE_FOR_ALL_STUDENTS      0
#define GENERATE_TYPE_FOR_CHOSED_GROUP      1
#define GENERATE_TYPE_FOR_CHOSED_STUDENTS   2

#define CHOSED_COLOR        QBrush(QColor(255, 255, 192))
#define NO_CHOSED_COLOR     QBrush(QColor(255, 255, 255))


#endif // FUNCTIONSGENERATOR_H
