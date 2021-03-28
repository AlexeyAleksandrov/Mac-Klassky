#ifndef FTLOADER_H
#define FTLOADER_H

#include <QObject>
#include <QFile>
#include <QDebug>
#include <QProgressBar>
#include <QList>
#include <QMessageBox>
#include <QLabel>
#include <hexfunction.h>
#include <QApplication>
//#include "crypter.h"
#include "md5crypter.h"


class ftloader : public QObject
{
    Q_OBJECT
public:
    explicit ftloader(QObject *parent = nullptr);
    void setData(QList<hexFunction> *flist, QList<hexFunction> *flist_2, QLabel *flabel);

public slots:
    void run();

signals:
    void finish();

private:
    //    QProgressBar *progressBar = NULL;
    QList<hexFunction> *list = NULL;
    QList<hexFunction> *list_2 = NULL;
    QLabel *label = NULL;

    QString getDataFromLineFile(QString line, QString key, bool *ok = nullptr); // возвращает значение из строки по ключу
    void message(QString text, QString title = "Генератор"); // выдать сообщение


};

#endif // FTLOADER_H
