#include "mainwindow.h"

bool MainWindow::proverkaTableOfTrue()
{
    allowSave = true; // разрешаем сохранение
    return proverkaTable(tableWidgetTot, mdnf->getSschTableList());
}

bool MainWindow::proverkaOnesOnly()
{
    return proverkaTable(tableWidgetOnesOnlyEditing, mdnf->getListOnes(typeMin));
}

bool MainWindow::proverkaSkleyki1()
{
    return proverkaTable(tableWidgetsSkleykiEditing[0], mdnf->getSkleyki1(typeMin), true);
}

bool MainWindow::proverkaSkleyki2()
{
    return proverkaTable(tableWidgetsSkleykiEditing[1], mdnf->getSkleyki2(typeMin), true);
}

bool MainWindow::proverkaSkleyki3()
{
    return proverkaTable(tableWidgetsSkleykiEditing[2], mdnf->getSkleyki3(typeMin), true);
}

bool MainWindow::proverkaKartaMinimizacii()
{
    qDebug() << "Проверяем карту минимизации";
    int rows = tableWidgetKartaMinimizacii->rowCount(); // получаем количество строк
    int cols = tableWidgetKartaMinimizacii->columnCount(); // получаем количество строк
    if(!rows || !cols)
    {
//        criticalError("Таблица пустая!");
        qDebug() << "Нет ячеек!";
        return false;
    }
    QStringList horizontalList; // создаем список заголовков столбцов
    for (int i=0; i<cols; i++)
    {
        horizontalList.append(tableWidgetKartaMinimizacii->horizontalHeaderItem(i)->text());
        qDebug() << "horisontal header: " << horizontalList[i];
    }
    QStringList verticalList; // создаем список заголовков строк
    for (int i=0; i<rows; i++)
    {
        verticalList.append(tableWidgetKartaMinimizacii->verticalHeaderItem(i)->text());
        qDebug() << "vertical header: " << verticalList[i];
    }
    qDebug() << "Начинаем проверку.";
    qDebug() << "rows =" << rows << "horizontalList =" << horizontalList.size() << "cols =" << cols << "verticalList =" << verticalList.size();
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<cols; j++)
        {
            qDebug() << "Цикл " << i << j;
            int count_v = verticalList[i].count(); // количество символов в элементе столбца
            int count_h = horizontalList[j].count(); // количество символов в элементе строки
            if(count_v != count_h) // если количество символов почему-то не совпадает
            {
                qDebug() << "Не совпадает количество элементов строки/столбца" << i << j;
                return false;
            }
            qDebug() << "Проверили на совпадение количества символов";
//            bool *ok = new bool [count_v]; // есть ли совпадение в этом столбце с этой строкой
            int sovpadenie = 0;
            for (int k=0; k<count_v; k++) // проходим по каждому символу
            {
//                qDebug() << "k =" << k << verticalList[i] << horizontalList[j];
                bool ok = ((verticalList[i].at(k) == "X") || (verticalList[i].at(k) == horizontalList[j].at(k))); // сохраняем результат сравнения (true/false)
                sovpadenie += static_cast<int>(ok); // прибавляем результат (+1 если true, +0 если false)
            }
            qDebug() << "Посчитали совпадение";
            QTableWidgetItem *item = tableWidgetKartaMinimizacii->item(i, j); // создаем указатель на ячейку
            if(item == nullptr)
            {
                qDebug() << "Ячейка не существует! Столбец" << j << ", строка" << i;
                return false;
            }
            if(sovpadenie == count_h) // если все символы совпадают
            {
                if(item->text() != "+") // но в карте минимизации этого + нет
                {
                    qDebug() << "Ошибка! " <<  verticalList[i] << horizontalList[j] << "символы совпадают, но нет +";
//                    warningError();
                    return false;
                }
            }
            else // если символы не совпадают, т.е. + быть не должно
            {
                if(item->text() == "+") // но в карте минимизации + есть
                {
                    qDebug() << "Ошибка! " <<  verticalList[i] << horizontalList[j] << "не должно быть +";
//                    warningError();
                    return false;
                }
            }
        }
    }
    return true;
}

bool MainWindow::proverkaItogMdnf()
{
//    if(lineEdit_itogMdnf == nullptr)
//    {
//        qDebug() << "lineEdit_itogMdnf == nullptr";
//        return false;
//    }
//    QString userMdnf = lineEdit_itogMdnf->text(); // получаем текст из строки, куда вводится МДНФ
    QString userMdnf = editor->getFormulaText(); // получаем формулу из редатора
    qDebug() << "МДНФ ввдённая пользователем:" << userMdnf;
    if(userMdnf.isEmpty()) // если строка пустая
    {
//        criticalError("Введите МДНФ в поле ввода!"); // выдаем ошибку
        qDebug() << "Не введена МДНФ";
        return false;
    }
    int skobkaStartCount = 0; // переменная для подсчёта открывающихся скобок - (
    int skobkaEndCount = 0; // переменная для подсчёта закрывающихся скобок - )
    for (int i=0; i<userMdnf.count(); i++)
    {
        if(userMdnf.at(i) == "(")
        {
            skobkaStartCount++;
        }
        if(userMdnf.at(i) == ")")
        {
            skobkaEndCount++;
        }
    }
    if(skobkaStartCount != skobkaEndCount) // если количество скобок разное
    {
        qDebug() << "Количество скобок разное " << skobkaStartCount << skobkaEndCount;
//        warningError("Ошибка! Проверьте скобки!");
        return false;
    }
    if(mdnf->isMnf(userMdnf, typeMin))
    {
//        message();
        return true;
    }
    else
    {
//        warningError();
        return false;
    }
}
