#ifndef LOADFUNCTIONS_H
#define LOADFUNCTIONS_H

#include <QObject>
#include <QFile>
#include <QMessageBox>
#include "mdnfmacklassky.h"
#include <QLabel>
#include <QProgressBar>
#include <QList>
#include "hexfunction.h"
#include <QTextStream>
#include <QDebug>
#include <QApplication>
#include "functionsgenerator.h"
#include "ftloader.h"

#define THREAD_COUNT 7  // количество потоков

class loadFunctions : public QObject
{
    Q_OBJECT
public:
    explicit loadFunctions(QObject *parent = nullptr);
    ~loadFunctions();
    bool load(QLabel *flabel, QList<hexFunction> *list, QList<hexFunction> *list_2, bool &fgeneratedFunctions, bool isStarting);


    bool getIsLoading() const;

private:
    void message(QString text, QString title = "Генератор"); // выдать сообщение
    QString getDataFromLineFile(QString line, QString key, bool *ok = nullptr); // возвращает значение из строки по ключу

    QFile *file = NULL;
    QList<hexFunction> *list_local = NULL;
    bool *generatedFunctions = NULL;

    QLabel *label = NULL;
    QProgressBar *progressBar = NULL;

    // для многопоточности
//    const int threadCount = 4;
    functionsGenerator *fgenerator = NULL;
//    QThread **fthread = NULL;
//    QList<QThread> fthread;
    QThread *fthread = NULL;

    ftloader *floader = NULL;

    int from[THREAD_COUNT];
    int to[THREAD_COUNT];
    int min, max;

    int threadFinishedCount = 0;

    int functionsCalculateCount = 0;

    bool isLoading = false;

private slots:
    void onThreadFinished();
//    void updateProgressBar();
    void onLoadFromFileFinished();

signals:

};

#endif // LOADFUNCTIONS_H
