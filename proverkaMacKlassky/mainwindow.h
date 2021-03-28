#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "logiceditor.h"
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setListToTW(QTableWidget *&tableWidgetInput, QStringList list, int varsCount); // вывести

private slots:
    void on_pushButton_calclulate_clicked();
    void sortSkleiki(QTableWidget *tableWidget);
    void sortSkleiki2X(QTableWidget *skleikiTableWidget);

    void on_pushButton_A_clicked();

    void on_pushButton_notA_clicked();

    void on_pushButton_B_clicked();

    void on_pushButton_C_clicked();

    void on_pushButton_D_clicked();

    void on_pushButton_notB_clicked();

    void on_pushButton_notC_clicked();

    void on_pushButton_notD_clicked();

    void on_pushButton_konunkciya_clicked();

    void on_pushButton_disunkciya_clicked();

    void on_pushButton_skobkaOpen_clicked();

    void on_pushButton_skobkaClose_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_clear_clicked();

//    void on_pushButton_reDrowFormula_clicked();

    void on_pushButton_proverka_clicked(); 
    void on_comboBox_formulas_currentIndexChanged(const QString &arg1);

    void on_radioButton_mknf_clicked();

    void on_radioButton_mdnf_clicked();

    void on_pushButton_addFormula_ToTable_clicked();

    void on_pushButton_saveTable_clicked();

    void on_pushButton_readfromfile_clicked();

    void on_pushButton_proverka_all_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    const QColor redColor = QColor(255, 87, 87); // создаём константу красного цвета
    const QColor greenColor = QColor(180, 255, 180); // создаём константу зелёного цвета
    struct skleyka
    {
        QStringList text;
        int type;
        int size;
    };
    logicEditor *editor = nullptr; // создаем логический редактор

    // работа с редактором формул
    void addSymvol(QString symvol, bool inversiya = false); // добавление символа
    bool autoInput = true; // автоматический ввод синтаксиса формулы

};
#endif // MAINWINDOW_H
