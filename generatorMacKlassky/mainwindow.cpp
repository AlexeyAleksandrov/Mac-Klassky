#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include "qfile.h"
#include "qtextstream.h"
#include "qmessagebox.h"
#include "ctime"
#include <thread>
//#include <crypter.h>

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

#define FILE_PRESETS_URL "presets.txt"

#define COLUMN_GROUP 0
#define COLUMN_FIO 1
#define COLUMN_FUNCTION 2

//#define SHOW_PROGRESS_BAR_ON_GENERATE_FOR_STUDENTS

// пока всё :)


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    srand(time(NULL)); // обнуляем рандом
    this->setWindowTitle("Генератор вариантов Куайна-Мак-Класки");

    ui->radioButton_varsCount_4->setChecked(true);
    ui->radioButton_mintermsCount_any_mdnf->setChecked(true);
    ui->radioButton_mintermsCount_any_mknf->setChecked(true);
    ui->radioButton_skleykiCountRandom_mdnf->setChecked(true);
    ui->radioButton_skleykiCountRandom_mknf->setChecked(true);
    ui->radioButton_varsCountInLastFunc_any_mdnf->setChecked(true);
    ui->radioButton_varsCountInLastFunc_any_mknf->setChecked(true);
    ui->radioButton_generateParam_forAll->setChecked(true);
    on_radioButton_generateParam_forAll_clicked();
    ui->radioButton_loadFromExel->setChecked(true);

    ui->checkBox_fullNoPovtor->setChecked(true);
    on_checkBox_fullNoPovtor_stateChanged(ui->checkBox_fullNoPovtor->isChecked());
    ui->checkBox_noPovtorInGroup->setChecked(false);

    //    ui->pushButton_acceptFilter->setEnabled(false);

    list = new QList<hexFunction>; // создаем лист
    list_2 = new QList<hexFunction>; // создаем лист
    listNoUsedFunc = new QList<hexFunction>;
    list_2_NoUsedFunc = new QList<hexFunction>;

//    ui->progressBar_loadFunctions->setVisible(false);
//    ui->progressBar_loadData->setVisible(false);
//    ui->progressBar_loadData->setValue(0);
    ui->label_loadFunctions->setText("Функции не загружены!");
    ui->label_loadData->setText("Список студентов не загружен!");

    QPalette p = ui->label_loadFunctions->palette();
    p.setColor(QPalette::Button, QColor(255, 75,75));
    ui->label_loadFunctions->setPalette(p);
    ui->label_loadData->setPalette(p);

    QString styleSheet = "background-color: rgb(255, 75, 75);";
    ui->label_loadData->setStyleSheet(styleSheet);
    ui->label_loadFunctions->setStyleSheet(styleSheet);

    loadPresetsFromFile(presetsList, FILE_PRESETS_URL); // загружаем пресеты из файла

    //    mdnfMacKlassky tesmmdnf;
    //    tesmmdnf.setFunction("2681");
    //    qDebug() << "Количество минтермов: " << tesmmdnf.getMintermsCount(1) << "Количество макстермов: " << tesmmdnf.getMintermsCount(0);

//    QTextStream ts(&f_w);
//    ts << tempStr;

}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::on_pushButton_generate_clicked()
//{
//    // получаем все данные
//    int vars = 0, skleyki = 0, typeMf = 0, // данные об логической функции
//            mintermsCountFrom = 0, mintermsCountTo = 0, // данные о минтервах
//            logicalVarsCountFrom = 0, logicalVarsCountTo = 0; // данные об ответе
////    getRandomParametsByUser(vars, skleyki, typeMf, mintermsCountFrom, mintermsCountTo, logicalVarsCountFrom, logicalVarsCountTo);

//    // данные получены



//    QString group = ui->comboBox_group->currentText(); // получаем выбранную группу
////    QString func = randomFunction(vars, skleyki, group); // получаем рандомную функцию
//    QString name = ui->comboBox_student->currentText(); // получаем имя выбранного студента
////    setFunction(name, func); // устанавливаем выбранную функцию
////    ui->lineEdit_outputGenerateFunction->setText(func);

//}

void MainWindow::message(QString text, QString title, int type)
{
    if(type == 0)
    {
        QMessageBox::information(this, title, text);
    }
    else if(type == 1)
    {
        QMessageBox::warning(this, title, text);
    }
    else if(type == 2)
    {
        QMessageBox::critical(this, title, text);
    }
}

//QString MainWindow::randomFunction(int varsCount, int skleykiCount, QString ignoreGroup)
//{
//    if(list == nullptr || list_2 == nullptr)
//    {
//        message("Нет данных о векторах!");
//        return "";
//    }
//    if(list->size() == 0 || list_2->size() == 0)
//    {
//        qDebug() << "Размер: " << list->size();
//        message("Нет данных о векторах!");
//        return "";
//    }

//    QList<hexFunction> *randomList = nullptr; // создаём лист для удобства
//    if(varsCount == 0) // если выбран рандом
//    {
//        if(rand()%2 == 0)
//        {
//            varsCount = VARS_COUNT_MEDIUM;
//        }
//        else
//        {
//            varsCount = VARS_COUNT_HARD;
//        }
//    }
//    if(varsCount == VARS_COUNT_MEDIUM) // если ищём функцию от 2х переменных
//    {
//        randomList = list_2;
//    }
//    else if (varsCount == VARS_COUNT_HARD) // если ищём функцию от 4х переменных
//    {
//        randomList = list;
//    }
//    int n = -1; // номер уже сгенерированной функции
//    n = rand()%(randomList->size()); // берем рандомный номер

//    while (true) // перебераем, пока не дойдём до нужнгой функции
//    {
//        n = rand()%(randomList->size()); // берем рандомный номер
//        if(randomList->at(n).used > 0) // если эта функция уже использовалась
//        {
//            int notUsedCount = getNotUsedCount(varsCount, skleykiCount, ignoreGroup); // получаем количество вариантов, которые можно использовать
//            if(notUsedCount == 0) // если не занятых нет
//            {
//                int minUsedCount = getMinUsedCount(varsCount, skleykiCount, ignoreGroup); // получаем, сколько минимальное количество использвоаний одной функции
//                while(randomList->at(n).used != minUsedCount) // пока не анйдем функцию, которая будет иметь минимальное количество повторений
//                {
//                    n = rand()%(randomList->size()); // берем рандомный номер
//                    while(n<0)
//                    {
//                        n = rand()%(randomList->size()); // берем рандомный номер
//                    }
//                }
//            }
//        }
//        bool notError = true;
//        if(randomList->at(n).varsFuncCount != varsCount)
//        {
//            if(varsCount == 0)
//            {
//                qDebug() << "Не важно количество переменных!";
//                notError = true;
//            }
//            else
//            {
//                qDebug() << "Количество переменных не совпадает";
//                notError = false;
//            }
//        }
//        bool notError2 = true;
//        if(randomList->at(n) != skleykiCount)
//        {
//            if(skleykiCount == 0)
//            {
//                notError2 = true;
//            }
//            else
//            {
//                notError2 = false;
//            }
//        }
//        if(notError && notError2)
//        {
//            break;
//        }
//        qDebug() << "Перебор функции: " << randomList->at(n).func16 << " количество переменных: " << randomList->at(n).varsFuncCount << " количество склеек: " << randomList->at(n).skleykiCount;
//    }
//    return randomList->at(n).func16;
//}

//int MainWindow::getNotUsedCount(int varsCount, int skleykiCount, QString ignoreGroup)
//{
//    int count = 0; // переменная для подсчёта количество совпадений
//    for (int i=0; i<students->size(); i++)
//    {
//        if(students->at(i).group != ignoreGroup)
//        {
//            for (int j=0; j<list->size(); j++)
//            {
//                if(list->at(j).func16 == students->at(i).function) // если функция в спсике совпадает с функцией у студента
//                {
//                    if((list->at(j).func16.count() == varsCount) || (varsCount == 0)) // если количество переменных совпадает
//                    {
//                        if((list->at(j).skleykiCount == skleykiCount) || (skleykiCount == 0)) // если совпадает количество склеек
//                        {
//                            count++; // увеличиваем счётчик
//                        }
//                    }
//                }
//            }
//        }
//    }
//    return count;
//}

//int MainWindow::getMinUsedCount(int varsCount, int skleykiCount, QString ignoreGroup)
//{
//    int min = 999999999; // переменная для подсчёта минимального количества повторений
//    for (int j=0; j<list->size(); j++)
//    {
//        if((list->at(j).func16.count() == varsCount) || (varsCount == 0)) // если количество переменных совпадает
//        {
//            if((list->at(j).skleykiCount == skleykiCount)  || (skleykiCount == 0)) // если совпадает количество склеек
//            {
//                if(list->at(j).used < min) // находим минимум
//                {
//                    if(!isFuncContainsInGroup(list->at(j).func16, ignoreGroup)) // если функция не содержится в этой группе
//                    {
//                        min = list->at(j).used;
//                    }
//                }
//            }
//        }
//    }
//    return min;
//}

bool MainWindow::isFuncContainsInGroup(QString function, QString group)
{
    for (int i=0; i<students->size(); i++)
    {
        if(students->at(i).group == group) // если группа совпадает
        {
            if(students->at(i).function == function) // если функция в этой группе совпадает
            {
                return true;
            }
        }
    }
    return false;
}

void MainWindow::setFunction(QString group, QString studentName, QString function)
{
    for (int i=0; i<students->size(); i++)
    {
        if(students->at(i).group == group)
        {
            if(students->at(i).name == studentName) // если группа совпадает
            {
                // если не работает, отменить
                studentStruct tempStuct = students->at(i); // вытаскиваем объект
                tempStuct.function = function; // задаём функцию
                //                students->insert(i, tempStuct); // возвращаем обратно

                students->replace(i, tempStuct);


//                qDebug() << "Дана функция: " << function;
                //                qDebug() << "Установлена функция: " << students->at(i-1).function;
//                qDebug() << "Установлена функция: " << students->at(i).function;
                //                qDebug() << "Установлена функция: " << students->at(i+1).function;
            }
        }
    }
}

void MainWindow::saveData()
{
    QFile *fileStudents = new QFile(FILE_STUDENTS_NAME);
//    if(!fileStudents->exists())
//    {
//        message("Не найден файл со студентами!");
//        return;
//    }
    if(!fileStudents->open(QIODevice::WriteOnly)) // если не открывается
    {
        message("Не удаётся открыть файл со студентами!");
        return;
    }
//    QTextStream *ts = new QTextStream(fileStudents);
//    for (int i=0; i<students->size(); i++)
//    {
//        *ts << students->at(i).group << "\t"
//            << students->at(i).name << "\t"
//            << students->at(i).function << "\r\n";
////        qDebug() << students->at(i).group
////                 << students->at(i).name
////                 << students->at(i).function;
//    }
//    message("Сохранено!");
//    delete ts;
//    ts = NULL;

    QString text;
    for (int i=0; i<students->size(); i++)
    {
        text.append(students->at(i).group);
        text.append(STR_SEPARATOR_2);
        text.append( students->at(i).name );
        text.append(STR_SEPARATOR_2);
        text.append(students->at(i).function);
        text.append(STR_SEPARATOR_1);
    }
//    QString hash = QString(QCryptographicHash::hash(text.toLatin1(),QCryptographicHash::Md5).toHex());
//    text.append("HASH=");
//    text.append(hash);

    md5crypter::cryptStr(text); // добавляем хеш

    fileStudents->write(text.toUtf8());
    fileStudents->close();
    message("Сохранено!");
}

void MainWindow::saveVariantsToExel()
{
    if(fileExelOutputDir.isEmpty())
    {
        QMessageBox::warning(this, "", "Не выбран файл Exel для сохранения");
        return;
    }

    // получаем данные для Exel
    int col_fio = ui->spinBox_saveExel_columnFIO->value();
    int col_group = ui->spinBox_saveExel_columnGroups->value();
    int col_variant = ui->spinBox_saveExel_columnVariants->value();
    int row_start = ui->spinBox_saveExel_firstLineExel->value();

    QAxObject* excel = new QAxObject("Excel.Application", 0);
    QAxObject* workbooks = excel->querySubObject("Workbooks");
    QAxObject* workbook = workbooks->querySubObject("Open(const QString&)", fileExelOutputDir);
    QAxObject* sheets = workbook->querySubObject("Worksheets");
    QAxObject* sheet = sheets->querySubObject("Item(int)", 1);

    auto setEItem = [sheet](int row, int col, QString itemText) // функция записи в ячейку
    {
        QAxObject *cell = sheet->querySubObject("Cells(int, int)",row, col);
        cell->dynamicCall("SetValue(const QVariant&", itemText);
    };

    for (int i=0; i<students->size(); i++)
    {
        qDebug() << "Вывод в Exel " << i << students->size();
        int row = row_start + i;
        setEItem(row, col_fio, students->at(i).name); // выводим имя студента
        setEItem(row, col_group, students->at(i).group); // выводим группу студента
        setEItem(row, col_variant, students->at(i).function); // выводим вариант студента
    }

    qDebug() << "Закончили вывод";
    workbook->dynamicCall("Save()");
    workbook->dynamicCall("Close()");
    excel->dynamicCall("Quit()");
    delete  excel;
    excel = NULL;
    QMessageBox::information(this, "Запись в exel", "Сохранено!");
}

void MainWindow::setStartUIData()
{
    ui->radioButton_varsCount_2->setText(QString::number(VARS_COUNT_MEDIUM));
    ui->radioButton_varsCount_4->setText(QString::number(VARS_COUNT_HARD));
    //    qDebug() << "a";
    // задём нажатие одного из радиобаттонов как нажато по умолчанию
    //    ui->lineEdit_outputGenerateFunction->setReadOnly(true);
    //    ui->lineEdit_nowFunction->setReadOnly(true);
    ui->lineEdit_mintermsCount_from_mdnf->setEnabled(false);
    ui->lineEdit_mintermsCount_to_mdnf->setEnabled(false);
    ui->lineEdit_varsCountInLastFunc_from_mdnf->setEnabled(false);
    ui->lineEdit_varsCountInLastFunc_to_mdnf->setEnabled(false);

    ui->lineEdit_mintermsCount_from_mknf->setEnabled(false);
    ui->lineEdit_mintermsCount_to_mknf->setEnabled(false);
    ui->lineEdit_varsCountInLastFunc_from_mknf->setEnabled(false);
    ui->lineEdit_varsCountInLastFunc_to_mknf->setEnabled(false);

    ui->radioButton_varsCount_2->setChecked(true);
    ui->radioButton_skleykiCountRandom_mdnf->setChecked(true);
    //    ui->radioButton_typeMin_any_mdnf->setChecked(true);
    ui->radioButton_mintermsCount_any_mdnf->setChecked(true);
    ui->radioButton_varsCountInLastFunc_any_mdnf->setChecked(true);

    ui->radioButton_skleykiCountRandom_mknf->setChecked(true);
    //    ui->radioButton_typeMin_any->setChecked(true);
    ui->radioButton_mintermsCount_any_mknf->setChecked(true);
    ui->radioButton_varsCountInLastFunc_any_mknf->setChecked(true);

    ui->label_mintermsCount_from->setEnabled(false);
    ui->label_mintermsCount_to->setEnabled(false);
    ui->label_varsCountInLastFunc_from->setEnabled(false);
    ui->label_varsCountInLastFunc_to->setEnabled(false);
}

void MainWindow::setLineColor(QTableWidget *tableWidgetInput, int rowNum, QBrush brush)
{
    int rows = tableWidgetInput->rowCount();
    int cols = tableWidgetInput->columnCount();
    if(rowNum >= 0 && rowNum < rows)
    {
        for (int j=0; j<cols; j++) // проходим по всем столбцам
        {
            if(tableWidgetInput->item(rowNum, j) != NULL)
            {
                tableWidgetInput->item(rowNum, j)->setBackground(brush);
            }
            else
            {
                tableWidgetInput->setItem(rowNum, j, new  QTableWidgetItem(""));
                tableWidgetInput->item(rowNum, j)->setBackground(brush);
            }
        }
    }
}

int MainWindow::getVarsCountByUser()
{
//    qDebug() << ui->radioButton_varsCount_2->isChecked();
//    qDebug() << ui->radioButton_varsCount_4->isChecked();
    if(ui->radioButton_varsCount_2->isChecked())
    {
        return VARS_COUNT_MEDIUM;
    }
    else if(ui->radioButton_varsCount_4->isChecked())
    {
        return VARS_COUNT_HARD;
    }
    else
    {
        return -1;
    }
}

int MainWindow::getSkleikyCountByUser(int type)
{
    if(type == TYPEMF_MDNF)
    {
        if(ui->radioButton_skleykiCountRandom_mdnf->isChecked())
        {
            return SKLEYKY_COUNT_ANY;
        }
        else if(ui->radioButton_skleykiCount_1_mdnf->isChecked())
        {
            return 1;
        }
        else if(ui->radioButton_skleykiCount_2_mdnf->isChecked())
        {
            return 2;
        }
        else if(ui->radioButton_skleykiCount_3_mdnf->isChecked())
        {
            return 3;
        }
        else
        {
            return -1;
        }
    }
    else if(type == TYPEMF_MKNF)
    {
        if(ui->radioButton_skleykiCountRandom_mknf->isChecked())
        {
            return SKLEYKY_COUNT_ANY;
        }
        else if(ui->radioButton_skleykiCount_1_mknf->isChecked())
        {
            return 1;
        }
        else if(ui->radioButton_skleykiCount_2_mknf->isChecked())
        {
            return 2;
        }
        else if(ui->radioButton_skleykiCount_3_mknf->isChecked())
        {
            return 3;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
}

//int MainWindow::getTypeMfByUser(int type)
//{
//    if(type == TYPEMF_MDNF)
//    {
//        if(ui->radioButton_typeMin_onlyMknf->isChecked())
//        {
//            return TYPEMF_MKNF;
//        }
//        else if(ui->radioButton_typeMin_onlyMdnf->isChecked())
//        {
//            return TYPEMF_MDNF;
//        }
//        else if(ui->radioButton_typeMin_any->isChecked())
//        {
//            return TYPEMF_ANY;
//        }
//        else
//        {
//            return -1;
//        }
//    }

//}

void MainWindow::getMintermsCountByUser(int type, int &from, int &to)
{
//    qDebug() << "TYPE = " << type;
    if(type == TYPEMF_MDNF)
    {
        //        qDebug() << "=== " << ui->radioButton_mintermsCount_any_mdnf->isChecked() << ui->radioButton_mintermsCount_chose_mdnf->isChecked();
        if(ui->radioButton_mintermsCount_any_mdnf->isChecked()) // если выбрано любое количество
        {
            from = MINTERMS_COUNT_ANY;
            to = MINTERMS_COUNT_ANY;
        }
        else if(ui->radioButton_mintermsCount_chose_mdnf->isChecked()) // если пользователь выбирает значение
        {
            int var_from = 0, var_to = 0;

            if(!getFromToData(ui->lineEdit_mintermsCount_from_mdnf, ui->lineEdit_mintermsCount_to_mdnf, var_from, var_to))
            {
                message("Ошибка в параметрах ввода количества минтермов!", "Ошибка", 1);
                from = MINTERMS_COUNT_ANY;
                to = MINTERMS_COUNT_ANY;
                return;
            }
            from = var_from;
            to = var_to;
        }
        else // если ни один RadioButton не нажат
        {
            from = MINTERMS_COUNT_ANY;
            to = MINTERMS_COUNT_ANY;
        }
    }
    else if(type == TYPEMF_MKNF)
    {
        if(ui->radioButton_mintermsCount_any_mknf->isChecked()) // если выбрано любое количество
        {
            from = MINTERMS_COUNT_ANY;
            to = MINTERMS_COUNT_ANY;
        }
        else if(ui->radioButton_mintermsCount_chose_mknf->isChecked()) // если пользователь выбирает значение
        {
//            qDebug() << "IsCheced!!!";
            int var_from = 0, var_to = 0;
            if(!getFromToData(ui->lineEdit_mintermsCount_from_mknf, ui->lineEdit_mintermsCount_to_mknf, var_from, var_to))
            {
                message("Ошибка в параметрах ввода количества макстермов!", "Ошибка", 1);
                from = MINTERMS_COUNT_ANY;
                to = MINTERMS_COUNT_ANY;
                return;
            }
            from = var_from;
            to = var_to;
        }
        else // если ни один RadioButton не нажат
        {
            from = MINTERMS_COUNT_ANY;
            to = MINTERMS_COUNT_ANY;
        }
    }
    else
    {
        from = MINTERMS_COUNT_ANY;
        to = MINTERMS_COUNT_ANY;
    }
//    qDebug() << "TYPE = " << type << "from = " << from << " to = " << to;
}

void MainWindow::getLogicalVarsCountByUser(int type, int &from, int &to)
{
    if(type == TYPEMF_MDNF)
    {
        if(ui->radioButton_varsCountInLastFunc_any_mdnf->isChecked()) // если выбрано любое количество
        {
            from = LOGICAL_VARS_COUNT_ANY;
            to = LOGICAL_VARS_COUNT_ANY;
        }
        else if(ui->radioButton_varsCountInLastFunc_chose_mdnf) // если пользователь выбирает значение
        {
            int var_from = 0, var_to = 0;
            if(!getFromToData(ui->lineEdit_varsCountInLastFunc_from_mdnf, ui->lineEdit_varsCountInLastFunc_to_mdnf, var_from, var_to))
            {
                message("Ошибка в параметрах ввода количества логических переменных МДНФ!", "Ошибка", 1);
                from = LOGICAL_VARS_COUNT_ANY;
                to = LOGICAL_VARS_COUNT_ANY;
            }
            from = var_from;
            to = var_to;
        }
        else // если не нажат ни один radioButton
        {
            from = LOGICAL_VARS_COUNT_ANY;
            to = LOGICAL_VARS_COUNT_ANY;
        }
    }
    else if(type == TYPEMF_MKNF)
    {
        if(ui->radioButton_varsCountInLastFunc_any_mknf->isChecked()) // если выбрано любое количество
        {
            from = LOGICAL_VARS_COUNT_ANY;
            to = LOGICAL_VARS_COUNT_ANY;
        }
        else if(ui->radioButton_varsCountInLastFunc_chose_mknf) // если пользователь выбирает значение
        {
            int var_from = 0, var_to = 0;
            if(!getFromToData(ui->lineEdit_varsCountInLastFunc_from_mknf, ui->lineEdit_varsCountInLastFunc_to_mknf, var_from, var_to))
            {
                message("Ошибка в параметрах ввода количества логических переменных МКНФ!", "Ошибка", 1);
                from = LOGICAL_VARS_COUNT_ANY;
                to = LOGICAL_VARS_COUNT_ANY;
            }
            from = var_from;
            to = var_to;
        }
        else // если не нажат ни один radioButton
        {
            from = LOGICAL_VARS_COUNT_ANY;
            to = LOGICAL_VARS_COUNT_ANY;
        }
    }
    else // если type нек подходит
    {
        from = LOGICAL_VARS_COUNT_ANY;
        to = LOGICAL_VARS_COUNT_ANY;
    }
}

bool MainWindow::getFromToData(QLineEdit *lineEditFrom, QLineEdit *lineEdit_to, int &from_var, int &to_var)
{
    int from = 0, to = 0; // от и до
    bool flag1 = false, flag2 = false; // флаги

    from = lineEditFrom->text().toInt(&flag1); // переводим От
    to = lineEdit_to->text().toInt(&flag2); // переводим До

    if((!flag1 && !lineEditFrom->text().isEmpty()) || (!flag2 && !lineEdit_to->text().isEmpty())) // если есть ошибки при переводе и строки не пустые
    {
        return false;
    }
    if(lineEditFrom->text().isEmpty())
    {
        from = -1;
    }
    if(lineEdit_to->text().isEmpty())
    {
        to = -1;
    }

    if(to > -1) // если До не любое
    {
        if(to < from) // то проверяем, а не меньше-ли оно, чем от
        {
            return false;
        }
    }

    from_var = from; // передаём значения
    to_var = to;
    return true;
}

bool MainWindow::acceptFilter()
{
    if(!generatedFunctions)
    {
        QMessageBox::warning(this, "Ошибка", "Не загружены функции");
        return false;
    }
    generateFilter filter; // создаю фильтр
    // количество переменных в функции
    filter.vars = getVarsCountByUser();

    // МДНФ
    filter.skleykiMdnf = getSkleikyCountByUser(TYPEMF_MDNF);
    filter.mintermsCountFrom = -1;
    filter.mintermsCountTo = -1;
    getMintermsCountByUser(TYPEMF_MDNF, filter.mintermsCountFrom, filter.mintermsCountTo);
    filter.logicalVarsCountFrom_mdnf = -1;
    filter.logicalVarsCountTo_mdnf = -1;
    getLogicalVarsCountByUser(TYPEMF_MDNF, filter.logicalVarsCountFrom_mdnf, filter.logicalVarsCountTo_mdnf);

    // МКНФ
    filter.skleykiMknf = getSkleikyCountByUser(TYPEMF_MKNF);
    filter.maxtermsCountFrom = -1;
    filter.maxtermsCountTo = -1;
    getMintermsCountByUser(TYPEMF_MKNF, filter.maxtermsCountFrom, filter.maxtermsCountTo);
    filter.logicalVarsCountFrom_mknf = -1;
    filter.logicalVarsCountTo_mknf = -1;
    getLogicalVarsCountByUser(TYPEMF_MKNF, filter.logicalVarsCountFrom_mknf, filter.logicalVarsCountTo_mknf);

//    qDebug() << "varsCount = " << filter.vars;

    // генерируем список для рандома
    //    generateListFunctionBy(varsCount, skleykiCountMdnf, skleykiCountMknf, mintermsCountFrom, mintermsCountTo, maxtermsCountFrom, maxtermsCountTo, logicalVarsCountFrom_mdnf, logicalVarsCountTo_mdnf, logicalVarsCountFrom_mknf, logicalVarsCountTo_mknf);
    generateListFunctionBy(filter);

//        message("Под фильтр применено " + QString::number(filterFunctionsList.size()) + " функций");
    qDebug() << "Под фильтр применено " + QString::number(filterFunctionsList.size()) + " функций";

    return true;
}

//void MainWindow::getRandomParametsByUser(int &vars, int &skleyki, int &typeMf, int &mintermsCountFrom, int &mintermsCountTo, int &logicalVarsCountFrom, int &logicalVarsCountTo)
//{
//    vars = getVarsCountByUser();
//    skleyki = getSkleikyCountByUser();
//    typeMf = getTypeMfByUser();
//    getMintermsCountByUser(mintermsCountFrom, mintermsCountTo);
//    getLogicalVarsCountByUser(logicalVarsCountFrom, logicalVarsCountTo);
//}

void MainWindow::generateListFunctionBy(generateFilter filter)
{
    qDebug() << "Входные параметры:";
    qDebug() << "vars = " << filter.vars;
    qDebug() << "skleykiMdnf = " << filter.skleykiMdnf << "skleykiMknf = " << filter.skleykiMknf;
    qDebug() << "mintermsCountFrom = " << filter.mintermsCountFrom << "mintermsCountTo = " << filter.mintermsCountTo;
    qDebug() << "logicalVarsCountFrom_mdnf = " << filter.logicalVarsCountFrom_mdnf << "logicalVarsCountTo_mdnf = " << filter.logicalVarsCountTo_mdnf;
    qDebug() << "maxtermsCountFrom = " << filter.maxtermsCountFrom << "maxtermsCountTo = " << filter.maxtermsCountTo;
    qDebug() << "logicalVarsCountFrom_mknf = " << filter.logicalVarsCountFrom_mknf << "logicalVarsCountTo_mknf = " << filter.logicalVarsCountTo_mknf;
    int count = 0; // количество возможных значений
    filterFunctionsList.clear();
    QList<hexFunction> allFunc; // список для всех функций, которые могут быть изначально использованы
    if(filter.vars == VARS_COUNT_MEDIUM) // если количество переменных соответствует количеству для среднего уровня
    {
        count = list_2_NoUsedFunc->size();
        if(count == 0) // если в списке не осталось функций
        {
            list_2_NoUsedFunc = list_2; // то сбрасывваем его, закидывая обратно все наши функции
        }
        allFunc = *list_2_NoUsedFunc; // заносим возможные функции
    }
    else if(filter.vars == VARS_COUNT_HARD)
    {
        count = listNoUsedFunc->size();
        if(count == 0) // если в списке не осталось функций
        {
            listNoUsedFunc = list; // то сбрасывваем его, закидывая обратно все наши функции
        }
        allFunc = *listNoUsedFunc; // заносим возможные функции
    }
    count = allFunc.size(); // получаем, сколько всего элементов в списке
    for (int i=0; i<count; i++)
    {

        if(isCorrectMknfFilter(allFunc.at(i), filter) && isCorrectMdnfFilter(allFunc.at(i), filter)) // если функция подходит под фильтры для МКНФ и МДНФ
        {
            filterFunctionsList.append(allFunc.at(i)); // добавляем эту функцию в список
            //            qDebug() << "Функция: " << allFunc.at(i).func16 << "Минтернмы: " << allFunc.at(i).mintermsCount << "макстермы: " << allFunc.at(i).maxtermsCount;
        }
    }

}

void MainWindow::generateFunctionsForStudents(QList<MainWindow::studentStruct> &studentsListParam, bool fullRandomFunctions,  bool noPovtorInGroup, bool &error)
{
    //    bool fullRandomFunctions = false; // пытаться сделать полностью эксклюзивные функции
    //    bool noPovtorInGroup = false; // флаг отсутсвия повторов в группе

    qDebug() << "fullRandomFunctions = " << fullRandomFunctions << "noPovtorInGroup = " << noPovtorInGroup;

    QList<MainWindow::studentStruct> *listOfStudentsFunc = new QList<MainWindow::studentStruct>(studentsListParam); // создаем временный список для функции
    for (int i=0; i<listOfStudentsFunc->size(); i++) // зануляем все функции всех студентов
    {
        studentStruct temp = listOfStudentsFunc->at(i);
        temp.function = "0000";
        listOfStudentsFunc->replace(i, temp);
    }
    if(fullRandomFunctions) // если обязательно полностью эксклюзивные функции
    {
        if(filterFunctionsList.size() < listOfStudentsFunc->size()) // если количество фукций меньше, чем количество студентов
        {
            error = true;
            QMessageBox::warning(this, "Ошибка", "Эксклюзивных функций меньше, чем студентов, которым необходимо сгенерировать функции!");
            return;
        }
        noPovtorInGroup = false;
    }
    QList<hexFunction> *listForRandom = new QList<hexFunction>(filterFunctionsList); // копируем список, чтобы его не генерировать снова
    #ifdef SHOW_PROGRESS_BAR_ON_GENERATE_FOR_STUDENTS
    ui->progressBar_loadFunctions->setVisible(true);
    ui->progressBar_loadFunctions->setMinimum(0);
    ui->progressBar_loadFunctions->setMaximum(listOfStudentsFunc->size()-1);
    ui->progressBar_loadFunctions->setValue(0);
    ui->label_loadFunctions->setText("Генерация вариантов");
    #endif
    QApplication::processEvents();
    for (int i=0; i<listOfStudentsFunc->size(); i++)
    {
        if(listForRandom->size() == 0)
        {
            qDebug() << "В списке закончились функции";
            //            continue;
            // очищаем и заполняем заново
            listForRandom->clear();
            delete listForRandom;
            listForRandom = new QList<hexFunction>(filterFunctionsList);
        }
        int num = rand()%(listForRandom->size()); // генерируем рандомный номер
        hexFunction func = listForRandom->at(num); // вытаскиваем элемент с рандомным номером
        if(noPovtorInGroup) // если не должно быть повторов внутри одной группы
        {
            QString groupName = listOfStudentsFunc->at(i).group; // получаем группу, которую ищем
            bool exist = true; // флаг поиска совпадаений, изначально ставим True, чтобы зайти в цикл
            QList<hexFunction> *tempFuncList = new QList<hexFunction>(*listForRandom); // создаём временный список с функциями, чтобы не мешать основному
            while(exist)
            {
                exist = false; // сбрасываем флаг
                for (const auto &student : qAsConst(*listOfStudentsFunc)) // сначала перебираем всех в текущем списке
                {
                    if(student.function == func.func16 && student.group == groupName)
                    {
                        exist = true;
                        break;
                    }
                }
                if(!exist) // если в локальбном спсике не найдено, проверяем всех в глобальном
                {
                    for (int j=0; j<students->size(); j++) // перебираем всех студентов
                    {
                        bool contains = false;
                        for (const auto &student : qAsConst(*listOfStudentsFunc)) // сначала перебираем всех в текущем списке
                        {
                            if(student.name == students->at(j).name && student.group == groupName) // если этот студент уже есть в спсике на генерацию, игнорирем его
                            {
                                contains = true;
                                break;
                            }
                        }
                        if(!contains) // если его нет в списке на генерацию, проверяем
                        {
                            if(students->at(j).function == func.func16 && students->at(j).group == groupName)    // если функция найдена и группа совпадает
                            {
                                exist = true;
                                break;
                            }
                        }

                    }
                }
                if(exist) // если совпадение найдено и эта функция есть в этой группе, генерируем новую функцию и проверяем её
                {
                    if(tempFuncList->size() == 0)
                    {
                        qDebug() << "Во временном списке закончились функции!";
                        break; // выходим из цикла
                    }
                    int n = rand()%(tempFuncList->size()); // генерируем рандомный номер
                    func = tempFuncList->takeAt(n); // вытаскиваем элемент с рандомным номером
                }
            }
            tempFuncList->clear();
            delete tempFuncList;
            tempFuncList = NULL;
            if(exist) // если не получилось сгенерировать эксклюзивную функцию для данной группы
            {
                error = true;
                QMessageBox::warning(this, "Ошибка", "Эксклюзивных функций меньше, чем студентов в группе, которым необходимо сгенерировать функции!");
                ui->label_loadFunctions->setText("Ошибка генерации");
                return;
            }
            else
            {
                for (int j=0; j<listForRandom->size(); j++) // проходим по всему списку доступных функций
                {
                    if(listForRandom->at(j).func16 == func.func16) // если совпадает функция
                    {
                        num = j; // номер становится равен номеру этой функции
                        break;
                    }
                }
            }

        }

        listForRandom->removeAt(num); // удаляем элемент из списка, как использованный
        QString func_16 = func.func16; // получаем вектор
        studentStruct *stud = new studentStruct; // создаём структуру
        // копируем структуру
        stud->function = func_16; // заменяем функцию
        stud->group = listOfStudentsFunc->at(i).group;
        stud->name = listOfStudentsFunc->at(i).name;
        listOfStudentsFunc->replace(i, *stud); // заменяю структуру
        delete stud;
        stud = nullptr;
        #ifdef SHOW_PROGRESS_BAR_ON_GENERATE_FOR_STUDENTS
        ui->progressBar_loadFunctions->setValue(i);
        QApplication::processEvents();
        #endif
    }

    listForRandom->clear();
    delete listForRandom;
    listForRandom = nullptr;

    studentsListParam = *listOfStudentsFunc; // возвращаем значение списка
    listOfStudentsFunc->clear();
    delete listOfStudentsFunc;
    listOfStudentsFunc = NULL;
    error = false;

    ui->label_loadFunctions->setText("Варианты сгенерированы");
    #ifdef SHOW_PROGRESS_BAR_ON_GENERATE_FOR_STUDENTS
    ui->progressBar_loadFunctions->setVisible(false);
    QApplication::processEvents();
    #endif
}

bool MainWindow::isCorrectSkleykiCount(int skleykiCountFunc, int skleykiCountFilter)
{
    return ((skleykiCountFunc == skleykiCountFilter && skleykiCountFunc != SKLEYKY_COUNT_ANY) // если количество склеек совпадает и оно не равно любому
            || (skleykiCountFilter == SKLEYKY_COUNT_ANY && skleykiCountFunc >= 1)); // или если количество любое, но больше, либо равно 1
}

bool MainWindow::isCorrectMintermsCount(int mintermsCountFunc, int mintermsCountFilterFrom, int mintermsCountFilterTo)
{
    return (        // условие от
                    ((mintermsCountFunc >= mintermsCountFilterFrom && mintermsCountFilterFrom != MINTERMS_COUNT_ANY) // или если их количество >= минимума по условию
                     || mintermsCountFilterFrom == MINTERMS_COUNT_ANY) // или если по условию их количество не важно
                    // условие до
                    && ((mintermsCountFunc <= mintermsCountFilterTo && mintermsCountFilterTo != MINTERMS_COUNT_ANY) // и если их количество <= максимума по условию
                        || mintermsCountFilterTo == MINTERMS_COUNT_ANY) // или если по условию их количество не важно
                    );
}

bool MainWindow::isCorrectLogicalVarsCountCount(int logicalVarsCountFunc, int logicalVarsCountFilterFrom, int logicalVarsCountFilterTo)
{
    return (        // условие от
                    (((logicalVarsCountFunc >= logicalVarsCountFilterFrom && logicalVarsCountFilterFrom != LOGICAL_VARS_COUNT_ANY) // или если количество переменных в МНФ >= минимальному
                      || logicalVarsCountFilterFrom == LOGICAL_VARS_COUNT_ANY)  // или выбрано любое количество от
                     // условие до
                     && ((logicalVarsCountFunc <= logicalVarsCountFilterTo && logicalVarsCountFilterTo != LOGICAL_VARS_COUNT_ANY) // и если количество переменных в МНФ <= максимальному
                         || logicalVarsCountFilterTo == LOGICAL_VARS_COUNT_ANY))    // или выбрано любое количество до
                    );

}

bool MainWindow::isCorrectMknfFilter(hexFunction func, MainWindow::generateFilter filter)
{
    return (func.canCalculateMknf // если вообще можно посчитать МКНФ
            && isCorrectSkleykiCount(func.skleykiCountMknf, filter.skleykiMknf) // количество склеек
            && isCorrectMintermsCount(func.maxtermsCount, filter.maxtermsCountFrom, filter.maxtermsCountTo) // количество макстермов
            && isCorrectLogicalVarsCountCount(func.varsCountMknf, filter.logicalVarsCountFrom_mknf, filter.logicalVarsCountTo_mknf)); // количество логических переменных
}

bool MainWindow::isCorrectMdnfFilter(hexFunction func, MainWindow::generateFilter filter)
{
    return (func.canCalculateMdnf // если вообще можно посчитать МКНФ
            && isCorrectSkleykiCount(func.skleykiCountMdnf, filter.skleykiMdnf) // количество склеек
            && isCorrectMintermsCount(func.mintermsCount, filter.mintermsCountFrom, filter.mintermsCountTo) // количество макстермов
            && isCorrectLogicalVarsCountCount(func.varsCountMdnf, filter.logicalVarsCountFrom_mdnf, filter.logicalVarsCountTo_mdnf)); // количество логических переменных
}

void MainWindow::useFunc(QString func_16, QList<hexFunction> *&listInput)
{
    for (int i=0; i<listInput->size(); i++)
    {
        if(listInput->at(i).func16 == func_16)
        {
            listInput->removeAt(i);
            return;
        }
    }
}

bool MainWindow::isEqval(hexFunction function_struct1, hexFunction function_struct2)
{
    // ФУНКЦИЯ НЕДОПИСАНА!!!!!
    if(function_struct1.canCalculateMdnf == function_struct2.canCalculateMdnf
            && function_struct1.canCalculateMknf == function_struct2.canCalculateMknf
            && function_struct1.func16 == function_struct2.func16
            && function_struct1.maxtermsCount == function_struct2.maxtermsCount
            && function_struct1.mintermsCount == function_struct2.mintermsCount
            && function_struct1.skleykiCountMdnf == function_struct2.skleykiCountMdnf
            && function_struct1.skleykiCountMknf == function_struct2.skleykiCountMknf
            && function_struct1.typeMnf == function_struct2.typeMnf
            && function_struct1.varsCountMdnf == function_struct2.varsCountMdnf
            && function_struct1.varsCountMknf == function_struct2.varsCountMknf
            )
    {

    }
    return false;
}

QString MainWindow::getDataFromLineFile(QString line, QString key, bool *ok)
{
    QStringList lineList =  line.split("\t", Qt::SplitBehavior(Qt::SkipEmptyParts)); // разбиваем строку
    for (int i=0; i<lineList.size(); i++)
    {
        QStringList list = lineList.at(i).split(":", Qt::SplitBehavior(Qt::SkipEmptyParts)); // разбиваем каждую пару на ключ : значение
        // получаем значение ключа, удаляем проблемы
        QString searchKey = key.remove(" ");
        QString listKey = list.at(0);
        listKey.remove(" ");
        QString textBykey = list.at(1);
        textBykey.remove(" ");
        if(listKey == searchKey) // если ключ совпадает
        {
            if(ok != nullptr)
            {
                *ok = true;
            }
            return textBykey; // возвращаем значение, соответствующее данному ключу (типа JSON на минималках)
        }
    }
    if(ok != nullptr)
    {
        *ok = false;
    }
    return "";
}

QList<MainWindow::studentStruct> MainWindow::getStudentsByGroup(QString group)
{
    QList<studentStruct> groupList; // список, который будет хранить всех студентов из данной группы
    if(students != NULL)
    {
        for(const auto &student : qAsConst(*students)) // перебираем всех студентов
        {
//            qDebug() << "Студент: " + student.name + " группа: " + student.group;
            if(student.group == group) // если группа совпадает
            {
                groupList.append(student); // добавляем
//                qDebug() << "СОВПАДАЕТ! Студент: " + student.name + " группа: " + student.group + " Искомая группа: " + group;
            }
        }
    }
    return groupList;
}

MainWindow::studentStruct MainWindow::getStudentBy(QString fio, QString group, QString function)
{
    for (auto student : qAsConst(*students))
    {
        if(student.name == fio && student.group == group && student.function == function)
        {
            return student;
        }
    }
    return studentStruct();
}

MainWindow::studentStruct MainWindow::getStudentBy(QString fio, QString group)
{
    for (auto student : qAsConst(*students))
    {
        if(student.name == fio && student.group == group)
        {
            return student;
        }
    }
    return studentStruct();
}

MainWindow::studentStruct MainWindow::getStudentBy(QString function)
{
    for (auto student : qAsConst(*students))
    {
        if(student.function == function)
        {
            return student;
        }
    }
    return studentStruct();
}

void MainWindow::loadPresetsFromFile(QList<presets> &presetsList, QString fileURL)
{
    QFile file(fileURL);
    if(!file.exists())
    {
//        message("Не найден файл с пресетами");
        qDebug() << "Не найден файл с пресетами";
        return;
    }
    if(!file.open(QIODevice::ReadOnly))
    {
//        message("Не удалось открыть файл с пресетами");
        qDebug() << "Не удалось открыть файл с пресетами";
        return;
    }
    bool okmd5 = false;
    QString text = file.readAll(); // считываем весь файл
    md5crypter::decryptStr(text, okmd5); // проверяем хеш
    if(!okmd5)
    {
        qDebug() << "Не удалось открыть файл с пресетами. Хеш повреждён";
        return;
    }
    file.close(); // закрываем файл
    QStringList textList = text.split("\r\n", Qt::SplitBehavior(Qt::SkipEmptyParts)); // разбиваенм на строки
    presetsList.clear(); // очищаем предыдущие пресеты, если они есть
    for (const auto &line : qAsConst(textList)) // проходим по всем строкам
    {
        QString name = getDataFromLineFile(line, "name");
        int varsFuncCount = getDataFromLineFile(line, "varsFuncCount").toInt();

        int varsCountMknfFrom = getDataFromLineFile(line, "varsCountMknfFrom").toInt();
        int varsCountMknfTo = getDataFromLineFile(line, "varsCountMknfTo").toInt();

        int varsCountMdnfFrom = getDataFromLineFile(line, "varsCountMdnfFrom").toInt();
        int varsCountMdnfTo = getDataFromLineFile(line, "varsCountMdnfTo").toInt();

        int skleykiCountMknf = getDataFromLineFile(line, "skleykiCountMknf").toInt();
        int skleykiCountMdnf = getDataFromLineFile(line, "skleykiCountMdnf").toInt();

        int maxtermsCountFrom = getDataFromLineFile(line, "maxtermsCountFrom").toInt();
        int maxtermsCountTo = getDataFromLineFile(line, "maxtermsCountTo").toInt();

        int mintermsCountFrom = getDataFromLineFile(line, "mintermsCountFrom").toInt();
        int mintermsCountTo = getDataFromLineFile(line, "mintermsCountTo").toInt();

        int generateType = getDataFromLineFile(line, "generateType").toInt();

        bool fullNoPovtor = getDataFromLineFile(line, "fullNoPovtor").toInt();
        bool noPovtorInGroup = getDataFromLineFile(line, "noPovtorInGroup").toInt();

        presets localPreset(name, varsFuncCount, varsCountMknfFrom, varsCountMknfTo, varsCountMdnfFrom, varsCountMdnfTo, skleykiCountMknf, skleykiCountMdnf, maxtermsCountFrom, maxtermsCountTo, mintermsCountFrom, mintermsCountTo, generateType, fullNoPovtor, noPovtorInGroup);
        presetsList.append(localPreset); // добавляем пресет
    }
    updatePresets();
}

void MainWindow::addPresets(QList<presets> &presetsList, presets addingPreset)
{
    presetsList.append(addingPreset);
}

void MainWindow::removePreset(QList<presets> &presetsList, presets removingPreset)
{
    for (auto &&preset : presetsList)
    {
        if(preset == removingPreset)
        {
            presetsList.removeOne(preset);
        }
    }
}

void MainWindow::clearPresets(QList<presets> &presetsList)
{
    presetsList.clear();
}

void MainWindow::savePresetsToFile(QList<presets> &presetsList, QString fileURL)
{
//    QStringList outputDataList;
    QString outputData;
    for (auto &&preset : presetsList)
    {
        QString line;
        line.append("name : " + preset.getName() + STR_SEPARATOR_2);
        line.append("varsFuncCount : " + QString::number(preset.getVarsFuncCount()) + STR_SEPARATOR_2);
        line.append("varsCountMknfFrom : " + QString::number(preset.getVarsCountMknfFrom()) + STR_SEPARATOR_2);
        line.append("varsCountMknfTo : " + QString::number(preset.getVarsCountMknfTo()) + STR_SEPARATOR_2);
        line.append("varsCountMdnfFrom : " + QString::number(preset.getVarsCountMdnfFrom()) + STR_SEPARATOR_2);
        line.append("varsCountMdnfTo : " + QString::number(preset.getVarsCountMdnfTo()) + STR_SEPARATOR_2);
        line.append("skleykiCountMknf : " + QString::number(preset.getSkleykiCountMknf()) + STR_SEPARATOR_2);
        line.append("skleykiCountMdnf : " + QString::number(preset.getSkleykiCountMdnf()) + STR_SEPARATOR_2);
        line.append("maxtermsCountFrom : " + QString::number(preset.getMaxtermsCountFrom()) + STR_SEPARATOR_2);
        line.append("maxtermsCountTo : " + QString::number(preset.getMaxtermsCountTo()) + STR_SEPARATOR_2);
        line.append("mintermsCountFrom : " + QString::number(preset.getMintermsCountFrom()) + STR_SEPARATOR_2);
        line.append("mintermsCountTo : " + QString::number(preset.getMintermsCountTo()) + STR_SEPARATOR_2);
        line.append("generateType : " + QString::number(preset.getGenerateType()) + STR_SEPARATOR_2);
        line.append("fullNoPovtor : " + QString::number(preset.getFullNoPovtor()) + STR_SEPARATOR_2);
        line.append("noPovtorInGroup : " + QString::number(preset.getNoPovtorInGroup()) + STR_SEPARATOR_2);

//        outputDataList.append(line);
        outputData.append(line);
        outputData.append(STR_SEPARATOR_1);
    }
    qDebug() << "preset data: " << outputData;
    QFile fileOutput(fileURL);
    if(!fileOutput.open(QIODevice::WriteOnly))
    {
        message("Не удалось открыть файл для записи пресетов!");
        return;
    }
    md5crypter::cryptStr(outputData); // добавляем хеш
    fileOutput.write(outputData.toUtf8()); // выводим в файл
//    QTextStream ts(&fileOutput);
//    for (auto &&line : outputDataList)
//    {
//        ts << line << "\r\n";
//    }
    fileOutput.close();
}

presets MainWindow::getPresetByUI()
{
    presets presetUI;
    presetUI.setName("presetName");
    presetUI.setVarsFuncCount(getVarsCountByUser());

    int from = -1, to = -1;
    getLogicalVarsCountByUser(TYPEMF_MKNF, from, to);
    presetUI.setVarsCountMknfFrom(from);
    presetUI.setVarsCountMknfTo(to);

    getLogicalVarsCountByUser(TYPEMF_MDNF, from, to);
    presetUI.setVarsCountMdnfFrom(from);
    presetUI.setVarsCountMdnfTo(to);

    presetUI.setSkleykiCountMknf(getSkleikyCountByUser(TYPEMF_MKNF));
    presetUI.setSkleykiCountMdnf(getSkleikyCountByUser(TYPEMF_MDNF));

    getMintermsCountByUser(TYPEMF_MKNF, from, to);
    presetUI.setMaxtermsCountFrom(from);
    presetUI.setMaxtermsCountTo(to);

    getMintermsCountByUser(TYPEMF_MDNF, from, to);
    presetUI.setMintermsCountFrom(from);
    presetUI.setMintermsCountTo(to);

    presetUI.setGenerateType(getGenerateType());
    presetUI.setFullNoPovtor(getFullNoPovtor());
    presetUI.setNoPovtorInGroup(getNoPovtorInGroup());

    return presetUI;
}

void MainWindow::setUIPreset(presets presetToUI)
{
    ui->radioButton_varsCount_2->setChecked(presetToUI.getVarsFuncCount() == VARS_COUNT_MEDIUM);
    ui->radioButton_varsCount_4->setChecked(presetToUI.getVarsFuncCount() == VARS_COUNT_HARD);
    ui->lineEdit_presetsName->setText(presetToUI.getName());

    int from = 0, to = 0;

    auto isAnyFrom = [](int from) // если значение from любое
    {
        return (from == -1);
    };
    auto isAnyTo = [](int to) // если значение to любое
    {
        return (to == -1);
    };
    auto isAny = [isAnyFrom, isAnyTo](int from, int to) // если оба значения любые
    {
        return (isAnyFrom(from) && isAnyTo(to));
    };

    auto funcApplyInterface = [isAny, isAnyFrom, isAnyTo](QRadioButton *rbAny, QRadioButton *rbChose, QLineEdit *leFrom, QLineEdit *leTo, int from, int to)
    {
        if(isAny(from, to))
        {
            rbAny->setChecked(true);
            leFrom->setText("");
            leTo->setText("");
        }
        else
        {
            rbChose->setChecked(true);
            if(isAnyFrom(from))
            {
                leFrom->setText("");
            }
            else
            {
                leFrom->setText(QString::number(from));
            }
            if(isAnyTo(to))
            {
                leTo->setText("");
            }
            else
            {
                leTo->setText(QString::number(to));
            }
        }
    };
    auto funcApplySkleykiCount = [](QRadioButton *rbAny, QRadioButton *rbOne, QRadioButton *rbTwo, QRadioButton *rbThree, int skleykiCount)
    {
        switch (skleykiCount) {
        case SKLEYKY_COUNT_ANY:
            rbAny->setChecked(true);
            break;
        case 1:
            rbOne->setChecked(true);
            break;
        case 2:
            rbTwo->setChecked(true);
            break;
        case 3:
            rbThree->setChecked(true);
            break;
        default:
            break;
        }
    };

    // количество переменных в МКНФ
    from = presetToUI.getVarsCountMknfFrom();
    to = presetToUI.getVarsCountMknfTo();

    funcApplyInterface(ui->radioButton_varsCountInLastFunc_any_mknf,
                       ui->radioButton_varsCountInLastFunc_chose_mknf,
                       ui->lineEdit_varsCountInLastFunc_from_mknf,
                       ui->lineEdit_varsCountInLastFunc_to_mknf,
                       from, to);

    // количество переменных в МДНФ
    from = presetToUI.getVarsCountMdnfFrom();
    to = presetToUI.getVarsCountMdnfTo();

    funcApplyInterface(ui->radioButton_varsCountInLastFunc_any_mdnf,
                       ui->radioButton_varsCountInLastFunc_chose_mdnf,
                       ui->lineEdit_varsCountInLastFunc_from_mdnf,
                       ui->lineEdit_varsCountInLastFunc_to_mdnf,
                       from, to);

    // количество макстермов
    from = presetToUI.getMaxtermsCountFrom();
    to = presetToUI.getMaxtermsCountTo();

    funcApplyInterface(ui->radioButton_mintermsCount_any_mknf,
                       ui->radioButton_mintermsCount_chose_mknf,
                       ui->lineEdit_mintermsCount_from_mknf,
                       ui->lineEdit_mintermsCount_to_mknf,
                       from, to);

    // количество минтермов
    from = presetToUI.getMintermsCountFrom();
    to = presetToUI.getMintermsCountTo();

    funcApplyInterface(ui->radioButton_mintermsCount_any_mdnf,
                       ui->radioButton_mintermsCount_chose_mdnf,
                       ui->lineEdit_mintermsCount_from_mdnf,
                       ui->lineEdit_mintermsCount_to_mdnf,
                       from, to);


    // количество склеек МКНФ
    funcApplySkleykiCount(ui->radioButton_skleykiCountRandom_mknf,
                          ui->radioButton_skleykiCount_1_mknf,
                          ui->radioButton_skleykiCount_2_mknf,
                          ui->radioButton_skleykiCount_3_mknf,
                          presetToUI.getSkleykiCountMknf());

    // количество склеек МДНФ
    funcApplySkleykiCount(ui->radioButton_skleykiCountRandom_mdnf,
                          ui->radioButton_skleykiCount_1_mdnf,
                          ui->radioButton_skleykiCount_2_mdnf,
                          ui->radioButton_skleykiCount_3_mdnf,
                          presetToUI.getSkleykiCountMdnf());

    // количество переменных
    int varsCount = presetToUI.getVarsFuncCount(); // количество переменных в функции
    ui->radioButton_varsCount_2->setEnabled(varsCount == VARS_COUNT_MEDIUM);
    ui->radioButton_varsCount_2->setEnabled(varsCount == VARS_COUNT_HARD);

    // параметры генерации
    ui->checkBox_fullNoPovtor->setChecked(presetToUI.getFullNoPovtor());
    ui->checkBox_noPovtorInGroup->setChecked(presetToUI.getNoPovtorInGroup());

    // тип генерации
    int generateType = presetToUI.getGenerateType();
    ui->radioButton_generateParam_forAll->setChecked(generateType == GENERATE_TYPE_FOR_ALL_STUDENTS);
    ui->radioButton_generateParam_forChoseGroup->setChecked(generateType == GENERATE_TYPE_FOR_CHOSED_GROUP);
    ui->radioButton_generateParam_forChosedInTable->setChecked(generateType == GENERATE_TYPE_FOR_CHOSED_STUDENTS);
    }

void MainWindow::updatePresets()
{
    ui->comboBox_chosePrests->clear();
    for (const auto &preset : qAsConst(presetsList))
    {
        ui->comboBox_chosePrests->addItem(preset.getName());
    }
}

int MainWindow::getGenerateType()
{
    if(ui->radioButton_generateParam_forAll->isChecked())
    {
        return GENERATE_TYPE_FOR_ALL_STUDENTS;
    }
    else if (ui->radioButton_generateParam_forChoseGroup->isChecked())
    {
        return GENERATE_TYPE_FOR_CHOSED_GROUP;
    }
    else if (ui->radioButton_generateParam_forChosedInTable->isChecked())
    {
        return GENERATE_TYPE_FOR_CHOSED_STUDENTS;
    }
    else
    {
        return -1;
    }
}

bool MainWindow::getFullNoPovtor()
{
    return ui->checkBox_fullNoPovtor->isChecked();
}

bool MainWindow::getNoPovtorInGroup()
{
    return ui->checkBox_noPovtorInGroup->isChecked();
}


void MainWindow::on_pushButton_generate_2_clicked()
{

    bool error = false;

    error = acceptFilter(); // применяем фильтр из интерфейса

    if(error == false)
    {
        return;
    }

    // дополнительно
    bool fullRandomFunctions = ui->checkBox_fullNoPovtor->isChecked(); // пытаться сделать полностью эксклюзивные функции
    bool noPovtorInGroup = ui->checkBox_noPovtorInGroup->isChecked(); // флаг отсутсвия повторов в группе

    if(fullRandomFunctions) // если выбран вариант полностью эксклюзивные функции
    {
        noPovtorInGroup = false;  // то проверять эксклюзивность внутри одной группы бессмысленно
    }

    //    fullRandomFunctions = true;
    //    noPovtorInGroup = true;

    if(list == nullptr || list_2 == nullptr)
    {
        QMessageBox::warning(this, "Ошибка", "Не загружены функции!");
        return;
    }
    if(filterFunctionsList.size() == 0)
    {
        QMessageBox::warning(this, "Ошибка", "Не применены фильтры функций!");
        return;
    }
    if(students == nullptr)
    {
        QMessageBox::warning(this, "Ошибка", "Не загружен список студентов!");
        return;
    }
    if(students->size() == 0)
    {
        QMessageBox::warning(this, "Ошибка", "Не загружен список студентов!");
        return;
    }

    QList<studentStruct> listForGenerate; // создаём список тех, для кого надо сгенерировать значения

    //    int generateType = -1;
    if(ui->radioButton_generateParam_forAll->isChecked()) // если выбрана генерация для всех
    {
        //        generateType = GENERATE_TYPE_FOR_ALL_STUDENTS;
        listForGenerate = *students;
    }
    else if (ui->radioButton_generateParam_forChoseGroup->isChecked())  // если выбрана генерация для конкретной группы
    {
        //        generateType = GENERATE_TYPE_FOR_CHOSED_GROUP;
        QString chosedGroup = ui->comboBox_generateParam_forChosedGroup->currentText(); // получаем группу, для которой генерируем
        qDebug() << "Выбранная группа: " << chosedGroup;
        listForGenerate = getStudentsByGroup(chosedGroup); // получаем список студентов данной группы
        for (const auto &student : qAsConst(listForGenerate))
        {
            qDebug() << "Лист генерации: " << student.name << student.group;
        }
    }
    else if (ui->radioButton_generateParam_forChosedInTable->isChecked())  // если выбрана генерация для выбранных в таблице
    {
        //        generateType = GENERATE_TYPE_FOR_CHOSED_STUDENTS;

        auto tw = ui->tableWidget; // ссылка на таблицу

        auto getChosed = [tw](int row) // лямбда для получения, выбран ли данный студент или нет
        {
            if(tw->item(row, 0) != NULL)
            {
                return (tw->item(row, 0)->background() == CHOSED_COLOR); // возвращает, является-ли цвет выбранным, или нет
            }
            else
            {
                return false;
            }
        };

        auto getText = [tw](int i, int j) // лямбда для получения данных из таблицы
        {

            if(tw->item(i, j) != nullptr) // если ячейка не пустая
            {
                return tw->item(i, j)->text(); // получаем текст
            }
            else
            {
                return QString(""); // возвращаем пустоту
            }
        };

        for (int i=0; i<ui->tableWidget->rowCount(); i++) // перебераем все строки
        {
            if(getChosed(i)) // если данная строка выбрана
            {
                // получаем данные о студенте
                QString FIO, group, function;

                FIO = getText(i, 1);    // получаем ФИО
                group = getText(i, 0); // получаем группу
                function = getText(i, 2); // получаем функцию

                studentStruct tempStudent; // создаём временную структуру для добавления выбранного студента
                tempStudent.name = FIO;
                tempStudent.group = group;
                tempStudent.function = function;

                listForGenerate.append(tempStudent); // добавляем студента в список для генерации

                qDebug() << "LAMBDA: " << FIO << group << function;
            }
        }
    }
    else
    {
        QMessageBox::warning(this, "Ошибка", "Не выбраны студенты для генерации!");
        return;
    }

    generateFunctionsForStudents(listForGenerate, fullRandomFunctions, noPovtorInGroup, error); // генерируем функции для этой группы студентов

    for (const auto &student : qAsConst(listForGenerate))
    {
        qDebug() << "Лист генерации: " << student.name << student.group;
    }

    if(error)
    {
        return;
    }

    ui->label_loadFunctions->setText("Обработка изменений...");
    #ifdef SHOW_PROGRESS_BAR_ON_GENERATE_FOR_STUDENTS
    ui->progressBar_loadFunctions->setMinimum(0);
    ui->progressBar_loadFunctions->setMaximum(listForGenerate.size()-1);
    ui->progressBar_loadFunctions->setValue(0);
    ui->progressBar_loadFunctions->setVisible(true);
    #endif
    QApplication::processEvents();
    for (int i=0; i<listForGenerate.size(); i++) // проходим по всем сгенерированным студентам
    {
        for (int j=0; j<students->size(); j++) // проходим по всему списку
        {
            if((listForGenerate.at(i).name == students->at(j).name) && (listForGenerate.at(i).group == students->at(j).group)) // если ФИО и группа совпадают
            {
                qDebug() << "Совпадает! " << listForGenerate.at(i).name << listForGenerate.at(i).group << students->at(j).name << students->at(j).group;
                students->replace(j, listForGenerate.at(i)); // заменяем на новую функцию
                //                setLineColor(ui->tableWidget, j, QBrush(QColor(255, 128, 128)));
            }
        }
        #ifdef SHOW_PROGRESS_BAR_ON_GENERATE_FOR_STUDENTS
        ui->progressBar_loadFunctions->setValue(i);
        QApplication::processEvents();
        #endif
    }


    for (int i=0; i<students->size(); i++) // выводим в таблицу
    {
        ui->tableWidget->item(i, 0)->setText(students->at(i).group);
        ui->tableWidget->item(i, 1)->setText(students->at(i).name);
        ui->tableWidget->item(i, 2)->setText(students->at(i).function);
    }
    #ifdef SHOW_PROGRESS_BAR_ON_GENERATE_FOR_STUDENTS
    ui->progressBar_loadFunctions->setVisible(false);
    #endif
    ui->label_loadFunctions->setText("Готово");
}

void MainWindow::on_pushButton_saveToFile_clicked()
{
//    if(!generatedFunctions)
//    {
//        QMessageBox::warning(this, "Ошибка", "Не загружены функции");
//        return;
//    }
    if(students->size() == 0)
    {
        QMessageBox::warning(this, "Ошибка", "Не загружен список студентов");
        return;
    }

    bool toFileForStudents = ui->checkBox_saveTofileForStudents->isChecked();
    bool toFileExel = ui->checkBox_saveToExel->isChecked();
    if(!toFileExel && !toFileForStudents)
    {
        QMessageBox::warning(this, "Ошибка", "Выберите, куда сохранить данные");
        return;
    }
    if(toFileForStudents)
    {
       saveData(); // сохранение в вариантов в файл
    }
    if(toFileExel)
    {
        saveVariantsToExel(); // сохраняем данные в Exel
    }
}

//void MainWindow::on_radioButton_mintermsCount_chose_clicked(bool checked)
//{
//    if(checked) // включаем возможность выбора количество минтермов (макстермов)
//    {
//        ui->label_mintermsCount_from->setEnabled(true);
//        ui->label_mintermsCount_to->setEnabled(true);
//        ui->lineEdit_mintermsCount_from->setEnabled(true);
//        ui->lineEdit_mintermsCount_to->setEnabled(true);
//    }
//}

//void MainWindow::on_radioButton_mintermsCount_any_clicked(bool checked)
//{
//    if(checked) // выключаем возможность выбора количество минтермов (макстермов)
//    {
//        ui->label_mintermsCount_from->setEnabled(false);
//        ui->label_mintermsCount_to->setEnabled(false);
//        ui->lineEdit_mintermsCount_from->setEnabled(false);
//        ui->lineEdit_mintermsCount_to->setEnabled(false);
//    }
//}

//void MainWindow::on_radioButton_varsCountInLastFunc_chose_clicked(bool checked)
//{
//    if(checked) // включаем возможность выбора количества логических переменных
//    {
//        ui->label_varsCountInLastFunc_from->setEnabled(true);
//        ui->label_varsCountInLastFunc_to->setEnabled(true);
//        ui->lineEdit_varsCountInLastFunc_from->setEnabled(true);
//        ui->lineEdit_varsCountInLastFunc_to->setEnabled(true);
//    }
//}

//void MainWindow::on_radioButton_varsCountInLastFunc_any_clicked()
//{
//    ui->label_varsCountInLastFunc_from->setEnabled(false);
//    ui->label_varsCountInLastFunc_to->setEnabled(false);
//    ui->lineEdit_varsCountInLastFunc_from->setEnabled(false);
//    ui->lineEdit_varsCountInLastFunc_to->setEnabled(false);
//}

void MainWindow::on_pushButton_loadStudentsFile_clicked()
{
    bool loadFromExel = ui->radioButton_loadFromExel->isChecked();
    bool loadFromFileVariants = ui->radioButton_loadFromFileVariants->isChecked();
    if(loadFromExel)
    {
        // считываем данные с формы
        int colFIO = ui->spinBox_columnFIO->value();
        int colGroup = ui->spinBox_columnGroups->value();
        int colVariant = ui->spinBox_columnVariants->value();
        int rowFirst = ui->spinBox_firstLineExel->value();
        int rowLast = ui->spinBox_lastLineExel->value();

        if(rowFirst >= rowLast && rowLast != 0) // проверка диапазона, если не выбран параметр по умолчанию
        {
            QMessageBox::warning(this, "Ошибка", "Номер первой строки должен быть меньше номера последней.");
            updateDataText("");
            return;
        }

        // директория, где находится файл
        QString str = QFileDialog::getOpenFileName(this,tr("OpenFile"),"",tr("Exel Files (*.xlsx);"));
        if(str.isEmpty())
        {
            QMessageBox::information(this, "Внимание", "Вы не выбрали файл");
            return;
        }

        // считываем файл Exel
        int rows = -1;
        int cols = -1;
        ExelEditor exel; // создаем загрузчик


//        connect(&exel, &ExelEditor::updateDataText, this, &MainWindow::updateDataText);
//        connect(&exel, SIGNAL(updateDataText(QString)), this, SLOT(updateDataText(QString)));


        QString **exelText = exel.readFile(str, rows, cols); // загружаем данные

        qDebug() << "Строк: " << rows << " Столбцов: " << cols;

        // функция проверки, не вышли-ли мы за диапазон по столбцам
        auto checkValueCol = [=](int countExel, int number, QString error, bool &exit)
        {
            if(number > countExel)
            {
                QMessageBox::warning(this, "Ошибка", "Ошибка в столбце " + error + ". Заданный столбец не задействован.");
                updateDataText("");
//                return;
                exit = true;
            }
            else
            {
                exit = false;
            }
        };
        qDebug() << "Идём дальше";

        bool exit = false;
        // проверем выход за диапазон по столбцам
        checkValueCol(cols, colFIO, "ФИО студента", exit);
        if(exit)
            return;
        checkValueCol(cols, colGroup, "группы", exit);
        if(exit)
            return;
        checkValueCol(cols, colVariant, "варианта", exit);
        if(exit)
            return;

        // проверем выход за дипазаон по первой строке
        if(rowFirst > rows)
        {
            QMessageBox::warning(this, "Ошибка", "Ошибка в номере первой строки. Заданная строка не задействована.");
            updateDataText("");
            return;
        }

        if(rowLast > rows)
        {
            QMessageBox::warning(this, "Ошибка", "Ошибка в номере последней строки. Заданная строка не задействована.");
            updateDataText("");
            return;
        }

        if(rowLast == 0) // если выбран параментр по умолчанию, то приравниваем к последней строке файла
        {
            rowLast = rows;
        }

        qDebug() << "Проверили на выход за диапазон";

        // смещаем на -1, т.к. в массиве нумерация с 0
        colFIO--;
        colGroup--;
        colVariant--;
        rowFirst--;

        if(students == NULL)
        {
            students = new QList<studentStruct>;
        }
        else
        {
            students->clear(); // очищаем список студентов
        }

        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(rowLast - rowFirst); // устанавливаем количество строк равное количеству строк из таблицы
        ui->tableWidget->setColumnCount(3);
        ui->tableWidget->setColumnWidth(1, 300); // ставим ширину для столбца фамилии
        ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Группа" << "ФИО" << "Функция");

        qDebug() << "Установили количество строк";

        updateDataText("Вывод данных в таблицу...");

        groups.clear();
        int rowNum = 0; // номер строки в таблице, которую заполняем
        for (int i=rowFirst; i<rowLast; i++) // проходим по всем выбранным строкам
        {
            // создаем объект студента
            studentStruct student;
            student.group = exelText[i][colGroup];
            student.name = exelText[i][colFIO];
            student.function = exelText[i][colVariant];

            bool existGroup = false; // хранит, существует ли уже эта группа
            for (int j=0; j<groups.size(); j++)
            {
                if(groups[j] == student.group)  // если группа есть
                {
                    existGroup = true;
                    break;
                }
            }
            qDebug() << "Проверили наличие группы " << existGroup;
            if(!existGroup) // если группа не найдена
            {
                groups.append(student.group); // добавляем группу
                qDebug() << "Добавили группу";
            }

            students->append(student); // добавляем в главный список

            // очищаем память
            for (int j=0; j<3; j++)
            {
                if(ui->tableWidget->item(i, j) != NULL)
                {
                    delete ui->tableWidget->item(i, j);
                }
            }

            // выводим на экран
            ui->tableWidget->setItem(rowNum, COLUMN_GROUP, new QTableWidgetItem(student.group));
            ui->tableWidget->setItem(rowNum, COLUMN_FIO, new QTableWidgetItem(student.name));
            ui->tableWidget->setItem(rowNum, COLUMN_FUNCTION, new QTableWidgetItem(student.function));
            rowNum++;
        }
        ui->comboBox_generateParam_forChosedGroup->clear();
        ui->comboBox_generateParam_forChosedGroup->addItems(groups);
        ui->lineEdit_exelOutput->setText(str); // выводим в путь сохранения наш текущий файл
        fileExelOutputDir = str;
        qDebug() << "Загрузка завершена";
        updateDataText("Загрузка списка студентов завершена");
        QString styleSheet = "background-color: none;";
        ui->label_loadData->setStyleSheet(styleSheet);
    }
    else if (loadFromFileVariants)
    {
        QFile *fileStudents = new QFile(FILE_STUDENTS_NAME);
        if(!fileStudents->exists())
        {
            message("Не найден файл со студентами!");
            needClose = true;
            return;
        }
        if(!fileStudents->open(QIODevice::ReadOnly)) // если не открывается
        {
            message("Не удаётся открыть файл со студентами!");
            needClose = true;
            return;
        }
        qDebug() << "Проверили файл";
        QString text = fileStudents->readAll(); // считываем весь файл

//        QStringList fileHexList = text.split("HASH="); // Разбиваем на до и после хэша
//        if(fileHexList.size() < 2)
//        {
//            QMessageBox::warning(this, "Ошибка", "Файл повреждён");
//            return;
//        }
//        QString hashFromFile = fileHexList.at(1); // получаем хэш
//        QString hash = QString(QCryptographicHash::hash(fileHexList.at(0).toLatin1(),QCryptographicHash::Md5).toHex()); // получаем хэш содержимого
//        if(hashFromFile != hash) // Если эешт различаются
//        {
//            QMessageBox::warning(this, "Ошибка", "Ошибка хэша!");
//            return;
//        }
//        text = fileHexList.at(0);

        bool ok = false; // флаг корректности хеша
        md5crypter::decryptStr(text, ok); // проверяем хеш
        if(!ok) // если ошибка
        {
            QMessageBox::warning(this, "Ошибка", "Ошибка хэша!");
            return;
        }

        qDebug() << "text read: " << text;
        fileStudents->close(); // закрываем файл
        delete fileStudents;
        fileStudents = nullptr;
        QStringList studentsQStringList = text.split(STR_SEPARATOR_1, Qt::SplitBehavior(Qt::SkipEmptyParts)); // разбиваем по строкам
        if(studentsQStringList.size() <= 1) // если в файле ничего нет
        {
            message("Файл пустой или повреждён!");
            return;
        }
        qDebug() << "Прочли файл";
        students = new QList<studentStruct>; // выделяем память
        qDebug() << "Создали список студентов";
        ui->label_loadData->setVisible(true);
        ui->label_loadData->setText("Идёт загрузка спсика студентов...");
        QString styleSheet = "background-color: rgb(255, 75, 75);";
        ui->label_loadData->setStyleSheet(styleSheet);
//        ui->progressBar_loadData->setVisible(true);
//        ui->progressBar_loadData->setMinimum(0);
//        ui->progressBar_loadData->setMaximum(studentsQStringList.size());
//        ui->progressBar_loadData->setValue(0);
        groups.clear();
        QApplication::processEvents();
        for (int i=0; i<studentsQStringList.size(); i++)
        {
            QStringList listLine = studentsQStringList[i].split(STR_SEPARATOR_2, Qt::SplitBehavior(Qt::SkipEmptyParts)); // ищем данные
            qDebug() << listLine;
            if(listLine.size() < 2)
            {
                continue;
            }
            qDebug() << "i = " << i;
            studentStruct *student = new studentStruct;
            student->group = listLine[0];
            student->name = listLine[1];
            if(listLine.size() > 2)
            {
                student->function = listLine[2];
            }
            qDebug() << "Добавли данные в структуру";
            students->append(*student);
            qDebug() << "удалили указатель";
            bool existGroup = false; // хранит, существует ли уже эта группа
            for (int j=0; j<groups.size(); j++)
            {
                if(groups[j] == student->group)  // если группа есть
                {
                    existGroup = true;
                    break;
                }
            }
            qDebug() << "Проверили наличие группы " << existGroup;
            if(!existGroup) // если группа не найдена
            {
                groups.append(student->group); // добавляем группу
                qDebug() << "Добавили группу";
            }
            qDebug() << "Идём дальше";
            delete student;
            student = nullptr;
//            ui->progressBar_loadData->setValue(i);
            QApplication::processEvents();
        }
        //    ui->comboBox_group->addItems(groups);
        ui->comboBox_generateParam_forChosedGroup->clear();
        ui->comboBox_generateParam_forChosedGroup->addItems(groups);
        ui->label_loadData->setText("Загрузка списка студентов завершена");
        styleSheet = "background-color: none;";
        ui->label_loadData->setStyleSheet(styleSheet);
        if(students->size() == 0)
        {
            message("Не удалось получить данные о студентах!");
            return;
        }
        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(students->size());
        ui->tableWidget->setColumnCount(3);
        ui->tableWidget->setColumnWidth(1, 300); // ставим ширину для столбца фамилии
        ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Группа" << "ФИО" << "Функция");

        ui->label_loadData->setText("Выгрузка в таблицу...");
//        ui->progressBar_loadData->setMinimum(0);
//        ui->progressBar_loadData->setMaximum(students->size());
//        ui->progressBar_loadData->setValue(0);
        QApplication::processEvents();
        for (int i=0; i<students->size(); i++)
        {
            QTableWidgetItem *itemGroup = new QTableWidgetItem(students->at(i).group);
            QTableWidgetItem *itemFIO = new QTableWidgetItem(students->at(i).name);
            QTableWidgetItem *itemFunc = new QTableWidgetItem(students->at(i).function);
            for (int j=0; j<3; j++)
            {
                if(ui->tableWidget->item(i, j) != nullptr)
                {
                    delete ui->tableWidget->item(i, j);
                }
            }
            ui->tableWidget->setItem(i, 0, itemGroup);
            ui->tableWidget->setItem(i, 1, itemFIO);
            ui->tableWidget->setItem(i, 2, itemFunc);
//            ui->progressBar_loadData->setValue(i);
            QApplication::processEvents();
        }
//        ui->progressBar_loadData->setVisible(false);
        //    if(ui->radioButton_generateParam_forAll->isChecked())
        //    {
        //        on_radioButton_generateParam_forAll_clicked();
        //    }
        //    else if (ui->radioButton_generateParam_forChoseGroup->isChecked())
        //    {
        //        on_radioButton_generateParam_forChoseGroup_clicked();
        //    }
        //    else if(ui->radioButton_generateParam_forChosedInTable->isChecked())
        //    {
        //        on_radioButton_generateParam_forChosedInTable_clicked();
        //    }
        ui->radioButton_generateParam_forAll->setChecked(true);
        on_radioButton_generateParam_forAll_clicked();
        ui->label_loadData->setText("Загрузка списка студентов завершена");
        ui->label_loadData->setVisible(false);
        styleSheet = "background-color: none;";
        ui->label_loadData->setStyleSheet(styleSheet);
    }

}

void MainWindow::on_pushButton_loadFuncs_clicked()
{
    if(generatedFunctions)
    {
        message("Функции уже загружены!");
        return;
    }
//    if(loader.getIsLoading()) // если уже идёт загрузка
//    {
//        return;
//    }
//    ui->pushButton_loadFuncs->setEnabled(false);
    ui->pushButton_loadFuncs->hide();
    if(!loader.load(ui->label_loadFunctions, /*ui->progressBar_loadFunctions,*/ list, list_2, generatedFunctions, starting) && starting) // загружаем функции
    {
        ui->label_loadFunctions->setText("Не удалось загрузить функции. Загрузите их вручную.");
//        ui->pushButton_loadFuncs->setEnabled(true);
        ui->pushButton_loadFuncs->show();
        QString styleSheet = "background-color: rgb(255, 75, 75);";
        ui->label_loadFunctions->setStyleSheet(styleSheet);
    }
    starting = false;
}

void MainWindow::on_pushButton_chose_clicked()
{
    int rows = ui->tableWidget->rowCount();
    int cols = ui->tableWidget->columnCount();
    for (int i=0; i<rows; i++) // проходим по всем строкам
    {
        bool selected = false;
        for (int j=0; j<cols; j++) // проходим по всем столбцам
        {
            if(ui->tableWidget->item(i, j) != NULL) // если память выделена
            {
                if(ui->tableWidget->item(i, j)->isSelected()) // если хотя бы один столбце выбран
                {
                    selected = true;
                }
            }
        }
        if(selected) // если эта строка выбрана
        {
            setLineColor(ui->tableWidget, i, CHOSED_COLOR);
        }
    }
}

void MainWindow::on_pushButton_noChose_clicked()
{
    int rows = ui->tableWidget->rowCount();
    int cols = ui->tableWidget->columnCount();
    for (int i=0; i<rows; i++) // проходим по всем строкам
    {
        bool selected = false;
        for (int j=0; j<cols; j++) // проходим по всем столбцам
        {
            if(ui->tableWidget->item(i, j) != NULL) // если память выделена
            {
                if(ui->tableWidget->item(i, j)->isSelected()) // если хотя бы один столбце выбран
                {
                    selected = true;
                }
            }
        }
        if(selected) // если эта строка выбрана
        {
            setLineColor(ui->tableWidget, i, NO_CHOSED_COLOR);
        }
    }
}

void MainWindow::on_pushButton_clearAllChose_clicked()
{
    int rows = ui->tableWidget->rowCount();
    for (int i=0; i<rows; i++) // проходим по всем строкам
    {
        setLineColor(ui->tableWidget, i, NO_CHOSED_COLOR);
    }
}

void MainWindow::on_radioButton_generateParam_forChoseGroup_clicked()
{
    auto list = getStudentsByGroup(ui->comboBox_generateParam_forChosedGroup->currentText());
    int size = list.size();
    ui->label_generateStudentsCount->setText("Генерация будет выполнена для " + QString::number(size) + " студентов");
}

void MainWindow::on_radioButton_generateParam_forChosedInTable_clicked()
{
    auto tw = ui->tableWidget; // ссылка на таблицу

    auto getChosed = [tw](int row) // лямбда для получения, выбран ли данный студент или нет
    {
        if(tw->item(row, 0) != NULL)
        {
            return (tw->item(row, 0)->background() == CHOSED_COLOR); // возвращает, является-ли цвет выбранным, или нет
        }
        else
        {
            return false;
        }
    };

    int size = 0;

    for (int i=0; i<ui->tableWidget->rowCount(); i++) // перебераем все строки
    {
        if(getChosed(i)) // если данная строка выбрана
        {
            size++;
        }
    }

    ui->label_generateStudentsCount->setText("Генерация будет выполнена для " + QString::number(size) + " студентов");
}

void MainWindow::on_radioButton_generateParam_forAll_clicked()
{
    int size = 0;
    if(students != NULL)
    {
        size = students->size();
    }
    ui->label_generateStudentsCount->setText("Генерация будет выполнена для " + QString::number(size) + " студентов");
}

void MainWindow::on_comboBox_generateParam_forChosedGroup_currentIndexChanged(int index)
{
    ui->radioButton_generateParam_forChoseGroup->setChecked(true);
    on_radioButton_generateParam_forChoseGroup_clicked();
    Q_UNUSED(index);
}

void MainWindow::on_lineEdit_mintermsCount_from_mdnf_textChanged(const QString &arg1)
{
    if(!arg1.isEmpty())
    {
        ui->radioButton_mintermsCount_chose_mdnf->setChecked(true);
    }
}


void MainWindow::on_lineEdit_mintermsCount_to_mdnf_textChanged(const QString &arg1)
{
    if(!arg1.isEmpty())
    {
        ui->radioButton_mintermsCount_chose_mdnf->setChecked(true);
    }
}


void MainWindow::on_lineEdit_varsCountInLastFunc_from_mdnf_textChanged(const QString &arg1)
{
    if(!arg1.isEmpty())
    {
        ui->radioButton_varsCountInLastFunc_chose_mdnf->setChecked(true);
    }
}

void MainWindow::on_lineEdit_varsCountInLastFunc_to_mdnf_textChanged(const QString &arg1)
{
    if(!arg1.isEmpty())
    {
        ui->radioButton_varsCountInLastFunc_chose_mdnf->setChecked(true);
    }
}

void MainWindow::on_lineEdit_mintermsCount_from_mknf_textChanged(const QString &arg1)
{
    if(!arg1.isEmpty())
    {
        ui->radioButton_mintermsCount_chose_mknf->setChecked(true);
    }
}

void MainWindow::on_lineEdit_mintermsCount_to_mknf_textChanged(const QString &arg1)
{
    if(!arg1.isEmpty())
    {
        ui->radioButton_mintermsCount_chose_mknf->setChecked(true);
    }
}

void MainWindow::on_lineEdit_varsCountInLastFunc_from_mknf_textChanged(const QString &arg1)
{
    if(!arg1.isEmpty())
    {
        ui->radioButton_varsCountInLastFunc_chose_mknf->setChecked(true);
    }
}

void MainWindow::on_lineEdit_varsCountInLastFunc_to_mknf_textChanged(const QString &arg1)
{
    if(!arg1.isEmpty())
    {
        ui->radioButton_varsCountInLastFunc_chose_mknf->setChecked(true);
    }
}

void MainWindow::on_pushButton_clearFilter_clicked()
{
    ui->radioButton_varsCount_4->setChecked(true);
    ui->radioButton_mintermsCount_any_mdnf->setChecked(true);
    ui->radioButton_mintermsCount_any_mknf->setChecked(true);
    ui->radioButton_skleykiCountRandom_mdnf->setChecked(true);
    ui->radioButton_skleykiCountRandom_mknf->setChecked(true);
    ui->radioButton_varsCountInLastFunc_any_mdnf->setChecked(true);
    ui->radioButton_varsCountInLastFunc_any_mknf->setChecked(true);
    ui->radioButton_generateParam_forAll->setChecked(true);
    on_radioButton_generateParam_forAll_clicked();

    QList<QLineEdit*> list;
    list.append(ui->lineEdit_mintermsCount_from_mdnf);
    list.append(ui->lineEdit_mintermsCount_from_mknf);
    list.append(ui->lineEdit_mintermsCount_to_mdnf);
    list.append(ui->lineEdit_mintermsCount_to_mknf);
    //    list.append(ui->lineEdit_outputGenerateFunction);
    list.append(ui->lineEdit_varsCountInLastFunc_from_mdnf);
    list.append(ui->lineEdit_varsCountInLastFunc_from_mknf);
    list.append(ui->lineEdit_varsCountInLastFunc_to_mdnf);
    list.append(ui->lineEdit_varsCountInLastFunc_to_mknf);

    for(auto lineEdit : list)
    {
        lineEdit->clear();
    }

    ui->checkBox_fullNoPovtor->setChecked(true);
    ui->checkBox_noPovtorInGroup->setChecked(false);
}

void MainWindow::updateDataText(QString text)
{
    if(text == "")
    {
        ui->label_loadData->setVisible(false);
    }
    ui->label_loadData->setVisible(true);
    ui->label_loadData->setText(text);
}

void MainWindow::on_checkBox_fullNoPovtor_stateChanged(int arg1)
{
    ui->checkBox_noPovtorInGroup->setEnabled(!arg1); // если выбран варинт полностью эксклюзивные функкции, то эксклюзивность в группах не нужна
}

void MainWindow::on_pushButton_presetsAdd_clicked()
{
    presets preset;
    preset = getPresetByUI();
    QString name = ui->lineEdit_presetsName->text();
    if(name.isEmpty())
    {
        name = "Новый пресет";
    }
    preset.setName(name);
    addPresets(presetsList, preset); // добавляем пресет в список
    updatePresets(); // обновляем пресеты на форме
    savePresetsToFile(presetsList, FILE_PRESETS_URL);
    ui->comboBox_chosePrests->setCurrentIndex(ui->comboBox_chosePrests->count()-1);
}

void MainWindow::on_pushButton_presetsDelete_clicked()
{
    int n = ui->comboBox_chosePrests->currentIndex(); // номер выбранного пресета
    if(n < 0)
    {
        return;
    }
    presetsList.removeAt(n);
    updatePresets(); // обновляем пресеты на форме
    savePresetsToFile(presetsList, FILE_PRESETS_URL);
    ui->comboBox_chosePrests->setCurrentIndex(ui->comboBox_chosePrests->count()-1);
}

void MainWindow::on_pushButton_presetsApplyName_clicked()
{
    QString name = ui->lineEdit_presetsName->text();
    if(name.isEmpty())
    {
        message("Не введёно название пресета!", "Ошибка", 3);
        return;
    }
    int n = ui->comboBox_chosePrests->currentIndex();// номер выбранного пресета
    if(n < 0)
    {
        return;
    }
    presets preset = getPresetByUI(); // формируем пресет из текщих данных
    preset.setName(name);
    presetsList.replace(n, preset); // заменяем пресет
    updatePresets();// обновляем пресеты на форме
    savePresetsToFile(presetsList, FILE_PRESETS_URL);
    ui->comboBox_chosePrests->setCurrentIndex(n);
}

void MainWindow::on_comboBox_chosePrests_currentIndexChanged(int index)
{
    if(index >= 0)
    {
       setUIPreset(presetsList.at(index));
    }
}

void MainWindow::on_pushButton_save_choseFileExel_clicked()
{
    fileExelOutputDir = QFileDialog::getOpenFileName(this,tr("OpenFile"),"",tr("Exel Files (*.xlsx);"));
    if(fileExelOutputDir.isEmpty())
    {
        ui->lineEdit_exelOutput->clear();
        QMessageBox::information(this, "Внимание", "Вы не выбрали файл");
        return;
    }
    ui->lineEdit_exelOutput->setText(fileExelOutputDir);
}

void MainWindow::on_pushButton_clicked()
{
    int min = 0; // 0000
    int max = 65535; // FFFF
//    QString func = QString::number(i, 16).toUpper(); // переводим из 10 в 16 ссч
//    QList <mdnfMacKlassky> listmmk;
//    qDebug() << "Ситаем 1008";
//    mdnfMacKlassky md;
//    md.setFunction("1008");
    qDebug() << "начали";
    QTime time_start = QTime::currentTime();
    qDebug() << "Время начала: " << time_start.toString();
    int count = 0;
    QStringList noCorrerct;
    for (int i=min; i<max; i++)
    {
        QString func = QString::number(i, 16).toUpper(); // переводим из 10 в 16 ссч
//        qDebug() << i << max << func;
        mdnfMacKlassky md;
        if(md.setFunction(func))
        {
            if(md.canCalculateMnf(0) && md.canCalculateMnf(1))
            {
                count++;
            }
            else
            {
                noCorrerct.append(func);
            }

        }
        else
        {
            noCorrerct.append(func);
        }
//        qDebug() << "Идём дальше";
//        listmmk.append(md);
    }
    qDebug() << "всё";
    QTime time_end = QTime::currentTime();
    qDebug() << "Время начала: " << time_start.toString() << "Время окончания: " << time_end.toString();
    qDebug() << "Успешно: " << count << "/" << max;
    qDebug() << "Не успешно: " << noCorrerct.size();
    qDebug() << "Не успешно: " << noCorrerct;
}
