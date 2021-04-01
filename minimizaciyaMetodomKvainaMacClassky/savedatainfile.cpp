#include "mainwindow.h"

#define LINE_COLOR_ON true
#define LINE_COLOR_OFF false

void MainWindow::saveDataToFile()
{
    QFile fileOutput(FILE_STUDENT_WORK);
    if(!fileOutput.open(QIODevice::WriteOnly))
    {
        qDebug() << "Не удалось открыть файл для записи!";
        warningError("Не удалось выполнить автоматическое сохранение данных. Проверьте доступность файла " + FILE_STUDENT_WORK);
        return;
    }
    else
    {
        QString str_name = "name=" + studentName; // сохраняем ФИО студента
        QString str_group = "group=" + StudentGroup; // сохраняем группу студента
        QString str_step = "step=" + QString::number(step); // сохраняем номер текущего шага
        qDebug() << "Последний сохраненный этап: " << step;
        QString str_func2 = "lineEditFunc_2=" + lineEditFunc_2->text(); // сохраняем то, что введено в строку функции, записанной в 2 ССЧ
        QString str_tot = "tableWidgetTot=" + getQStringByTableWidget(tableWidgetTot, false, false); // получаем строку из таблицы истинности
        QString str_ones_only_editing = "tableWidgetOnesOnlyEditing=" + getQStringByTableWidget(tableWidgetOnesOnlyEditing, LINE_COLOR_ON, false);
        QString str_tableWidget_one_only = "tableWidget_one_only=" + getQStringByTableWidget(tableWidgetOnesOnly, LINE_COLOR_ON, false);
        QString str_tableWidgetsSkleykiEditing_0 = "str_tableWidgetsSkleykiEditing_0=" + getQStringByTableWidget(tableWidgetsSkleykiEditing[0], LINE_COLOR_ON);
        QString str_tableWidgetsSkleykiEditing_1 = "str_tableWidgetsSkleykiEditing_1=" + getQStringByTableWidget(tableWidgetsSkleykiEditing[1], LINE_COLOR_ON);
        QString str_tableWidgetsSkleykiEditing_2 = "str_tableWidgetsSkleykiEditing_2=" + getQStringByTableWidget(tableWidgetsSkleykiEditing[2], LINE_COLOR_ON);
        QString str_tableWidgetsSkleyki_0 = "str_tableWidgetsSkleyki_0=" + getQStringByTableWidget(tableWidgetsSkleyki[0], LINE_COLOR_ON, false);
        QString str_tableWidgetsSkleyki_1 = "str_tableWidgetsSkleyki_1=" + getQStringByTableWidget(tableWidgetsSkleyki[1], LINE_COLOR_ON, false);
//        QString str_tableWidgetKartaMinimizacii = "tableWidgetKartaMinimizacii=" + getQStringByTableWidget(tableWidgetKartaMinimizacii);
        QString str_tableWidgetKartaMinimizacii = "tableWidgetKartaMinimizacii=";
        int rows = tableWidgetKartaMinimizacii->rowCount(); // получаем количество строк в карте минимизации
        int cols = tableWidgetKartaMinimizacii->columnCount(); // получаем количество столбцов в карте минимизации
        for (int i=0; i<rows; i++) // проходим по каждой строке
        {
            for (int j=0; j<cols; j++) // проходим по каждому столбцу
            {
                if(tableWidgetKartaMinimizacii->item(i, j) != nullptr) // если выделена память, значит в ячейке что-то есть
                {
                    if(tableWidgetKartaMinimizacii->item(i, j)->text().contains("+")) // если содержится +
                    {
                        str_tableWidgetKartaMinimizacii.append("+"); // добавляем + в строку
//                        qDebug() << "+ в строке" << i << "столбеце" << j;
                    }
                    else
                    {
                        str_tableWidgetKartaMinimizacii.append("-"); // добавляем - в строку
                    }
                    QColor itemColor = tableWidgetKartaMinimizacii->item(i, j)->background().color(); // получаем фоновый цвет
                    if(itemColor == *redColor)
                    {
                        str_tableWidgetKartaMinimizacii.append("r");
                    }
                    else if(itemColor == *greenColor)
                    {
                        str_tableWidgetKartaMinimizacii.append("g");
                    }
                    else
                    {
                        str_tableWidgetKartaMinimizacii.append("w");
                    }
                }
                else
                {
                    str_tableWidgetKartaMinimizacii.append("-"); // добавляем - в строку
                }
//                qDebug() << "Сохранение карты покрытия. Строка" << i << "столбец" << j << ". Значение: " << str_tableWidgetKartaMinimizacii;
            }
            if(i != rows-1)
            {
                str_tableWidgetKartaMinimizacii.append("|"); // добавляем знак переноса строки
            }
        }
//        QString str_lineEdit_itogMdnf = "lineEdit_itogMdnf=" + lineEdit_itogMdnf->text();
        QString str_lineEdit_itogMdnf = "lineEdit_itogMdnf=" + editor->getFormulaText();

//        QTextStream ts(&fileOutput); // создаём поток вывода
//        ts << str_name << "\r\n";
//        ts << str_group << "\r\n";
//        ts << str_step << "\r\n";
//        ts << str_func2 << "\r\n";
//        ts << str_tot << "\r\n";
//        ts << str_ones_only_editing << "\r\n";
//        ts << str_tableWidgetsSkleykiEditing_0 << "\r\n";
//        ts << str_tableWidget_one_only << "\r\n";
//        ts << str_tableWidgetsSkleykiEditing_1 << "\r\n";
//        ts << str_tableWidgetsSkleyki_0 << "\r\n";
//        ts << str_tableWidgetsSkleykiEditing_2 << "\r\n";
//        ts << str_tableWidgetsSkleyki_1 << "\r\n";
//        ts << str_tableWidgetKartaMinimizacii << "\r\n";
//        ts << str_lineEdit_itogMdnf << "\r\n";

        QString outputtext;
        outputtext.append(str_name);  outputtext.append("\r\n");
        outputtext.append(str_group);  outputtext.append("\r\n");
        outputtext.append(str_step);  outputtext.append("\r\n");
        outputtext.append(str_func2);  outputtext.append("\r\n");
        outputtext.append(str_tot);  outputtext.append("\r\n");
        outputtext.append(str_ones_only_editing);  outputtext.append("\r\n");
        outputtext.append(str_tableWidgetsSkleykiEditing_0);  outputtext.append("\r\n");
        outputtext.append(str_tableWidget_one_only);  outputtext.append("\r\n");
        outputtext.append(str_tableWidgetsSkleykiEditing_1);  outputtext.append("\r\n");
        outputtext.append(str_tableWidgetsSkleyki_0);  outputtext.append("\r\n");
        outputtext.append(str_tableWidgetsSkleykiEditing_2);  outputtext.append("\r\n");
        outputtext.append(str_tableWidgetsSkleyki_1);  outputtext.append("\r\n");
        outputtext.append(str_tableWidgetKartaMinimizacii);  outputtext.append("\r\n");
        outputtext.append(str_lineEdit_itogMdnf);  outputtext.append("\r\n");

        md5crypter::cryptStr(outputtext); // добавляем хеш
        fileOutput.write(outputtext.toUtf8()); // записываем

        fileOutput.close();
    }
}

void MainWindow::readDataFromFile()
{
    QFile file(FILE_STUDENT_WORK); // открываем файл с сохраненной работой
    if(!file.open(QIODevice::ReadOnly)) // если не удалось открыть
    {
        qDebug() << "Не удалось открыть файл для чтения!";
        return;
    }
    QString text = file.readAll(); // считываем весь файл
    bool ok = false;
    md5crypter::decryptStr(text, ok);
    if(!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Файл решения повреждён!");
        return;
    }
    QStringList list = text.split(STR_SEPARATOR_1); // разбиваем по строчкам
    int size = list.size(); // получаем количество строк
    if(size < 2) // если пусто
    {
        qDebug() << "Файл пустой!";
        return;
    }
    int file_step = 0; // переменная для хранения последнего шага, нужна при сорных ситуациях, например, склеек 3 нет, или их не должно быть
    for (int i=0; i<size; i++) // проходим по всем строчкам файла
    {
        qDebug() << "Установленный этап: " << step;
        if(!list[i].contains("=")) // если строка содержит знак =, т.е. в ней что-то сохранено
        {
            qDebug() << "Строка " << list[i] << " не содержит =";
            continue;
        }
        QStringList dataList = list[i].split("=", Qt::SplitBehavior(Qt::SkipEmptyParts));
        int dataListSize = dataList.size(); // получаем размер списка, сформированного из строки
        if(dataListSize < 2)
        {
            qDebug() << "размер строки < 2. Номер:" << i << "Строка:" << list[i];
            if(step >= file_step)
            {
                qDebug() << "Текущие этап: " << step << " Сохраненный этап: " << file_step;
//                continue; // пропускаем итерацию цикла
                break;
            }
        }
        if(dataList[0] == "step") // если есть информация о строке со значением последнего шага
        {
            QString str_step = dataList[1]; // считываем данные из 2й части строки
            if(str_step == "")
            {
                qDebug() << "str_step пустая строка";
                return;
            }
            file_step = str_step.toInt(); // нам тут не важно, корректно или некорректно переведётся строка, т.к. эта информация используется только для конфликтных случаев
//            lineEditFunc_2->setText(str_step); // выводим текст в lineEdit, где он должен быть
//            pushButtonFunc2Clicked();  // эмитируем нажатие кнопки
        }
        if(dataList[0] == "lineEditFunc_2") // если есть информация о строке со значением функции в 2 ссч
        {
            if(dataListSize < 2)
            {
                qDebug() << "str_func2 пустая строка";
                if(step < file_step)
                {
                    nextStep(); // делаем пропуск этапа, если последний сохраненный этап был больше
                }
                continue;
            }
            else
            {
                QString str_func2 = dataList[1]; // считываем данные из 2й части строки
                lineEditFunc_2->setText(str_func2); // выводим текст в lineEdit, где он должен быть
                on_lineEdit_func_2_textChanged(lineEditFunc_2->text());
//                pushButtonFunc2Clicked();  // эмитируем нажатие кнопки
            }

        }
        if(dataList[0] == "tableWidgetOnesOnlyEditing")
        {
            qDebug() << "tableWidgetOnesOnlyEditing";
            if(dataListSize < 2)
            {
                qDebug() << "tableWidgetOnesOnlyEditing пустая строка";
                if(step < file_step)
                {
                    nextStep(); // делаем пропуск этапа, если последний сохраненный этап был больше
                }
                continue;
            }
            QString str_tableWidgetOnesOnlyEditing = dataList[1]; // считываем данные из 2й части строки
            QStringList tableList = str_tableWidgetOnesOnlyEditing.split("|"); // разделяем
            nextStep(); // если таблица истинности верная и мы на этапе F(a,b,c,d) = 1, значи тпропускаем этап проверки таблицы истинности
            setQStringListToTW(tableWidgetOnesOnlyEditing, tableList);
            center_text_in_table(tableWidgetOnesOnlyEditing); // выраниваем текст в таблице
        }
        if(dataList[0] == "tableWidget_one_only")
        {
            qDebug() << "dataList[0] == tableWidget_one_only";
            if(dataListSize < 2)
            {
                qDebug() << "tableWidget_one_only пустая строка";
                if(step < file_step)
                {
                    nextStep(); // делаем пропуск этапа, если последний сохраненный этап был больше
                }
                continue;
            }
            QString str_tableWidget_one_only = dataList[1]; // считываем данные из 2й части строки
            QStringList tableList = str_tableWidget_one_only.split("|"); // разделяем
//            nextStep(); // если таблица истинности верная и мы на этапе F(a,b,c,d) = 1, значи тпропускаем этап проверки таблицы истинности
            qDebug() << "Выводим значения таблицы one_only";
            pushButtonProverkaClicked(); // вызываем функцию проверки, чтобы программа установила тип минимизации
            setQStringListToTW(tableWidgetOnesOnly, tableList, true);
            center_text_in_table(tableWidgetOnesOnly); // выраниваем текст в таблице
//            if(step >= file_step && file_step > 0)
//            {
//                qDebug() << "Текущие этап: " << step << " Сохраненный этап: " << file_step;
//    //                continue; // пропускаем итерацию цикла
//    //            break;
//                setStep(5);
//                qDebug() << "Принудительно установлен этап 5";
//                return;
//            }
//            qDebug() << "функция прервана!!";
//            return;
        }
        if(dataList[0] == "str_tableWidgetsSkleyki_0")
        {
//            qDebug() << "dataList[0] == tableWidget_one_only";
            if(dataListSize < 2)
            {
                qDebug() << "str_tableWidgetsSkleyki_0 пустая строка";
                if(step < file_step)
                {
                    nextStep(); // делаем пропуск этапа, если последний сохраненный этап был больше
                }
                continue;
            }
            QString str = dataList[1]; // считываем данные из 2й части строки
            QStringList tableList = str.split("|"); // разделяем
//            nextStep(); // если таблица истинности верная и мы на этапе F(a,b,c,d) = 1, значи тпропускаем этап проверки таблицы истинности
            qDebug() << "Выводим значения таблицы one_only";
            setQStringListToTW(tableWidgetsSkleyki[0], tableList, LINE_COLOR_ON);
            center_text_in_table(tableWidgetsSkleyki[0]); // выраниваем текст в таблице
//            qDebug() << "функция прервана!!";
//            return;
        }
        if(dataList[0] == "str_tableWidgetsSkleyki_1")
        {
//            qDebug() << "dataList[0] == tableWidget_one_only";
            if(dataListSize < 2)
            {
                qDebug() << "str_tableWidgetsSkleyki_0 пустая строка";
                if(step < file_step)
                {
                    nextStep(); // делаем пропуск этапа, если последний сохраненный этап был больше
                }
                continue;
            }
            QString str = dataList[1]; // считываем данные из 2й части строки
            QStringList tableList = str.split("|"); // разделяем
//            nextStep(); // если таблица истинности верная и мы на этапе F(a,b,c,d) = 1, значи тпропускаем этап проверки таблицы истинности
            qDebug() << "Выводим значения таблицы one_only";
            setQStringListToTW(tableWidgetsSkleyki[1], tableList, LINE_COLOR_ON);
            center_text_in_table(tableWidgetsSkleyki[1]); // выраниваем текст в таблице
//            qDebug() << "функция прервана!!";
//            return;
        }
        if(dataList[0] == "str_tableWidgetsSkleykiEditing_0")
        {
            if(dataListSize < 2)
            {
                qDebug() << "str_tableWidgetsSkleykiEditing_0 пустая строка";
                if(step < file_step)
                {
                    nextStep(); // делаем пропуск этапа, если последний сохраненный этап был больше
                    pushButton_nextStep_totClicked(); // как бы нажимаем кнопку далее
                }
                continue;
            }
            QString str_tableWidgetsSkleykiEditing_0 = dataList[1]; // считываем данные из 2й части строки
            QStringList tableList = str_tableWidgetsSkleykiEditing_0.split("|"); // разделяем
//            nextStep(); // если мы дошли до первой таблицы склеек, значит пропускаем этап проверки таблицы F = 1
            pushButton_nextStep_totClicked(); // как бы нажимаем кнопку далее
            setQStringListToTW(tableWidgetsSkleykiEditing[0], tableList, LINE_COLOR_ON); // добавляем строки в таблицу
            center_text_in_table(tableWidgetsSkleykiEditing[0]); // выраниваем текст в таблице
        }
        if(dataList[0] == "str_tableWidgetsSkleykiEditing_1")
        {
            if(dataListSize < 2)
            {
                qDebug() << "str_tableWidgetsSkleykiEditing_1 пустая строка";
                qDebug() << "Текущие этап: " << step << " Сохраненный этап: " << file_step;
                if(step < file_step)
                {
                    nextStep(); // делаем пропуск этапа, если последний сохраненный этап был больше
                    pushButton_nextStep_tot_2Clicked(); // как бы нажимаем кнопку далее
                }
                continue;
            }
            QString str_tableWidgetsSkleykiEditing_1 = dataList[1]; // считываем данные из 2й части строки
            QStringList tableList = str_tableWidgetsSkleykiEditing_1.split("|"); // разделяем
            nextStep(); // если мы дошли до второй таблицы склеек, значит пропускаем этап проверки таблицы склеек 1
            pushButton_nextStep_tot_2Clicked(); // как бы нажимаем кнопку далее
            setQStringListToTW(tableWidgetsSkleykiEditing[1], tableList, LINE_COLOR_ON); // добавляем строки в таблицу
            center_text_in_table(tableWidgetsSkleykiEditing[1]); // выраниваем текст в таблице
        }
        if(dataList[0] == "str_tableWidgetsSkleykiEditing_2")
        {
            if(dataListSize < 2)
            {
                qDebug() << "str_tableWidgetsSkleykiEditing_2 пустая строка";
                qDebug() << "Текущие этап: " << step << " Сохраненный этап: " << file_step;
                if(step < file_step)
                {
                    nextStep(); // делаем пропуск этапа, если последний сохраненный этап был больше
                    pushButton_nextStep_skleyki_2Clicked(); // как бы нажимаем кнопку далее
                }
                continue;
            }
            QString str_tableWidgetsSkleykiEditing_2 = dataList[1]; // считываем данные из 2й части строки
            QStringList tableList = str_tableWidgetsSkleykiEditing_2.split("|"); // разделяем
            nextStep(); // если мы дошли до третьей таблицы склеек, значит пропускаем этап проверки таблицы склеек 2
            pushButton_nextStep_skleyki_2Clicked(); // как бы нажимаем кнопку далее
            setQStringListToTW(tableWidgetsSkleykiEditing[2], tableList, LINE_COLOR_ON); // добавляем строки в таблицу
            center_text_in_table(tableWidgetsSkleykiEditing[2]); // выраниваем текст в таблице
        }
        if(dataList[0] == "tableWidgetKartaMinimizacii")
        {
            qDebug() << "str_tableWidgetKartaMinimizacii";
            if(dataListSize < 2)
            {
                qDebug() << "str_tableWidgetKartaMinimizacii пустая строка";
                qDebug() << "Текущие этап: " << step << " Сохраненный этап: " << file_step;
                if(step < file_step)
                {
                    nextStep(); // делаем пропуск этапа, если последний сохраненный этап был больше
                    pusbButton_move_skleyki_to_coresTable(); // как бы нажимаем кнопку далее
                }
                continue;
            }
            qDebug() << "Строка: " << dataList[1];
            QString str_tableWidgetKartaMinimizacii = dataList[1]; // считываем данные из 2й части строки
            QStringList tableList = str_tableWidgetKartaMinimizacii.split("|"); // разделяем
            nextStep(); // если мы дошли до карты минимизации, значит пропускаем этап проверки таблицы склеек 3
            pusbButton_move_skleyki_to_coresTable(); // как бы нажимаем кнопку далее
            nextStep(); // переходим к выводу + в таблицу
//            tableWidgetKartaMinimizacii->setRowCount(tableList.size()); // устанавливаем количе
            int rows = tableWidgetKartaMinimizacii->rowCount(); // получаем количество строк, которые установлены
            int cols = tableWidgetKartaMinimizacii->columnCount(); // получаем количество столбцов, которые установлены
            qDebug() << "Количество строк: " << rows;
            if(rows > 0)
            {
                qDebug() << "Заполняем + таблицу";
                int size = tableList.size(); // получаем  количество элементов в списке строк из файла
                if(size != rows) // количество строк сохраненное в файл, должно совпадать с тем количеством строк, которые получено в результате перевода последней склейки
                {
                    qDebug() << "количество элементов в спике не совпадает с количеством строк " << tableList.size() << rows;
                    continue;
                }
//                if(tableList[0].count() != cols) // количество символов в строке должно совпадать с количеством столбцов
//                {
//                    qDebug() << "количество элементов в спике не совпадает с количеством столбцов " << tableList[0].count() << cols;
//                    continue;
//                }
                for (int i=0; i<size; i++) // проходим по всем строкам, которые должны будут быт ьвыведены в таблицу
                {
                    qDebug() << "rowList = " << tableList[i];
                    QStringList rowList = tableList[i].split("", Qt::SplitBehavior(Qt::SkipEmptyParts)); // разбиваем строку на элементы
                    qDebug() << "rowList = " << tableList[i] << " после разделения";
//                    int columns = rowList.size(); // количество элементов в строке равно количеству столбцов
                    int c = 0; // позиция символа в строке (т.к. ячейка состоит из 2х значений - тект и цвет
                    for (int j=0; j<cols; j++)
                    {
                        qDebug() << "Позиция" << j << ". Символ:" << rowList[j];
                        QString symvolInTable = ""; // создаем переменную, в которую положим то, что надо будет занести в таблицу
                        if(rowList[c].contains("+")) // если символ +
                        {
                            symvolInTable = "+";
                            qDebug() << "символ + на позиции" << j;
                        }
                        qDebug() << "Выводим символ в таблицу" << symvolInTable << "строка" << i << "столбец" << j;
                        tableWidgetKartaMinimizacii->setItem(i, j, new QTableWidgetItem(symvolInTable)); // заносим символ в таблицу
                        // определяем цвет
                        c++; // переходим к следующему символу
                        if(rowList[c].contains("w")) // если цвет - белый
                        {
                            tableWidgetKartaMinimizacii->item(i, j)->setBackground(QBrush(Qt::white));
                        }
                        else if(rowList[c].contains("g")) // если цвет - зелёный
                        {
                            tableWidgetKartaMinimizacii->item(i, j)->setBackground(QBrush(*greenColor));
                        }
                        else if(rowList[c].contains("r")) // если цвет - красный
                        {
                            tableWidgetKartaMinimizacii->item(i, j)->setBackground(QBrush(*redColor));
                        }
                        c++; // переходим к следующему символу
                    }
                }
            }
//            setQStringListToTW(tableWidgetKartaMinimizacii, tableList); // добавляем строки в таблицу
        }
        if(dataList[0] == "lineEdit_itogMdnf")
        {
            qDebug() << "Текущие этап: " << step << " Сохраненный этап: " << file_step << "lineEdit_itogMdnf";
            if(dataListSize < 2)
            {
                qDebug() << "str_lineEdit_itogMdnf пустая строка";
                if(step < file_step)
                {
                    nextStep(); // делаем пропуск этапа, если последний сохраненный этап был больше
                }
                continue;
            }
            QString str_lineEdit_itogMdnf = dataList[1]; // считываем данные из 2й части строки
            if(str_lineEdit_itogMdnf != "ERROR")
            {
                qDebug() << "Идём к следующему этапу";
//                nextStep(); // если мы дошли до ввода МДНФ, значит пропускаем этап проверки карты минимизации
    //            lineEdit_itogMdnf->setText(str_lineEdit_itogMdnf);
                itogMdnfTextLine = str_lineEdit_itogMdnf;
                qDebug() << "itogMdnfTextLine = " << itogMdnfTextLine;
            }
        }
//        if(step >= file_step && file_step > 0)
//        {
//            qDebug() << "Текущие этап: " << step << " Сохраненный этап: " << file_step;
////                continue; // пропускаем итерацию цикла
////            break;
//            return;
//        }
    }
    qDebug() << "Чтение из файла завершено!";
    if((step < file_step) && (step == 11))
    {
        nextStep();
    }
//    editor->setEditChoseItem(true);
}
