#ifndef EXELEDITOR_H
#define EXELEDITOR_H

#include <QObject>
#include <QFile>
#include <QTableWidget>
#include <QAxObject>
#include <QFileDialog>
#include <QMessageBox>


class ExelEditor : public QObject
{
    Q_OBJECT
public:
    explicit ExelEditor(QObject *parent = nullptr);

     QString **readFile(QString fileDiretory, int &rowsCount, int &colsCount);
     void writeFile(QString fileDiretory, QString **text, int rows, int cols);

signals:
    void updateDataText(QString text); // сигнал обновления состояния

};

#endif // EXELEDITOR_H
