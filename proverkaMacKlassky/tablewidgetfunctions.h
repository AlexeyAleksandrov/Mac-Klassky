#ifndef TABLEWIDGETFUNCTIONS_H
#define TABLEWIDGETFUNCTIONS_H

#include <QObject>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDebug>

class tableWidgetFunctions : public QObject
{
    Q_OBJECT
public:
    explicit tableWidgetFunctions(QObject *parent = nullptr);

    static void center_text_in_table(QTableWidget *tbw); // центрировать текст в ячейках таблицы
    static void setQStringListToTW(QTableWidget *&tableWidget, QStringList list);
    static void addQStringToTWOneSymwolInItem(QTableWidget *&tableWidget, QString str);
    static void addQStringListToTWOneSymwolInItem(QTableWidget *&tableWidget, QStringList list);
    static void clearTW(QTableWidget *&tableWidget);
    static bool getTWItemText(QTableWidget *&tableWidget, int row, int col, QString &outputString);
    static bool getTWTextList(QTableWidget *&tableWidget, QStringList &outputList);
    static void addRow(QTableWidget *&tableWidget, QStringList rowList, int colorRed = -1, int colorGreen = -1, int colorBlue =-1);
    static void copyTableWidget(QTableWidget *tableWidgetInput, QTableWidget *&tableWidgetOutput, bool skipIdenticalLines);
    static QString getQStringByTableWidget(QTableWidget *tableWidget);
    static void setTWText(QTableWidget *&tableWidget, int row, int col, QString text, QColor *itemColor = nullptr);
    static void deletelastRow(QTableWidget *&tableWidget);
    static void setTableWidgetItemsSize(QTableWidget *&tableWidget, int rowSize, int columnSize);
signals:

};

#endif // TABLEWIDGETFUNCTIONS_H
