#include "tablewidgetfunctions.h"

tableWidgetFunctions::tableWidgetFunctions(QObject *parent) : QObject(parent)
{

}

void tableWidgetFunctions::center_text_in_table(QTableWidget *tbw)
{
    if(tbw == nullptr)
    {
        return;
    }
    int rows = tbw->rowCount(); // получили количество строк
    int cols = tbw->columnCount(); // получаем количество столбцов
    //СКРЫТО qDebug() << "Выравниваем. Количество строк: "  << rows << "Количество столбцов: "  << cols;
    if(rows == 0 || cols == 0)
    {
        return;
    }
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<cols; j++)
        {
            if(tbw->item(i, j) != nullptr)
            {
                tbw->item(i, j)->setTextAlignment(Qt::AlignCenter); // выравниаем все элементы по центру
            }
        }
    }
}

void tableWidgetFunctions::setQStringListToTW(QTableWidget *&tableWidget, QStringList list)
{
    int rows = list.size(); // получаем размер списка
    if(!rows)
    {
        tableWidget->setRowCount(0);
        tableWidget->setRowCount(0);
        //СКРЫТО qDebug() << "Недостоточно строк для вывода QStringList в QTableWIdget";
        return;
    }
    int cols = list[0].count(); // получаем размер 0го элемента
    for (int i=0; i<rows; i++)
    {
        int razmer = list[i].count(); // получаем размер текущего элемента
        if(razmer > cols) // если один элемент больше предыддущего
        {
            cols = razmer; // увеличиваем количество столбцов
        }
    }
    if(!cols)
    {
        tableWidget->setRowCount(0);
        tableWidget->setRowCount(0);
        //СКРЫТО qDebug() << "Недостоточно столбцов для вывода QStringList в QTableWIdget";
        return;
    }
    tableWidget->setRowCount(rows);
    tableWidget->setColumnCount(cols);
    for (int i=0; i<rows; i++)
    {
//        QString colorFlag = ""; // флаг цвета. На данный момент это r и y

//        //СКРЫТО qDebug() << "Выставляемый цвет: " << color << color.red() << color.green() << color.blue();
        QStringList element = list[i].split("", Qt::SplitBehavior(Qt::SkipEmptyParts)); // разбиваем элемент спика на элементы
        int elementSize = element.size(); // получаем длину строки этого элемента
        for (int j=0; j<elementSize; j++)
        {
            setTWText(tableWidget, i, j, element[j]); // устаналиваем элемент
        }
    }
}

void tableWidgetFunctions::addQStringToTWOneSymwolInItem(QTableWidget *&tableWidget, QString str)
{
    if(tableWidget == nullptr || str == "")
    {
        //СКРЫТО qDebug() << "Нельзя добавить строку в таблицу!" << str;
        return;
    }
    addRow(tableWidget, str.split("", Qt::SplitBehavior(Qt::SkipEmptyParts))); // разбиваем строку списка по символам и передаём функции как список строки, где 1 символ равен одному item
}

void tableWidgetFunctions::addQStringListToTWOneSymwolInItem(QTableWidget *&tableWidget, QStringList list)
{
    if(tableWidget == nullptr || list.size() == 0)
    {
        //СКРЫТО qDebug() << "Нельзя добавить список в таблицу!";
        return;
    }
    int s = list.size(); // получаем размер списка
    for (int i=0; i<s; i++)
    {
        addQStringToTWOneSymwolInItem(tableWidget, list[i]); // Добавляем по строчкам в таблицу
    }
}


void tableWidgetFunctions::clearTW(QTableWidget *&tableWidget)
{
    int rows = tableWidget->rowCount(); // получаем количество строк
    int cols = tableWidget->columnCount(); // получаем количество столбцов
    if((!rows) || (!cols))
    {
        tableWidget->clear();
        tableWidget->setRowCount(0);
        tableWidget->setColumnCount(0);
        return;
    }
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<cols; j++)
        {
            if(!tableWidget->item(i, j))  // если памят ьпод ячейку выделена
            {
                delete tableWidget->item(i, j); // удаляем память
            }
        }
    }
    tableWidget->clear();
    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(0);
}

bool tableWidgetFunctions::getTWItemText(QTableWidget *&tableWidget, int row, int col, QString &outputString)
{
    // стартовые проверки входных параметров
    if(*&tableWidget == nullptr) // если указатель пустой
    {
        //СКРЫТО qDebug() << "Указаетль на tableWidget пустой";
        return false;
    }
    if(row < 0) // если номер искомой строки меньше 0
    {
        //СКРЫТО qDebug() << "Номер строки меньше нуля. Невозможно получить текст" << row;
        return false;
    }
    if(col < 0) // если номер искомой строки меньше 0
    {
        //СКРЫТО qDebug() << "Номер столбца меньше нуля. Невозможно получить текст" << col;
        return false;
    }
    int rows = tableWidget->rowCount(); // получаем количество строк
    int cols = tableWidget->columnCount(); // получаем количество столбцов
    if(rows == 0) // если нет строк
    {
        //СКРЫТО qDebug() << "В таблице нет строк";
        return false;
    }
    if(cols == 0) // если нет столбцов
    {
        //СКРЫТО qDebug() << "В таблице нет столбцов";
        return false;
    }
    if(row >= rows) // если строка, к которой обращаемся не существует
    {
        //СКРЫТО qDebug() << "Невозможно получить текст из данной строки" << row << rows;
        return false;
    }
    if(row >= rows) // если столбец, к которому обращаемся не существует
    {
        //СКРЫТО qDebug() << "Невозможно получить текст из данного столбца" << col << cols;
        return false;
    }
    // получаем значение
    if(tableWidget->item(row, col) == nullptr) // если под элемент не выделена память
    {
        //СКРЫТО qDebug() << "Под элемент " << row << col << " не выделена память";
        return false;
    }
    outputString = tableWidget->item(row, col)->text(); // полчаем текст из ячейки
    return true;
}

bool tableWidgetFunctions::getTWTextList(QTableWidget *&tableWidget, QStringList &outputList)
{
    // стартовые проверки входных параметров
    if(*&tableWidget == nullptr) // если указатель пустой
    {
        //СКРЫТО qDebug() << "Указаетль на tableWidget пустой";
        return false;
    }
    int rows = tableWidget->rowCount(); // получаем количество строк
    int cols = tableWidget->columnCount(); // получаем количество столбцов
    if(rows == 0) // если нет строк
    {
        //СКРЫТО qDebug() << "В таблице нет строк";
        return false;
    }
    if(cols == 0) // если нет столбцов
    {
        //СКРЫТО qDebug() << "В таблице нет столбцов";
        return false;
    }
    QStringList listRows; // создаем список, в который занесем элементы
    for (int i=0; i<rows; i++)
    {
        QString string = ""; // создаем временную строку для создания строки из элементов столбцов
        for (int j=0; j<cols; j++)
        {
            QString str = ""; // врменная переменная для хранения содержимого ячейки
            bool ok = false; // переменная для хранения корректности получения значения
            ok = getTWItemText(tableWidget, i, j, str); // получаем текст из ячейки
            if(!ok) // если возникла проблема при получении текста
            {
                //СКРЫТО qDebug() << "[getTWTextList] Не удалдось получить текст из ячейки " << i << j;
                return false;
            }
            string.append(str); // добавляем элемент в общую строку
        }
        listRows.append(string); // добавляем строку в список
    }
    outputList = listRows; // передаем список назад
    return true;
}

void tableWidgetFunctions::addRow(QTableWidget *&tableWidget, QStringList rowList, int colorRed, int colorGreen, int colorBlue)
{
    if(tableWidget == nullptr)
    {
        //СКРЫТО qDebug() << "addRow Таблицы не существует";
        return;
    }
    int rows = tableWidget->rowCount(); // получаем количество строк
    int cols = tableWidget->columnCount(); // получаем количество столбцов
    int rowListSize = rowList.size(); // получаем количество элементов в списке
    if(cols < rowListSize) // если количество столбцов меньше количества элементов в списке
    {
        cols = rowListSize; // устанавливаем число столбцов равное количеству элементов в списке
        tableWidget->setColumnCount(cols); // задаем количество столбцов
    }
    rows += 1; // обавляем 1 строку
    tableWidget->setRowCount(rows); // устанавливаем количество строк
    for (int i=0; i<cols; i++)
    {
        tableWidget->setItem(rows-1, i, new QTableWidgetItem(rowList[i])); // выделяем память и задаём текст
        if((colorBlue >= 0 && colorBlue <= 255) && (colorGreen >= 0 && colorGreen <= 255) && (colorRed >= 0 && colorRed <= 255))
        {
            tableWidget->item(rows-1, i)->setBackground(QColor(colorRed, colorGreen, colorBlue)); // устанавливаем цвет ячейки
        }
    }
    center_text_in_table(tableWidget); // выравниваем ячейки
}


void tableWidgetFunctions::copyTableWidget(QTableWidget *tableWidgetInput, QTableWidget *&tableWidgetOutput, bool skipIdenticalLines)
{
    QStringList inputList; // список для получения значений из первой таблицы
    bool flag = getTWTextList(tableWidgetInput, inputList); // получаем список строк
    if(!flag)
    {
        //СКРЫТО qDebug() << "copyTableWidget - ошибка!";
        return;
    }
    if(skipIdenticalLines) // если нужно исключить повторяющиеся элементы
    {
        QStringList sortList; // создаём новый список для эначений без повтора
        for (int i=0; i<inputList.size(); i++)
        {
            bool contain = false; // флаг наличия элемента в новом списке
            for (int j=0; j<sortList.size(); j++)
            {
                if(sortList.at(j) == inputList.at(i)) // если в новом списке уже есть этот элемент
                {
                    contain = true; // ставим, что элемент уже есть
                    break;
                }
            }
            if(!contain) // если элемент не содержится в списке
            {
                sortList.append(inputList.at(i)); // добавляем элемент в новый список
            }
        }
        inputList = sortList; // заменяем исходный список на новый, без повторений
    }
    addQStringListToTWOneSymwolInItem(tableWidgetOutput, inputList); // выводим элементы в таблицу
    center_text_in_table(tableWidgetOutput); // выравниваем ячейки

//    //СКРЫТО qDebug() << "начало функции copyTableWidget";
//    int rows = tableWidgetInput->rowCount(); // получаем количество строк в исходном виджете
//    int cols = tableWidgetInput->columnCount(); // получаем количество столбцов в исходном виджете
//    //СКРЫТО qDebug() << "получили количество строк и столбцов";
//    tableWidgetOutput->setRowCount(rows); // устанавливаем количество строк такое же, как в исходном виджете
//    tableWidgetOutput->setColumnCount(cols); // устанавливаем количество
//    //СКРЫТО qDebug() << "Установили корличество столбцов и строк";
//    if(rows > 0 && cols > 0) // если есть хоть один столбиц или одна строка
//    {
////        QStringList horisontalHeaders;
////        QStringList verticalHeaders;
//        QStringList tableWidgetInputList; // создаем список для получения значений из таблицы импорта
//        getTWTextList(tableWidgetInput, tableWidgetInputList); // получаем значения из таблицы
//        int fillRowsCount = 0; // переменная для хранения, какая строка должна заполняться следующей в таблице экспорта
//        for (int i=0; i< rows; i++) // проходим по каждой строке
//        {
//            QStringList tableWidgetOutputList; // создаем список для получения значений из таблицы экспорта
//            getTWTextList(tableWidgetOutput, tableWidgetOutputList); // получаем значения из таблицы
//            if(skipIdenticalLines) // если нужно пропускать одинаковые строки
//            {
//                bool contain = false;
//                for (int h=0; h<tableWidgetOutputList.size(); h++) // проходим по каждому элементу, который уже есть в таблице вывода
//                {
//                    if(tableWidgetInputList.at(i) == tableWidgetOutputList.at(h)) // если елемент совпадает с тем, что уже есть
//                    {
//                        contain = true;
//                        break;
//                    }
//                }
//                if(contain) // если элемент содержится
//                {
//                    continue; // пропускам итерацию цикла
//                    tableWidgetOutput->setRowCount(tableWidgetOutput->rowCount()-1); // устанавливаем количество строк на 1 меньше, т.к. одно значение уже пропущено
//                }
//            }
//            //СКРЫТО qDebug() << "строка" << i;
////            verticalHeaders.append(tableWidgetInput->verticalHeaderItem(i)->text()); // добавляем название вертикального элемента в список
//            for (int j=0; j<cols; j++) // проходим по каждому столбцу
//            {
//                //СКРЫТО qDebug() << "столбец" << j;
////                horisontalHeaders.append(tableWidgetInput->horizontalHeaderItem(j)->text());// добавляем название горизонтального элемента в список
//                QTableWidgetItem *item = tableWidgetInput->item(i, j); // берем ящейку
//                if(item != nullptr) // если она не пустая
//                {
//                    //СКРЫТО qDebug() << "Элемент не пустой";
//                    QString text = tableWidgetInput->item(i, j)->text();
//                    tableWidgetOutput->setItem(fillRowsCount, j, new QTableWidgetItem(text)); // копируем её
//                }
//                else
//                {
//                    //СКРЫТО qDebug() << "элемент пустой";
//                    tableWidgetOutput->setItem(fillRowsCount, j, new QTableWidgetItem("")); // иначе создаем новую
//                }
//            }
//            fillRowsCount++; // увеличиваем номер строки
//        }
//        //СКРЫТО qDebug() << "прошли по всем строчкам";
////        tableWidgetOutput->setHorizontalHeaderLabels(horisontalHeaders); // устанавиваем горизонтальные заголовки
////        tableWidgetOutput->setVerticalHeaderLabels(verticalHeaders); // устанавливаем вертикальные заголовки
//    }
//    else {
//        //СКРЫТО qDebug() << "недостаточн острок или столбцов" << rows << cols;
//    }
//    //СКРЫТО qDebug() << "Функция copyTableWidget завершена";
//    center_text_in_table(tableWidgetOutput); // выравниваем ячейки
}

void tableWidgetFunctions::deletelastRow(QTableWidget *&tableWidget)
{
    if(tableWidget == nullptr)
    {
        //СКРЫТО qDebug() << "указатель пуст";
        return;
    }
    int rows = tableWidget->rowCount();
    if(rows <= 0)
    {
        //СКРЫТО qDebug() << "нет строк для удаления";
        return;
    }
    tableWidget->setRowCount(rows-1);
    center_text_in_table(tableWidget); // выравниваем ячейки
}

void tableWidgetFunctions::setTableWidgetItemsSize(QTableWidget *&tableWidget, int rowSize, int columnSize)
{
    if(tableWidget == nullptr)
    {
        //СКРЫТО qDebug() << "Невозможно установить размер ячеек!";
        return;
    }
    int rows = tableWidget->rowCount();
    int cols = tableWidget->columnCount();
    if(rows == 0 || cols == 0)
    {
        //СКРЫТО qDebug() << "Невозможно установить размер ячеек! Недоастаточно строк/столбцов";
        return;
    }
    for(int i=0; i<rows; i++)
    {
        tableWidget->setRowHeight(i, rowSize); // задаем высоту строк
    }
    for(int j=0; j<cols; j++)
    {
        tableWidget->setColumnWidth(j, columnSize); // задаем ширину столбцов
    }
}


QString tableWidgetFunctions::getQStringByTableWidget(QTableWidget *tableWidget)
{
    QStringList list; // создаём список, в который запишем строки из таблицы
    getTWTextList(tableWidget, list); // получаем список из строк из таблицы
    if(list.size() == 0) // если в списке ничего нет
    {
        return ""; // вызврпащаем пустоту
    }
    QString str; // строка, в которую запишем весь список
    for (int i=0; i<list.size(); i++) // проходим по всем элементам
    {
        str.append(list[i]); // добавляем каждую строку
        if(i != list.size()-1) // до -1, т.к. к полсденему элементу не надо добавлять знак разделения
        {
            str.append("|"); // добавляем разделитель
        }

    }
//    str.append(list[list.size()-1]); // добавляем последнюю строку
    return str;
}

void tableWidgetFunctions::setTWText(QTableWidget *&tableWidget, int row, int col, QString text, QColor *itemColor)
{
    int rows = tableWidget->rowCount();
    int cols = tableWidget->columnCount();
    //СКРЫТО qDebug() << "Количество строк (Ф) = " << rows;
    //СКРЫТО qDebug() << "Количество столбцов (Ф) = " << cols;

    if((rows <= 0) || (cols <= 0))
    {
        //СКРЫТО qDebug() << "В таблице недолстаточно строк или столбцов";
        return;
    }
    if(row >= rows)
    {
        //СКРЫТО qDebug() << "Номер строки больше, чем количество столбцов";
        return;
    }
    if(col >= cols)
    {
        //СКРЫТО qDebug() << "Номер столбца больше, чем количество столбцов";
        return;
    }
    //СКРЫТО qDebug() << "Выставляем значение" << row << col << text;
//    if(tableWidget->item(row, col) == nullptr) // если нет элекмента, создаем его
//    {
//        tableWidget->setItem(row, col, new QTableWidgetItem); // выделяем память
//        tableWidget->item(row, col)->setBackground(itemColor); // устанавливаем цвет
//    }
    tableWidget->setItem(row, col, new QTableWidgetItem(text)); // записывем значение
    if(itemColor != nullptr)
    {
       tableWidget->item(row, col)->setBackground(QBrush(*itemColor)); // устанавливаем цвет
    }

//    //СКРЫТО qDebug() << "цвет установлен " << itemColor << itemColor.red() << itemColor.green() << itemColor.blue();
    center_text_in_table(tableWidget); // выравниваем ячейки
}

