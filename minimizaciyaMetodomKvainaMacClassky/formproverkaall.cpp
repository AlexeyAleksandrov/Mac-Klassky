#include "formproverkaall.h"
#include "ui_formproverkaall.h"
#include "math.h"

#define TEXT_OK "ОК!"
#define TEXT_ERROR "Ошибка!"

formProverkaAll::formProverkaAll(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::formProverkaAll)
{
    ui->setupUi(this);
}

formProverkaAll::~formProverkaAll()
{
    delete ui;
}

void formProverkaAll::setData(bool tableOfTrue, bool viborMin, bool skleyka1, bool skleyka2, bool skleyka3, bool kartaPokritiya, bool itogMnf, QString levelOfhard)
{
    int pravilno = 0; // хранит, сколько всего позиций сделано верно
    const int vsegoPoziciy = 7; // хранит сколько всег опозиций вообще
    if(tableOfTrue) // выводим результат проверки таблицы истинности
    {
        ui->label_tableOfTrue_result->setText(TEXT_OK);
        pravilno++; // увеличиввем счётчик правильно выполненных пунктов
    }
    else
    {
        ui->label_tableOfTrue_result->setText(TEXT_ERROR);
    }
    if(viborMin) // выводим результат проверки выбора типа минимизации
    {
        ui->label_viborZnacheniy_result->setText(TEXT_OK);
        pravilno++; // увеличиввем счётчик правильно выполненных пунктов
    }
    else
    {
         ui->label_viborZnacheniy_result->setText(TEXT_ERROR);
    }
    if(skleyka1) // выводим результат проверки склейки 1
    {
        ui->label_skleyka1_result->setText(TEXT_OK);
        pravilno++; // увеличиввем счётчик правильно выполненных пунктов
    }
    else
    {
        ui->label_skleyka1_result->setText(TEXT_ERROR);
    }
    if(skleyka2) // выводим результат проверки склейки 2
    {
        ui->label_skleyka2_result->setText(TEXT_OK);
        pravilno++; // увеличиввем счётчик правильно выполненных пунктов
    }
    else
    {
        ui->label_skleyka2_result->setText(TEXT_ERROR);
    }
    if(skleyka3) // выводим результат проверки склейки 3
    {
        ui->label_skleyka3_result->setText(TEXT_OK);
        pravilno++; // увеличиввем счётчик правильно выполненных пунктов
    }
    else
    {
        ui->label_skleyka3_result->setText(TEXT_ERROR);
    }
    if(kartaPokritiya) // выводим результат проверки карты покрытия
    {
        ui->label_kartaPokritiya_result->setText(TEXT_OK);
        pravilno++; // увеличиввем счётчик правильно выполненных пунктов
    }
    else
    {
        ui->label_kartaPokritiya_result->setText(TEXT_ERROR);
    }
    if(itogMnf)// выводим результат проверки итоговой функции
    {
        ui->label_minNf_result->setText(TEXT_OK);
        pravilno++; // увеличиввем счётчик правильно выполненных пунктов
    }
    else
    {
       ui->label_minNf_result->setText(TEXT_ERROR);
    }
    ui->label_levelOfHard_result->setText(levelOfhard); // выодим уровень сложности
    double procent = (static_cast<double>(pravilno) / static_cast<double>(vsegoPoziciy)) * 100.00; // рассчитываем процент правильных заданий
    procent = round(procent); // округляем процент
    ui->label_resultAll_result->setText(QString::number(procent) + "%"); // выводим процент
}
