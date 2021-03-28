#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mdnfmacklassky.h"
#include "tablewidgetfunctions.h"
#include <QMessageBox>

// сделать очистку логического редактора

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(editor != nullptr)
    {
        delete editor;
        editor = nullptr;
    }
    editor = new logicEditor(ui->tableWidget_logicEditor);
    editor->setAutoInput(true);
    editor->setAutoInputType(1);

//    on_pushButton_calclulate_clicked();
    this->setWindowTitle("Минимизация методом Куайна Мак-Класски");
    ui->checkBox_addFromula_toeditor->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_calclulate_clicked()
{
    // начальные проверки
    QString function_16;
    bool ok = false;
    function_16 = ui->lineEdit_function_16->text(); // берем текст функциии
    function_16.toInt(&ok, 16); // проверяем, можно-ли её перевести из 16й системы счисления
    if(!ok)
    {
        QMessageBox::warning(this, "Ошибка!", "Это не 16-я функция!");
        return;
    }
//    if(function_16.count() != 4) // проверяем, что в функции 4 переменных
//    {
//        QMessageBox::warning(this, "Ошибка!", "Функция должна состоять из 4х переменных!");
//        return;
//    }
    mdnfMacKlassky *mdnf = new mdnfMacKlassky;
    if(!mdnf->setFunction(function_16))
    {
        QMessageBox::warning(this, "Ошибка!", "Программа не может посчитать данную функцию :(");
        return;
    }
    int typeMnf = -1;
    if(ui->radioButton_mknf->isChecked())
    {
        typeMnf = 0;
    }
    else if(ui->radioButton_mdnf->isChecked())
    {
       typeMnf = 1;
    }
    if(typeMnf == -1)
    {
        QMessageBox::warning(this, "Ошибка!", "Выберите тип минимизации");
        return;
    }
    if(!mdnf->canCalculateMnf(typeMnf))
    {
        QMessageBox::warning(this, "Ошибка!", "Программа не может посчитать МДНФ данной функции :(");
        return;
    }

    // очистка предыдущих полей
    tableWidgetFunctions::clearTW(ui->tableWidget_tot);
    tableWidgetFunctions::clearTW(ui->tableWidget_ones);
    tableWidgetFunctions::clearTW(ui->tableWidget_skleyki_1);
    tableWidgetFunctions::clearTW(ui->tableWidget_skleyki_2);
    tableWidgetFunctions::clearTW(ui->tableWidget_skleyki_3);
    tableWidgetFunctions::clearTW(ui->tableWidget_karta_minimizacii);
    ui->tableWidget_logicEditor->setColumnCount(0);


    // вывод таблицы истинности
    QStringList tableWidgetTotList = mdnf->getSschTableList(); // получаем список таблицы истинности
    tableWidgetFunctions::addQStringListToTWOneSymwolInItem(ui->tableWidget_tot, tableWidgetTotList); // выводим текст таблицы истинности
    tableWidgetFunctions::setTableWidgetItemsSize(ui->tableWidget_tot, 30, 40); // задаем размер
    ui->tableWidget_tot->setHorizontalHeaderLabels(QStringList() << "a" << "b" << "c" << "d" << "F"); // выставляем переменные
    QStringList verticalList;
    for (int i=0; i<16; i++)
    {
        QString str;
        str.setNum(i, 16);
        verticalList.append(str.toUpper());
    }
    ui->tableWidget_tot->setVerticalHeaderLabels(verticalList); // заполняем вертикальные заголовки

    // вывод значений, где функция равна 1
    QStringList ones = mdnf->getListOnes(typeMnf);
    tableWidgetFunctions::addQStringListToTWOneSymwolInItem(ui->tableWidget_ones, ones);
    tableWidgetFunctions::setTableWidgetItemsSize(ui->tableWidget_ones, 30, 40); // задаем размер

    // вывод склеек
    QStringList skleyki1 = mdnf->getSkleyki1(typeMnf);
    if(skleyki1 != ones)
    {
        tableWidgetFunctions::addQStringListToTWOneSymwolInItem(ui->tableWidget_skleyki_1, skleyki1);
        tableWidgetFunctions::setTableWidgetItemsSize(ui->tableWidget_skleyki_1, 30, 40); // задаем размер
        sortSkleiki(ui->tableWidget_skleyki_1);
    }

    QStringList skleyki2 = mdnf->getSkleyki2(typeMnf);
    if(skleyki2 != skleyki1)
    {
        tableWidgetFunctions::addQStringListToTWOneSymwolInItem(ui->tableWidget_skleyki_2, skleyki2);
        tableWidgetFunctions::setTableWidgetItemsSize(ui->tableWidget_skleyki_2, 30, 40); // задаем размер
        sortSkleiki2X(ui->tableWidget_skleyki_2);
    }

    QStringList skleyki3 = mdnf->getSkleyki3(typeMnf);
    if(skleyki3 != skleyki2)
    {
        tableWidgetFunctions::addQStringListToTWOneSymwolInItem(ui->tableWidget_skleyki_3, skleyki3);
        tableWidgetFunctions::setTableWidgetItemsSize(ui->tableWidget_skleyki_3, 30, 40); // задаем размер
        sortSkleiki2X(ui->tableWidget_skleyki_3);
    }

    // создание карты покрытия
    ui->tableWidget_karta_minimizacii->setRowCount(skleyki3.size()); // количество строк равно количеству склеек
    ui->tableWidget_karta_minimizacii->setColumnCount(ones.size()); // количество столбцов равно количеству значений, при которых функция равна 1
    tableWidgetFunctions::setTableWidgetItemsSize(ui->tableWidget_karta_minimizacii, 30, 40); // задаем размер
    ui->tableWidget_karta_minimizacii->setVerticalHeaderLabels(skleyki3); // в качестве вертикальных значений ставим результат склейки
    ui->tableWidget_karta_minimizacii->setHorizontalHeaderLabels(ones); // в качестве горизонтальных заголовков ставим значения при которрых F = 1
    // выводим + в карту покрытия
    for (int i=0; i<skleyki3.size(); i++) // строки
    {
        for (int j=0; j<ones.size(); j++) // столбцы
        {
            QString chislo = ones.at(j); // получаем текущее значение сверху
            QString skleyka = skleyki3.at(i); // получаем текущую проверяемую склейку
            if(chislo.count() != skleyka.count()) // если в значениях разное количество символов
            {
                QMessageBox::critical(this, "ERROR", "Произошла непредвиденная ошибка! " + QString::number(i) + " " + QString::number(j));
                return;
            }
            bool ekvivalent = true; // флаг совпадения значения и склейки
            for (int k=0; k<chislo.count(); k++) // символы числа
            {
                if((chislo.at(k) != skleyka.at(k)) && (skleyka.at(k) != "X")) // если символы разные и символ не Х
                {
                    ekvivalent = false; // ставим, что числа различаются
                }
            }
            if(ekvivalent) // если число совпадает со склейкой
            {
                ui->tableWidget_karta_minimizacii->setItem(i, j, new QTableWidgetItem("+")); // выводим + в ячейку
            }
            else
            {
               ui->tableWidget_karta_minimizacii->setItem(i, j, new QTableWidgetItem("")); // создаём пустую ячейку
            }
        }
    }
    // выделяем ядра
    for (int j=0; j<ones.size(); j++) // столбцы
    {
        int plusCount = 0; // счётчик количество + в столбце
        int lastRow = 0; // хранит последнюю строку, чтобы если будет 1 плюс, можно было бы закрасить
        for (int i=0; i<skleyki3.size(); i++) // строки
        {
            if(ui->tableWidget_karta_minimizacii->item(i, j)->text() == "+") // если в ячейке стоит +
            {
                plusCount++; // увеличиваем счётчик
                lastRow = i; //сохраняем номер строки
            }
        }
        if(plusCount == 1) // если в столбце всего 1 плюс
        {
            ui->tableWidget_karta_minimizacii->item(lastRow, j)->setBackground(QBrush(redColor)); // закрашиваем ячейку
        }
    }
    // выделяем то, что покрывается ядрами
    for (int i=0; i<skleyki3.size(); i++) // строки
    {
        for (int j=0; j<ones.size(); j++) // столбцы
        {
            if(ui->tableWidget_karta_minimizacii->item(i, j)->background().color() == redColor) // если есть ядро в строке
            {
                for (int k=0; k<ones.size(); k++) // столбцы
                {
                    if(ui->tableWidget_karta_minimizacii->item(i, k)->text() == "+") // если в ячейке есть +
                    {
                        for (int h=0; h<skleyki3.size(); h++) // строки
                        {
                            if(ui->tableWidget_karta_minimizacii->item(h, k)->background().color() != redColor) // если ячейка не закрашена в красный цвет
                            {
                                ui->tableWidget_karta_minimizacii->item(h, k)->setBackground(QBrush(greenColor)); // закрашиваем ячейку в зелёный цвет
                            }
                        }
                    }
                }
            }
        }
    }


    qDebug() << "Количество минтермов: " << mdnf->getMintermsCount(1);
    qDebug() << "Количество макстермов: " << mdnf->getMintermsCount(0);

    ui->comboBox_formulas->clear();
    editor->clearTable();
    if(ui->checkBox_addFromula_toeditor->isChecked())
    {
        // выводим ядровую формулу в логический редактор
        editor->setVirazhenie(mdnf->getCoresMfFormula(typeMnf));
        ui->comboBox_formulas->addItems(mdnf->getMdnfList(typeMnf));
    }

    for (const auto &formula : mdnf->getMdnfList(typeMnf))
    {
        qDebug() << "МДНФ: " << formula;
    }

}

void MainWindow::sortSkleiki(QTableWidget *tableWidget)
{
    int rows = tableWidget->rowCount(); // получаем колитечтво строк
    if(rows <= 0)
        return;
    int cols = tableWidget->columnCount(); // получаем количество столбцов
    if(cols <= 0)
        return;
    // создаем массив
    QString **tableText = nullptr; // массив для хранения элементов таблицы
    tableText = new QString *[rows]; // выделяем память под строки
    for (int i=0; i<rows; i++)
    {
        tableText[i] = new QString [cols]; // выделяем память под столбцы
    }
    // получаем данные из таблицы
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<cols; j++)
        {
            tableText[i][j] = tableWidget->item(i, j)->text(); // сохраняем данные в массив
        }
    }
    // сортируем склейки
    QStringList sortSkleikyList; // создаем список для отсортированных склеек
//    int lastRow = 0; // номер строки, в которую последний раз были занесены данные (склейка)
//    bool wasAppended = false; // флаг, был-ли добавлен хоть один элемент при данном p
    for (int p=0; p<cols; p++) // p = номер позиции Х в строке
    {
        qDebug() << "Ищем Х в позиции " << p;
        for (int i=0; i<rows; i++)
        {
            for (int j=0; j<cols; j++)
            {
                if((tableText[i][j] == "X") && (j == p))  // если позиция Х совпадает с текущим номером
                {
                    qDebug() << "В строке" << i << "в столбце" << j << "X находится на позиции" << p;
                    QString skleyka;
                    for (int h=0; h<cols; h++) // проходим по всем элементам строки
                    {
//                        if(!wasAppended) // если с предыдущего раза не было добавлено ни одного элемента
//                        {
////                            skleyka.append("    ");
//                            wasAppended = true; // ставим что один элемент был добавлен
//                        }
                        skleyka.append(tableText[i][h]);
//                        tableWidget->setItem(lastRow, h, new QTableWidgetItem((tableText[i][h]))); // выводим в таблицу
//                        lastRow++; // увеличиваем на 1 номер последней строки
                    }
                    qDebug() << "Создана склейка " << skleyka;
                    sortSkleikyList.append(skleyka); // обавляем склейку в список
                }
            }
        }
//        wasAppended = false;
    }
    // ищём повторящиеся элементы (из-за несовсем корректной работы группировщика)
    for (int i=0; i<sortSkleikyList.size(); i++)
    {
        for (int j=0; j<i; j++)
        {
            if(sortSkleikyList[j] == sortSkleikyList[i]) // если существует
            {
                sortSkleikyList.removeAt(i); // удаляем
            }
        }
    }
    QStringList ishodniyList;
    if(!tableWidgetFunctions::getTWTextList(tableWidget, ishodniyList))
    {
        qDebug() << "Не получилось получить данные из " << tableWidget;
        return;
    }
    for (int i=0; i<ishodniyList.size(); i++)
    {
        if(!(ishodniyList[i].contains("X"))) // если элемент не содержит Х, то добавляем его в конец
        {
            sortSkleikyList.append(ishodniyList[i]);
        }
    }
//    tableWidgetFunctions::clearTW(tableWidget);
//    tableWidgetFunctions::addQStringListToTWOneSymwolInItem(tableWidget, sortSkleikyList);
    tableWidgetFunctions::clearTW(tableWidget);
    tableWidget->setColumnCount(sortSkleikyList.at(0).count());
    int size = sortSkleikyList.size(); // получаем количество склеек в списке
    int p = 0; // позиция Х
    for (int i=0; i<size; i++)
    {
        QString skleyka = sortSkleikyList[i]; // получаем склейку
        int pos = 0; // текущая позиция Х
        for (int j=0; j<skleyka.count(); j++)
        {
            if(skleyka.at(j) == "X")
            {
                pos = j;
            }
        }
        if(pos != p)
        {
            tableWidget->setRowCount(tableWidget->rowCount()+1);
            tableWidget->setRowHeight(tableWidget->rowCount()-1, 3);
            for (int j=0; j<skleyka.count(); j++)
            {
                tableWidget->setItem(tableWidget->rowCount()-1, j, new QTableWidgetItem(" "));
                tableWidget->item(tableWidget->rowCount()-1, j)->setBackground(QBrush(Qt::black));
            }
        }
        p = pos;
        tableWidget->setRowCount(tableWidget->rowCount()+1);
        for (int j=0; j<skleyka.count(); j++)
        {
            tableWidget->setItem(tableWidget->rowCount()-1, j, new QTableWidgetItem(skleyka.at(j))); // выподим каждый элемент
        }
    }
}

void MainWindow::sortSkleiki2X(QTableWidget *skleikiTableWidget)
{
    int rows = skleikiTableWidget->rowCount(); // получаем количество строк
    int cols = skleikiTableWidget->columnCount(); // получаем количество стобцов
    if((rows == 0) || (cols == 0))
    {
        qDebug() << "Таблица пустая. Сортировка невозможна!" << rows << cols;
        return;
    }
    QStringList skleikiList; // создаём лист для хранения склеек
    if(!tableWidgetFunctions::getTWTextList(skleikiTableWidget, skleikiList))
    {
        qDebug() << "Не удалось получить данные из таблицы";
        return;
    }
    qDebug() << "исходный список " << skleikiList;
    int size = skleikiList.size();
    if(size < 2)
    {
        qDebug() << "Нет склеек для сортировки";
        return;
    }
    skleyka *skleikaSort = new skleyka [size]; // выделяем память под структуру склеек
    for (int i=0; i<size; i++)
    {
        skleikaSort[i].text = skleikiList[i].split("", Qt::SplitBehavior(Qt::SkipEmptyParts));
        skleikaSort[i].size = skleikiList[i].size(); // сохраняем размер массива
        skleikaSort[i].type = -1;
        // для склеек, где нет Х
//        bool ok = false; // хранит, был ли отсортирован хотя бы один элемент
        if(!skleikiList[i].contains("X")) // если не содержит Х
        {
            qDebug() << "Склейка" <<  skleikiList[i] << "не содержт Х";
            skleikaSort[i].type = 0; // задем тип 0
            continue;
        }
        // для 2х x
        // X на 1 месте

        // перебираем все возможные варианты с 2 Х
        for (int j=0; j<skleikaSort[i].size-1; j++) // минус 1, т.к. к = j+1
        {
            for (int k=j+1; k<skleikaSort[i].size; k++)
            {
                if((skleikaSort[i].text[j] == "X") && (skleikaSort[i].text[k] == "X")) // если 2 Х находятся на нужных местах
                {
                    bool error = false; // флаг ошибки
                    for (int g=0; g<skleikaSort[i].size; g++) // проходим по всем элементам
                    {
                        if((skleikaSort[i].text[g] == "X") && ((g != j) || (g != k))) // если есть ещё один Х и он не находится на позиции J или K, т.е. содержится 3Х, то пропускаем
                        {
                            error = true;
                            qDebug() << "Склейка" << skleikiList[i] << "не подходит - у неё не 2 Х";
                        }
                    }
                    if(!error)  // если ошибок нет
                    {
                        skleikaSort[i].type = (j*100) + (k*10); // устанавливаем тип (J умножаем на 10, чтобы не было повторений)
                        error = false;
                    }
                }
            }
        }
        // для одного Х
        for (int j=0; j<skleikaSort[i].size; j++) // минус 1, т.к. к = j+1
        {
            if((skleikaSort[i].text[j] == "X")) // если 2 Х находятся на нужных местах
            {
                bool error = false; // флаг ошибки
                for (int g=0; g<skleikaSort[i].size; g++) // проходим по всем элементам
                {
                    if((skleikaSort[i].text[g] == "X") && ((g != j))) // если есть ещё один Х и он не находится на позиции J, т.е. содержится 2Х и более, то пропускаем
                    {
                        error = true;
                        qDebug() << "Склейка" << skleikiList[i] << "не подходит - у неё 2 Х";
                    }
                }
                if(!error)  // если ошибок нет
                {
                    skleikaSort[i].type = (j*10); // устанавливаем тип (J умножаем на 10, чтобы не было повторений)
                    error = false;
                }
            }
        }
        for (int i=0; i<size; i++) // начинаем сортировать элементы по возрастанию типа (сортировка пузырьком)
        {
            for (int j=0; j<i; j++)
            {
                if(skleikaSort[i].type < skleikaSort[j].type)
                {
                    skleyka temp = skleikaSort[i];
                    skleikaSort[i] = skleikaSort[j];
                    skleikaSort[j] = temp;
                }
            }
        }
        QStringList sortSkleykiList;
        for (int i=0; i<size; i++)
        {
            sortSkleykiList.append(skleikaSort[i].text);
            for (int j=0; j<skleikaSort[i].text.size(); j++)
            {
                tableWidgetFunctions::setTWText(skleikiTableWidget, i, j, skleikaSort[i].text[j]);
            }
        }
//        qDebug() << "значения на выходе: " << sortSkleykiList;
    }
    delete [] skleikaSort;
    skleikaSort = nullptr;
}


void MainWindow::on_pushButton_A_clicked()
{
    if(editor)
    {
//        addSymvol("A");
        addSymvol("A");
    }
}

void MainWindow::on_pushButton_B_clicked()
{
    if(editor)
    {
        addSymvol("B");
    }
}

void MainWindow::on_pushButton_C_clicked()
{
    if(editor)
    {
        addSymvol("C");
    }
}

void MainWindow::on_pushButton_D_clicked()
{
    if(editor)
    {
        addSymvol("D");
    }
}

void MainWindow::on_pushButton_notA_clicked()
{
    if(editor)
    {
        addSymvol("A",true);
    }
}

void MainWindow::on_pushButton_notB_clicked()
{
    if(editor)
    {
        addSymvol("B",true);
    }
}

void MainWindow::on_pushButton_notC_clicked()
{
    if(editor)
    {
        addSymvol("C",true);
    }
}

void MainWindow::on_pushButton_notD_clicked()
{
    if(editor)
    {
        addSymvol("D",true);
    }
}

void MainWindow::on_pushButton_konunkciya_clicked()
{
    if(editor)
    {
        addSymvol("*");
    }
}

void MainWindow::on_pushButton_disunkciya_clicked()
{
    if(editor)
    {
        addSymvol("+");
    }
}

void MainWindow::on_pushButton_skobkaOpen_clicked()
{
    if(editor)
    {
        addSymvol("(");
    }
}

void MainWindow::on_pushButton_skobkaClose_clicked()
{
    if(editor)
    {
        addSymvol(")");
    }
}

void MainWindow::on_pushButton_delete_clicked()
{
    if(editor)
    {
//        editor->deleteSelectedColumns();
        editor->deleteSelectedColumns(ui->tableWidget_logicEditor);
    }
}

void MainWindow::on_pushButton_clear_clicked()
{
    if(editor)
    {
        editor->clearTable();
    }
}

//void MainWindow::on_pushButton_reDrowFormula_clicked()
//{
//    if(editor)
//    {
//        editor->clearTable();
//    }
//}

void MainWindow::on_pushButton_proverka_clicked()
{
    if(editor)
    {
        QString function_16;
        bool ok = false;
        function_16 = ui->lineEdit_function_16->text(); // берем текст функциии
        function_16.toInt(&ok, 16); // проверяем, можно-ли её перевести из 16й системы счисления
        if(!ok)
        {
            QMessageBox::warning(this, "Ошибка!", "Это не 16-я функция!");
            return;
        }
        if(function_16.count() != 4) // проверяем, что в функции 4 переменных
        {
            QMessageBox::warning(this, "Ошибка!", "Функция должна состоять из 4х переменных!");
            return;
        }
        mdnfMacKlassky *mdnf = new mdnfMacKlassky;
        if(!mdnf->setFunction(function_16))
        {
            QMessageBox::warning(this, "Ошибка!", "Программа не может посчитать данную функцию :(");
            return;
        }
        if(!mdnf->canCalculateMnf(1))
        {
            QMessageBox::warning(this, "Ошибка!", "Программа не может посчитать МДНФ данной функции :(");
            return;
        }
        if(mdnf->isMnf(editor->getFormulaText(), 1))
        {
            QMessageBox::information(this, "Мак-Класски", "Правильно");
            return;
        }
        else
        {
            QMessageBox::critical(this, "Ошибка!", "Ошибка!");
            return;
        }
    }
}


void MainWindow::on_comboBox_formulas_currentIndexChanged(const QString &arg1)
{
    editor->clearTable();
    editor->setVirazhenie(arg1);
}

void MainWindow::on_radioButton_mknf_clicked()
{
    on_pushButton_calclulate_clicked();
}

void MainWindow::on_radioButton_mdnf_clicked()
{
    on_pushButton_calclulate_clicked();
}

void MainWindow::on_pushButton_addFormula_ToTable_clicked()
{
    ui->tableWidget_formulas->setColumnCount(3);
    int row = ui->tableWidget_formulas->rowCount();
    ui->tableWidget_formulas->setRowCount(row+1);
    QString func = ui->lineEdit_function_16->text();
    QString formula = editor->getFormulaText();
    QString student_name = ui->lineEdit_studentfio->text();
    ui->tableWidget_formulas->setItem(row, 0, new QTableWidgetItem(func));
    ui->tableWidget_formulas->setItem(row, 1, new QTableWidgetItem(formula));
    ui->tableWidget_formulas->setItem(row, 2, new QTableWidgetItem(student_name));
    ui->tableWidget_formulas->setColumnWidth(2, 200);
    ui->tableWidget_formulas->scrollToBottom();

}

void MainWindow::on_pushButton_saveTable_clicked()
{
    QString table_data;
    for (int i=0; i<ui->tableWidget_formulas->rowCount(); i++)
    {
        QString row_text;
        for (int j=0; j<ui->tableWidget_formulas->columnCount(); j++)
        {
            if(!row_text.isEmpty())
            {
                row_text.append("\t");
            }
            row_text.append(ui->tableWidget_formulas->item(i, j)->text());
        }
        table_data.append(row_text);
        table_data.append("\r\n");
    }
    QFile file("functions.txt");
    file.open(QIODevice::WriteOnly);
    file.write(table_data.toUtf8());
    file.close();
}

void MainWindow::on_pushButton_readfromfile_clicked()
{
    QFile file("functions.txt");
    if(file.open(QIODevice::ReadOnly))
    {
        QString text = file.readAll();
        QStringList rows = text.split("\r\n", Qt::SplitBehavior(Qt::SkipEmptyParts));
        ui->tableWidget_formulas->clear();
        ui->tableWidget_formulas->setRowCount(rows.size());
        for (int i=0; i<rows.size(); i++)
        {
            QString line = rows.at(i);
            QStringList cols = line.split("\t", Qt::SplitBehavior(Qt::SkipEmptyParts));
            for (int j=0; j<cols.size(); j++)
            {
                ui->tableWidget_formulas->setColumnCount(cols.size());
                ui->tableWidget_formulas->setItem(i, j, new QTableWidgetItem(cols.at(j)));
            }
        }
       file.close();
    }
    ui->tableWidget_formulas->setColumnWidth(2, 200);
    ui->tableWidget_formulas->scrollToBottom();
}

void MainWindow::addSymvol(QString symvol, bool inversiya)
{
    if(editor == nullptr) // если не создан объект редакора формул
    {
        return;
    }
    symvol = symvol.toUpper();
    qDebug() << symvol;
    if(autoInput) // если включена функция автоматического ввода
    {
        if(((symvol >= "A") && (symvol <= "D")) || (symvol == ")")) // если символ - буква или символ
        {
            QString formula = editor->getFormulaText(); // получаем формулу
            qDebug() << "formula = " << formula;
            if(formula != "ERROR")
            {
                qDebug() << "count =" << formula.count();
                formula = formula.toUpper();
                QString lastSymvol = formula.at(formula.count() -1); // получаем последний символ
                if(editor->isEditChoseItem()) // если происходит редактирование выбранного элемента
                {
                    int selectedItem = -1;
                    int selectedCount = 0;
                    editor->getSelectedItemNumber(ui->tableWidget_logicEditor, selectedItem, &selectedCount);
                    if(selectedCount > 1)
                    {
//                        warningError("Выберите только один символ, после которого необходимо добавление олперанда");
                        QMessageBox::warning(this, "Ошибка", "Выберите только один символ, после которого необходимо добавление олперанда");
                        return;
                    }
                    qDebug() << "номер выбранного столбца: " << selectedItem;
                    qDebug() << "Всего символов в строке: " << formula.count();
                    lastSymvol = ui->tableWidget_logicEditor->item(1, selectedItem)->text();
                }
                qDebug() << "last symvol = " << lastSymvol;
                int typeMin = -1;
                if(ui->radioButton_mknf->isChecked())
                {
                    typeMin = 0;
                }
                else if(ui->radioButton_mdnf->isChecked())
                {
                   typeMin = 1;
                }
                if(typeMin == -1)
                {
                    QMessageBox::warning(this, "Ошибка!", "Выберите тип минимизации");
                    return;
                }
                if((lastSymvol >= "A") && (lastSymvol <= "D")/* && (symvol >= "A") && (symvol <= "D")*/) // если предыдущий символ - буква
                {
                    if(symvol > lastSymvol)
                    {
                        if(typeMin == 0)
                        {
                            editor->addSymvol("+");
                        }
                        else if(typeMin == 1)
                        {
                            editor->addSymvol("*");
                        }
                    }
                    else
                    {
                        editor->addSymvol(")");
                        if(typeMin == 0)
                        {
                            editor->addSymvol("*");
                        }
                        else if(typeMin == 1)
                        {
                            editor->addSymvol("+");
                        }
                        editor->addSymvol("(");
                    }
                }
                else if (lastSymvol == ")") // если закрывающаяся скобка
                {
                    if(typeMin == 0)
                    {
                        editor->addSymvol("*");
                    }
                    else if(typeMin == 1)
                    {
                        editor->addSymvol("+");
                    }
                    editor->addSymvol("(");
                }
            }
            else // если у нас первый символ, то ставим открывающуюся скобку
            {
                editor->addSymvol("(");
            }
        }
    }
    qDebug() << "formula = " << editor->getFormulaText();
    editor->addSymvol(symvol, inversiya);
    if(symvol == "D") // если символ D, то сразу закрываем скобку
    {
        editor->addSymvol(")");
    }
}

void MainWindow::on_pushButton_proverka_all_clicked()
{
    qDebug() << "Нажато!";
    for (int i=0; i<ui->tableWidget_formulas->rowCount(); i++)
    {
        qDebug() << "Начало цикла";
        QString func = ui->tableWidget_formulas->item(i, 0)->text();
        QString formula = ui->tableWidget_formulas->item(i, 1)->text();
//        QString student_name = ui->lineEdit_studentfio->text();
        mdnfMacKlassky mdnf;
        auto setColorLine = [=](QColor color)
        {
            for (int j=0; j<ui->tableWidget_formulas->columnCount(); j++)
            {
                qDebug() << "i = " << i << "j = " << j;
                if(ui->tableWidget_formulas->item(i, j))
                {
                    try
                    {
                        ui->tableWidget_formulas->item(i, j)->setBackground(QBrush(color));
                    }  catch (std::exception &e)
                    {
                        qDebug() << e.what();
                    }
                }

            }
        };
        qDebug() << "устанавливаем функцию";
        bool ok = mdnf.setFunction(func);
        if(!ok)
        {
            setColorLine(Qt::red);
            continue;
        }
        qDebug() << "Проверяем корректность расчёта";
        if(mdnf.isMnf(formula, 0) || mdnf.isMnf(formula, 1))
        {
            setColorLine(Qt::green);
        }
        else
        {
            setColorLine(Qt::red);
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    auto table = ui->tableWidget_formulas;
    for (int i=0; i<table->rowCount(); i++)
    {
        bool selected = false;
        for (int j=0; j<table->columnCount(); j++)
        {
            auto item = table->item(i, j);
            if(item)
            {
                if(item->isSelected())
                {
                    selected = true;
                    break;
                }
            }
        }
        if(selected)
        {
            table->removeRow(i);
            i--;
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    auto table = ui->tableWidget_formulas;
    for (int i=0; i<table->rowCount(); i++)
    {
        for (int j=0; j<table->columnCount(); j++)
        {
            auto item = table->item(i, j);
            if(item)
            {
                item->setBackground(QBrush(Qt::white));
            }
        }
    }
}
