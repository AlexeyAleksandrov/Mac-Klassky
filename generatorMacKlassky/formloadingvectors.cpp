#include "formloadingvectors.h"
#include "ui_formloadingvectors.h"
#include <qdebug.h>
#include <QMessageBox>

#define VARS_COUNT_IGNORE_ONE 1
#define VARS_COUNT_IGNORE_TWO 2

#define TYPEMF_MKNF 0
#define TYPEMF_MDNF 1
#define TYPEMF_ANY  2


FormLoadingVectors::FormLoadingVectors(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormLoadingVectors)
{
    ui->setupUi(this);
    this->setWindowTitle("Генератор вариантов");
}

FormLoadingVectors::~FormLoadingVectors()
{
    delete ui;
}

bool FormLoadingVectors::onStart()
{
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);
//    QFile *fileStudents = new QFile("students.txt");
//    if(!fileStudents->exists())
//    {
//        QMessageBox::critical(this, "Ошибка!", "Не найден файл со студентами!");
//        delete fileStudents;
//        fileStudents = nullptr;
//        return false;
//    }
    QFile *file = new QFile("vectors.txt"); // создаем файл
    if(file->exists()) // если файл существует
    {
        if(!file->open(QIODevice::ReadOnly))
        {
           QMessageBox::critical(this, "Ошибка!", "Не удалось открыть файл с вариантами!");
           return false;
        }
        bool cont = true; // для поиска всех чисел
        if(cont) // если все функции есть
        {
           return true;
        }
        file->close();
    }

    if(closeVar) // если программа закрывается, функция останавливается
    {
        return false;
    }
//    qDebug() << "1";
    list = new QList<functionStruct>;

    mdnf = new mdnfMacKlassky;

    for (int i=0; i<=65535; i++) // перебираем все значения от 0000 до FFFF
    {
        if(closeVar) // если программа закрывается, функция останавливается
        {
            return false;
        }
        ui->label->setText("Генерация вариантов. Сгенерировано " + QString::number(i, 16).toUpper() + " из " + QString::number(65535, 16).toUpper());
        QString func = QString::number(i, 16).toUpper(); // переводим из 10 в 16 ссч
        if((func.count() == VARS_COUNT_IGNORE_ONE) || (func.count() == VARS_COUNT_IGNORE_TWO)) // если один или 3 символа
        {
            continue; // пропускаем
        }
        if(mdnf != nullptr)
        {
            delete mdnf;
            mdnf = new mdnfMacKlassky();
        }
        else
        {
            mdnf = new mdnfMacKlassky();
        }
        if(mdnf->setFunction(func)) // проверяем, возможно ли посчитать
        {
                functionStruct *function = new functionStruct; // создаем экзампляр функции
                function->varsFuncCount = func.count(); // сохраняем количество переменных
                function->func16 = func; // сохраняем саму фнкцию
//                function->typeMnf = mdnf->getOptimizeType16(func); // получаем оптимальный тип минимизации
                function->canCalculateMknf = mdnf->canCalculateMnf(TYPEMF_MKNF);
                function->canCalculateMdnf = mdnf->canCalculateMnf(TYPEMF_MDNF);

                // формируем параметры МКНФ
                if(function->canCalculateMknf)
                {
                    function->varsCountMknf = mdnf->getVarsCount(TYPEMF_MKNF);
                    function->skleykiCountMknf = mdnf->getSkleykiMaxNumber(TYPEMF_MKNF);
                    function->maxtermsCount = mdnf->getMintermsCount(TYPEMF_MKNF);
                }
                else
                {
                    function->varsCountMknf = -1;
                    function->skleykiCountMknf = -1;
                    function->maxtermsCount = -1;
                }

                // формируем параметры МДНФ
                if(function->canCalculateMdnf)
                {
                    function->varsCountMdnf = mdnf->getVarsCount(TYPEMF_MDNF);
                    function->skleykiCountMdnf = mdnf->getSkleykiMaxNumber(TYPEMF_MDNF);
                    function->mintermsCount = mdnf->getMintermsCount(TYPEMF_MDNF);
                }
                else
                {
                    function->varsCountMdnf = -1;
                    function->skleykiCountMdnf = -1;
                    function->mintermsCount = -1;
                }
                function->used = 0; // ставим, что не использовалась
                list->append(*function); // добавляем функцию в лист
                delete function; // удаляем память
                function = nullptr; // зануляем указатель
            delete mdnf;
            mdnf = nullptr;
        }
        double progress = static_cast<double>(i)*100.0/65535.0; // считаем прогресс
        ui->progressBar->setValue(static_cast<int>(progress)); // выдаём прогресс
        QApplication::processEvents(); // ожидаем заершения работы UI
    }
//    qDebug() << "2";
    int size = list->size(); // получаем количество элементов
//    QFile *file = new QFile("vectors.txt"); // создаем файл
    if(!file->open(QIODevice::WriteOnly))
    {
        return false;
    }
    QTextStream *ts = new QTextStream(file);
    for (int i=0; i<size; i++)
    {
        *ts << "func16 : " << list->at(i).func16 << "\t"
//            << list->at(i).typeMnf << "\t"
            << "canCalculateMknf : " << list->at(i).canCalculateMknf << "\t"
            << "canCalculateMdnf : " << list->at(i).canCalculateMdnf << "\t"
            << "varsCountMknf : " << list->at(i).varsCountMknf << "\t"
            << "varsCountMdnf : " << list->at(i).varsCountMdnf << "\t"
            << "skleykiCountMknf : " << list->at(i).skleykiCountMknf << "\t"
            << "skleykiCountMdnf : " << list->at(i).skleykiCountMdnf << "\t"
            << "maxtermsCount : " << list->at(i).maxtermsCount << "\t"
            << "mintermsCount : " << list->at(i).mintermsCount << "\t"

            << "\r\n";
        if(closeVar) // если программа закрывается, функция останавливается
        {
            delete ts;
            ts = nullptr;
            file->close();
            delete file;
            file = nullptr;
            list->clear();
            delete list;
            list = nullptr;
            return false;
        }
    }
//    qDebug() << "3";
    delete ts;
    ts = nullptr;
    file->close();
    delete file;
    file = nullptr;
    list->clear();
    delete list;
    list = nullptr;
//    this->hide();
    if(closeVar) // если программа закрывается, функция останавливается
    {
        return false;
    }
//    mdnfMacKlassky *tmmk = new mdnfMacKlassky;
//    tmmk->setFunction("100");
//    qDebug() << "Оптимальный тип для 100 : " << tmmk->getOptimizeType16("100");
//    delete tmmk;
//    tmmk = nullptr;
    return true;
}

void FormLoadingVectors::closeEvent(QCloseEvent *event)
{
//    qDebug() << "closeEvent";
    closeVar = true;
    event->ignore();
    this->close();
}
