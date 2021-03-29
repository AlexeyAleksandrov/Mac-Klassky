#include "mainwindow.h"

void MainWindow::setQStringListToTW(QTableWidget *&tableWidget, QStringList list, bool containsColor)
{
    int rows = list.size(); // получаем размер списка
    if(!rows)
    {
        tableWidget->setRowCount(0);
        tableWidget->setRowCount(0);
        qDebug() << "Недостоточно строк для вывода QStringList в QTableWIdget";
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
        qDebug() << "Недостоточно столбцов для вывода QStringList в QTableWIdget";
        return;
    }
    tableWidget->setRowCount(rows);
    bool isContColor = false; // флаг для проверки, есть-ли в списке хоть одно значение, содержащее флаг цвета строки
    if(containsColor) // если должен быть цвет по параметру функции
    {
        for (int i=0; i<rows; i++)
        {
            if(list[i].contains("r") || list[i].contains("y")) // если строка содержит флаг цвета строки
            {
                isContColor = true;
            }
        }
    }

    if(containsColor && isContColor) // цвет должен содержаться и он содержится
    {
        tableWidget->setColumnCount(cols-1); // на 1 меньше, т.к. последний символ является флагом цвета
    }
    else
    {
        tableWidget->setColumnCount(cols);
    }
    for (int i=0; i<rows; i++)
    {
//        QString colorFlag = ""; // флаг цвета. На данный момент это r и y
        QColor color = Qt::white;
        if(containsColor) // если в списке содержатся строки привязанные к цвету
        {
            if(list[i].contains("r")) // если строка должна быть красной (этот элемент уже содержится)
            {
                list[i].remove("r");
                color = *redColor; // ставим красный цвет строки
                qDebug() << "ставим красный цвет строки";
            }
            else if(list[i].contains("y")) // если строка должна быть красной (этот элемент уже прошёл склейку)
            {
                list[i].remove("y");
                color = SKLEYKA_BACKGROUNF_COLOR; // ставим цвет склейки
                qDebug() << "ставим цвет склейки";
            }
        }
        qDebug() << "Выставляемый цвет: " << color << color.red() << color.green() << color.blue();
        QStringList element = list[i].split("", Qt::SplitBehavior(Qt::SkipEmptyParts)); // разбиваем элемент спика на элементы
        int elementSize = element.size(); // получаем длину строки этого элемента
        for (int j=0; j<elementSize; j++)
        {
            setTWText(tableWidget, i, j, element[j], color); // устаналиваем элемент
        }
    }
}

void MainWindow::addQStringToTWOneSymwolInItem(QTableWidget *&tableWidget, QString str)
{
    if(tableWidget == nullptr || str == "")
    {
        qDebug() << "Нельзя добавить строку в таблицу!" << str;
        return;
    }
    addRow(tableWidget, str.split("", Qt::SplitBehavior(Qt::SkipEmptyParts))); // разбиваем строку списка по символам и передаём функции как список строки, где 1 символ равен одному item
}

void MainWindow::addQStringListToTWOneSymwolInItem(QTableWidget *&tableWidget, QStringList list)
{
    if(tableWidget == nullptr || list.size() == 0)
    {
        qDebug() << "Нельзя добавить список в таблицу!";
        return;
    }
    int s = list.size(); // получаем размер списка
    for (int i=0; i<s; i++)
    {
        addQStringToTWOneSymwolInItem(tableWidget, list[i]); // Добавляем по строчкам в таблицу
    }
}


void MainWindow::clearTW(QTableWidget *&tableWidget)
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

bool MainWindow::getTWItemText(QTableWidget *&tableWidget, int row, int col, QString &outputString)
{
    // стартовые проверки входных параметров
    if(*&tableWidget == nullptr) // если указатель пустой
    {
        qDebug() << "Указаетль на tableWidget пустой";
        return false;
    }
    if(row < 0) // если номер искомой строки меньше 0
    {
        qDebug() << "Номер строки меньше нуля. Невозможно получить текст" << row;
        return false;
    }
    if(col < 0) // если номер искомой строки меньше 0
    {
        qDebug() << "Номер столбца меньше нуля. Невозможно получить текст" << col;
        return false;
    }
    int rows = tableWidget->rowCount(); // получаем количество строк
    int cols = tableWidget->columnCount(); // получаем количество столбцов
    if(rows == 0) // если нет строк
    {
        qDebug() << "В таблице нет строк";
        return false;
    }
    if(cols == 0) // если нет столбцов
    {
        qDebug() << "В таблице нет столбцов";
        return false;
    }
    if(row >= rows) // если строка, к которой обращаемся не существует
    {
        qDebug() << "Невозможно получить текст из данной строки" << row << rows;
        return false;
    }
    if(row >= rows) // если столбец, к которому обращаемся не существует
    {
        qDebug() << "Невозможно получить текст из данного столбца" << col << cols;
        return false;
    }
    // получаем значение
    if(tableWidget->item(row, col) == nullptr) // если под элемент не выделена память
    {
        qDebug() << "Под элемент " << row << col << " не выделена память";
        return false;
    }
    outputString = tableWidget->item(row, col)->text(); // полчаем текст из ячейки
    return true;
}

bool MainWindow::getTWTextList(QTableWidget *&tableWidget, QStringList &outputList, bool ignoreCellWidgetInLastColumn)
{
    // стартовые проверки входных параметров
    if(*&tableWidget == nullptr) // если указатель пустой
    {
        qDebug() << "Указаетль на tableWidget пустой";
        return false;
    }
    int rows = tableWidget->rowCount(); // получаем количество строк
    int cols = tableWidget->columnCount(); // получаем количество столбцов
    if(rows == 0) // если нет строк
    {
        qDebug() << "В таблице нет строк";
        return false;
    }
    if(cols == 0) // если нет столбцов
    {
        qDebug() << "В таблице нет столбцов";
        return false;
    }
    if(ignoreCellWidgetInLastColumn) // последний столбец отдан под виджеты
    {
        cols--;
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
                qDebug() << "[getTWTextList] Не удалдось получить текст из ячейки " << i << j;
                return false;
            }
            string.append(str); // добавляем элемент в общую строку
        }
        listRows.append(string); // добавляем строку в список
    }
    outputList = listRows; // передаем список назад
    return true;
}

void MainWindow::addRow(QTableWidget *&tableWidget, QStringList rowList, int colorRed, int colorGreen, int colorBlue)
{
    if(tableWidget == nullptr)
    {
        qDebug() << "addRow Таблицы не существует";
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
        tableWidget->item(rows-1, i)->setBackground(QColor(colorRed, colorGreen, colorBlue)); // устанавливаем цвет ячейки
    }
    center_text_in_table(tableWidget); // выравниваем ячейки
}

void MainWindow::createSkleyka(QTableWidget *tableWidgetInput, QCheckBox **checkBoxesInput, QTableWidget *&tableWidgetOutput, int numSkleyka, bool ignoreCellWidgetInLastColumn)
{
//    for (int i=0; i<16; i++) // функция для отладки
//    {
//        qDebug() << "Функция" << checkBoxesInput[i]->isChecked();
//    }
    bool ones[16]; // создаем массив, который будет хранить информацию, нажата-ли галочка
    int countChecked = 0; // переменная для подсчёта количества "галочек"
    QStringList listOnes; // список, хранящий все лементы в таблице, где функция принимает 1
    getTWTextList(tableWidgetInput, listOnes, ignoreCellWidgetInLastColumn); // получаем элементы из таблицы с 1
    int listOnesSize = listOnes.size(); // поулчаем количество элементов в списке с 1
    QStringList elements; // список для хранения элементов, где стоит галочка
    int last_num = 0; // переменная для хранения индекса строки, которая была последней, нужно для проверки строки, если выбрана только одна, не проходилали она уже склейку
    for (int i=0; i<listOnesSize; i++)
    {
        ones[i] = checkBoxesInput[i]->isChecked(); // сохраняем информацию, нажато-ли галочка
        countChecked += static_cast<int>(ones[i]); // сразу считаем количество нажатий
        if(ones[i])
        {
            elements.append(listOnes[i]);
            last_num = i; // сохраняем номер индекса как последний использованный
        }
    }
//    if(countChecked == 0) // если не выбрано ни одной галочки
//    {
//        criticalError("Выберите 2 элемента для произведения их склейки или 1, если склейку произвести нельзя!");
//        return;
//    }
    if(countChecked != 2) // если выбрано больше чем 2 галочки (больше или меньше), то пишем ошибку
    {
        qDebug() << "Нажато " << countChecked << "галочек";
        criticalError("Выберите 2 элемента для произведения склейки!");
        return;
    }
    if((countChecked == 1) && (tableWidgetInput->item(last_num, 0)->background() == SKLEYKA_BACKGROUNF_COLOR)) // если выбрана только одна строка и она отмечена желтым, т.е. уже прошла склейку
    {
        criticalError("Этот элемент уже прошёл склейку!");
        return;
    }
    int proshliSkleykuCount = 0; // переменная для подсчёта, сколько выбраннх элементов прошли склейку
    for (int i=0; i<listOnesSize; i++) // проходим по всем элементам
    {
        if((checkBoxesInput[i]->isChecked()) && (tableWidgetInput->item(i, 0)->background() == SKLEYKA_BACKGROUNF_COLOR)) // если элемент выбран и цвет строки желтый
        {
            proshliSkleykuCount++; // увеличиваем счётчик элементов, прошедших склейку
        }
    }
//    if(proshliSkleykuCount == 2) // если оба имеющихся элемента прошли склейку
//    {
//        qDebug() << "Оба выбранных элемента прошли склейку, создание склейки невозможно!";
//        warningError("Ошибка! Оба выбранных элемента прошли склейку, создание склейки невозможно!");
//        return;
//    }
    QStringList skleykaList; // получаем результат склейки
    if(countChecked == 1) // если элемент всего 1
    {
        skleykaList = elements; // то просто прираниваем результат склейки в этому элементу (точнее приравниваем списки)
        if(!isContainsSkleyki(elements, numSkleyka)) // если на этом этапе склейки хотя бы один элемент отсутствует
        {
            warningError("Ошибка! Данный элемент можно склеить!");  // обрабатываем склейку, т.е. должна ли она вообще быть
            return;
        }
    }
    else // иначе производим склейку
    {
        bool maked = false;
        mdnfMacKlassky *mmk = new mdnfMacKlassky;
        mmk->makeSkleyki(elements, skleykaList, maked); // производим склейку 2х элементов
        delete mmk;
        mmk = nullptr;
    }
    int skleykaListSize = skleykaList.size(); // получаем количество элементов в склейке
    if(skleykaListSize != 1) // если элементы не склеиваются
    {
        qDebug() << "pushButtonAddTSkeyki_1 Не удалось произвести склейку";
        criticalError("Невозможно произвести склейку выбранных элементов!");
        return;
    }
//    if(skleykaListSize > 1)
//    {
//        qDebug() << "pushButtonAddTSkeyki_1 Не возможно произвести склейку";
//        criticalError("Невозможно произвести склейку выбраннх элементов!");
//        return;
//    }
    QString skleyka = skleykaList[0]; // получаем склейку
    QStringList skleykiInTable; // лист для получения склеек из таблицы
    getTWTextList(tableWidgetOutput, skleykiInTable, ignoreCellWidgetInLastColumn); // получаем склейки из таблицы
    int intTableSize = skleykiInTable.size();
    bool adding = true; // флаг, разрешающий добавление склейки в таблицу (false если склейка уже имеется)
    if(intTableSize > 0) // если в таблице есть элементы
    {
        for (int i=0; i<intTableSize; i++)
        {
            if(skleykiInTable[i] == skleyka) // если хоть один элемент совпадает с нашей склейкой
            {
                adding = false; // запрещаем добавлять
            }
        }
    }
//    if(adding) // если добавлять можно
//    {
        if(adding) // если склейка еще не соджержится
        {
           addRow(tableWidgetOutput, skleyka.split("", Qt::SplitBehavior(Qt::SkipEmptyParts)));  // добавляем склейку
        }
        else // если элемент уже есть
        {
            if(proshliSkleykuCount == 2) // если оба имеющихся элемента прошли склейку
            {
                qDebug() << "Оба выбранных элемента прошли склейку, создание склейки невозможно!";
                warningError("Ошибка! Оба выбранных элемента прошли склейку, создание склейки невозможно!");
                return;
            }
            addRow(tableWidgetOutput, skleyka.split("", Qt::SplitBehavior(Qt::SkipEmptyParts)), redColor->red(), redColor->green(), redColor->blue());  // добавляем склейку, подсвечивая её красным цветом
        }
        int rows = tableWidgetInput->rowCount(); // получаем количество строк в исходной таблице
        for (int i=0; i<rows; i++) // проходим по всем строкам
        {
            if(ones[i]) // если стоит галочка
            {
                int cols = tableWidgetInput->columnCount(); // получаем количество столбцов исходной таблицы
                for (int j=0; j<cols; j++) // проходим по всем столбцам этой строки
                {
                    tableWidgetInput->item(i, j)->setBackground(SKLEYKA_BACKGROUNF_COLOR); // закрашиваем фон в желтый цвет у тех, кто прошёл склейку
                }
            }
        }
//    }
//    else
//    {
////        criticalError("Данная склейка уже существует!");
//        int rows = tableWidgetInput->rowCount(); // получаем количество строк в исходной таблице
//        for (int i=0; i<rows; i++) // проходим по всем строкам
//        {
//            if(ones[i]) // если стоит галочка
//            {
//                int cols = tableWidgetInput->columnCount(); // получаем количество столбцов исходной таблицы
//                for (int j=0; j<cols; j++) // проходим по всем столбцам этой строки
//                {
//                    tableWidgetInput->item(i, j)->setBackground(SKLEYKA_BACKGROUNF_COLOR); // закрашиваем фон в желтый цвет у тех, кто прошёл склейку
//                }
//            }
//        }
//    }
    setVariablesToHeader(tableWidgetOutput); // задаем заголовки
    center_text_in_table(tableWidgetOutput); // выраниваем текст в таблице
}

bool MainWindow::isContainsSkleyki(QStringList skleykiList, int numSkleyka)
{
    QStringList skleykaListByMNF; // список для хранения тех склеек, которые должны быть
    if(numSkleyka == 1) // если мы на этапе 1й склейки
    {
        skleykaListByMNF = mdnf->getSkleyki1(typeMin); // записываем список всех склеек 1го этапа
    }
    if(numSkleyka == 2) // если мы на этапе 2й склейки
    {
        skleykaListByMNF = mdnf->getSkleyki2(typeMin); // записываем список всех склеек 2го этапа
    }
    if(numSkleyka == 3) // если мы на этапе 3й склейки
    {
        skleykaListByMNF = mdnf->getSkleyki3(typeMin); // записываем список всех склеек 3го этапа
    }
    return isAllElementsFirstQStrlingListContainsInSecondQStringList(skleykiList, skleykaListByMNF); // возвращаем результат поиска склейкит в списке, который должен быть
//    bool ok = true; // флаг проверки наличия данной склейки в рассчитанном списке
//    for (int i=0; i<skleykiList.size(); i++)  // сравниваем каждый элемент склейки
//    {
//        bool contained = false;
//        for (int j=0; j<skleykaListByMNF.size(); j++)  // с каждым, который должкен быть
//        {
//           if(skleykiList.at(i) == skleykaListByMNF.at(j)) // если элементы равны
//           {
//               contained = true; // сохраняем, что этот элемент найден
//           }
//        }
//        if(!contained) // если элемент не содержится
//        {
//            ok = false; // то сохрняем, что есть ошибка
//            break; // дальше нет смысла проверять
//        }
//    }
//    return ok;
}

void MainWindow::moveSkleyka(QTableWidget *tableWidgetInput, QCheckBox **checkBoxesInput, QTableWidget *&tableWidgetOutput, int numSkleyka, bool ignoreCellWidgetInLastColumn)
{
    QStringList listOnes; // список, хранящий все лементы в таблице, где есть галочка
    getTWTextList(tableWidgetInput, listOnes, ignoreCellWidgetInLastColumn); // получаем элементы, где есть галочка
    int listOnesSize = listOnes.size(); // поулчаем количество элементов в списке с 1
    if(listOnesSize < 1) // если не выбрано ни одного элемента
    {
        qDebug() << "Не выбрано ни одного элемента для перемещения";
        return;
    }
    qDebug() << "Размер списка = " << listOnesSize;
    QStringList elements; // список для хранения элементов, где стоит галочка
    for (int i=0; i<listOnesSize; i++)
    {
        if(checkBoxesInput[i]->isChecked()) // если галочка нажата
        {
            elements.append(listOnes[i]); // добавляем элемент в список
        }
    }
    if(!isContainsSkleyki(elements, numSkleyka)) // если хотя бы один элемент не содержится в итоговой склейке
    {
        if(elements.count() == 1) // если переносится только один элемент и он не содержится, т.е. может быть скллен
        {
            warningError("Ошибка! Данный элемент можно склеить!");
            return;
        }
        else
        {
            qDebug() << "Один или более элементов не могут быть склеены";
            warningError("Ошибка! Один или более выбранных элементов могут быть склеены с другими!\nДобавление без склейки невозможно.");
            return;
        }
    }
    QStringList outputTableWidgetDataList;
    getTWTextList(tableWidgetOutput, outputTableWidgetDataList, ignoreCellWidgetInLastColumn); // считываем данные из таблицы, нужно для проверки, нет ли уже этой склейки в таблице
    bool contained = false; // флаг проверки наличия данной склейки в рассчитанном списке
    for (int i=0; i<elements.size(); i++)  // сравниваем каждый элемент первого списка
    {
        for (int j=0; j<outputTableWidgetDataList.size(); j++)  // с каждым элементом второго списка
        {
           if(elements.at(i) == outputTableWidgetDataList.at(j)) // если элементы равны
           {
               contained = true; // сохраняем, что есть совпадение и выходим
               break;
           }
        }
        if(contained) // выходим, т.к. есть совпадение
        {
            break;
        }
    }
    if(contained) // если хотя бы один элемент уже есть в таблице
    {
        qDebug() << "Один или более элементов уже находятся в таблице!";
        if(elements.size() == 1) // если выбран только один элемент
        {
           warningError("Ошибка! Этот элемент уже прошёл склейку!");
           return;
        }
        else
        {
            warningError("Ошибка! Один или более выбранных элементов уже прошли склейку!");
            return;
        }

    }
    qDebug() << "Результат проверки склеек " << numSkleyka << " - " << isContainsSkleyki(elements, numSkleyka);
    for (int i=0; i<listOnesSize; i++)
    {
//        qDebug() << i << checkBoxesInput[i]->isChecked();
        if(checkBoxesInput[i]->isChecked()) // если галочка нажата
        {
            qDebug() << "Добавляем строку " << listOnes[i];
            addQStringToTWOneSymwolInItem(tableWidgetOutput, listOnes[i]); // добавляем этот элемент
            for (int j=0; j<tableWidgetInput->columnCount(); j++)
            {
                tableWidgetInput->item(i, j)->setBackground(SKLEYKA_BACKGROUNF_COLOR); // закрашиваем фон в желтый цвет у тех, кто прошёл склейку
            }
        }
    }
    setVariablesToHeader(tableWidgetOutput); // задаем заголовки
}

bool MainWindow::proverkaTable(QTableWidget *tableWidgetInput, QStringList listOfSkeyki, bool ignoreRedColor, bool ignoreCellWidgetInLastColumn)
{
    QStringList userFuncList; // создаем список для хранения данных из tableWidget с функцией
    bool ok= false; // переменная для получения коорректности получения списка из TableWidget для записи функции (таблицы истинности)
    qDebug() << "ignoreCellWidgetInLastColumn = " << ignoreCellWidgetInLastColumn;
    ok = getTWTextList(tableWidgetInput, userFuncList, ignoreCellWidgetInLastColumn); // получаем список из таблицы истинности
    if(!ok)
    {
        qDebug() << "Не удалось получить список значений из таблицы истинности";
        return false;
    }
//    QStringList listFuncByMdnf = mdnf->getSkleyki1(); // получаем списолк для таблицы истинности
    QStringList listFuncByMdnf = listOfSkeyki; // получаем списолк для таблицы истинности
    int userSize = userFuncList.size(); // поулчаем количество строк в списке из таблицы истинности пользователя
    if(ignoreRedColor) // если нужно игнорировать строки с красным цветом
    {
        userSize = 0; // обнуляем счётчик
        for (int i=0; i<tableWidgetInput->rowCount(); i++) // проходим по всем строкам
        {
             if(tableWidgetInput->item(i, 0)->background().color() != *redColor)   // если цвет первого элемента красный
             {
                userSize++; // увеличиваем счётчик
             }
        }
    }
    int mdndSize = listFuncByMdnf.size(); // поулчаем количество строк в списке от МДНФ
    if(userSize != mdndSize)
    {
        qDebug() << "Количество строк в таблице истинности пользователя не совпадает с количеством строк в МДНФ " << userSize << mdndSize;
//        criticalError();
        for (int i=0; i<mdndSize; i++)
        {
            qDebug() << listFuncByMdnf[i];
        }
        return false;
    }
//    bool flag = true; // флаг ошибки
    bool flag = true; // флаг корректности
    for (int i=0; i<mdndSize; i++)
    {
        bool exist = false; // проверяет, есть ли эта склейка в списке
        for (int j=0; j<userFuncList.size(); j++)
        {
            if(listFuncByMdnf[i] == userFuncList[j]) // если элементы строки одинаковые
            {
                exist = true; // сохраняем, что элемент есть
            }
        }
        if(!exist)
        {
            qDebug() << "Элементы таблицы истинности пользователя не совпадают с МДНФ " /*<< userFuncList[i]*/ << listFuncByMdnf[i];
            flag = false;
//            bool extUser = false; // поиск, есть ли пользовательское значение, в списке
            for (int j=0; j<userFuncList.size(); j++)
            {
                qDebug() << "Значение введенное пользователем [" << j << "] = " << userFuncList[j];
            }
            for (int j=0; j<mdndSize; j++)
            {
                qDebug() << "Значение, которое должно быть [" << j << "] = " <<  listFuncByMdnf[j];
            }
        }
        else
        {
           qDebug() << "Элементы совпадают: " << userFuncList[i] << listFuncByMdnf[i];
        }
    }
    if(userSize != mdndSize)
    {
        qDebug() << "Количество строк в таблице истинности пользователя не совпадает с количеством строк в МДНФ " << userSize << mdndSize;
//        criticalError();
        return false;
    }
    if(!flag)
    {
        qDebug() << "Элементы таблицы истинности пользователя не совпадают с МДНФ ";
//        criticalError();
        return false;
    }
//    QMessageBox::information(this, "Правильно", "Правильно!"); // выводим, что все нормально
//    message(); // выводим, что все павильно
    return true;
}

void MainWindow::goToNextStep(QTableWidget *tableWidgetInput, QTableWidget *&tableWidgetOutput, int nextTabIndex, bool ignoreCellWidgetInLastColumn)
{
    qDebug() << "кнопка далее";
    if(tableWidgetInput == nullptr)
    {
        qDebug() << "tableWidgetOnesOnlyEditing == nullptr";
        return;
    }
    if(tableWidgetOutput == nullptr)
    {
        qDebug() << "tableWidgetOnesOnly == nullptr";
        return;
    }
    copyTableWidget(tableWidgetInput, tableWidgetOutput, true, ignoreCellWidgetInLastColumn); // переносим данные из одной таблицы в другую для продолжения
    tabWidget->setCurrentIndex(nextTabIndex); // переключаемся на следующий экран
    setVariablesToHeader(tableWidgetOutput); // устанавливаем заголовки как переменные
    center_text_in_table(tableWidgetOutput); // выраниваем текст в таблице
}

void MainWindow::copyTableWidget(QTableWidget *tableWidgetInput, QTableWidget *&tableWidgetOutput, bool skipIdenticalLines, bool ignoreCellWidgetInLastColumn)
{
    QStringList inputList; // список для получения значений из первой таблицы
    bool flag = getTWTextList(tableWidgetInput, inputList, ignoreCellWidgetInLastColumn); // получаем список строк
    if(!flag)
    {
        qDebug() << "copyTableWidget - ошибка!";
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
//    if(addCheckboxes && checkBoxesInput)
//    {
//        int cols = tableWidgetOutput->columnCount();
//        tableWidgetOutput->setColumnCount(cols+1); // добавляем столбец
//        for (int i=0; i<tableWidgetOutput->rowCount(); i++)
//        {
//            setCellCheckBox(tableWidgetOutput, &checkBoxesInput[i], i, cols); // передаём на установку в последний столбец checkBox
//        }
//    }
}

void MainWindow::deletelastRow(QTableWidget *&tableWidget)
{
    if(tableWidget == nullptr)
    {
        qDebug() << "указатель пуст";
        return;
    }
    int rows = tableWidget->rowCount();
    if(rows <= 0)
    {
        qDebug() << "нет строк для удаления";
        return;
    }
    tableWidget->setRowCount(rows-1);
    center_text_in_table(tableWidget); // выравниваем ячейки
}

void MainWindow::sortSkleiki(QTableWidget *skleikiTableWidget, bool ignoreCellWidgetInLastColumn)
{
    int rows = skleikiTableWidget->rowCount(); // получаем количество строк
    int cols = skleikiTableWidget->columnCount(); // получаем количество стобцов
    if((rows == 0) || (cols == 0))
    {
        qDebug() << "Таблица пустая. Сортировка невозможна!" << rows << cols;
        return;
    }
    QStringList skleikiList; // создаём лист для хранения склеек
    if(!getTWTextList(skleikiTableWidget, skleikiList, ignoreCellWidgetInLastColumn))
    {
        qDebug() << "Не удалось получить данные из таблицы";
        return;
    }
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
                        qDebug() << "Склейка" << skleikiList[i] << "не подходит - у неё не 2 Х";
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
//        QStringList outputList; // создаем список, в который будем выводить отсортиованный массив
        for (int i=0; i<size; i++)
        {
//            QString str = ""; // собираем строку по символам
            for (int j=0; j<skleikaSort[i].text.size(); j++)
            {
//                str.append(skleikaSort[i].text[j]);
                setTWText(skleikiTableWidget, i, j, skleikaSort[i].text[j]);
            }
        }

//        else if((skleikaSort[i].text[0] == "X") && (skleikaSort[i].text[1] == "X")) // XX00
//        {
//            skleikaSort[i].type = 1; // задем тип 1
//            continue;
//        }
//        else if((skleikaSort[i].text[0] == "X") && (skleikaSort[i].text[2] == "X") && (skleikaSort[i].size > 2)) // X0X0
//        {
//            skleikaSort[i].type = 2; // задем тип 1
//            continue;
//        }
//        else if((skleikaSort[i].text[0] == "X") && (skleikaSort[i].text[3] == "X") && (skleikaSort[i].size > 3)) // X00X
//        {
//            skleikaSort[i].type = 2; // задем тип 1
//            continue;
//        }
//        // Х на
    }
    delete [] skleikaSort;
    skleikaSort = nullptr;
}

void MainWindow::sortOnesCount(QTableWidget *tableWidgetInput, bool ignoreCellWidgetInLastColumn)
{
    int rows = tableWidgetInput->rowCount(); // получаем количество строк
    int cols = tableWidgetInput->columnCount(); // получаем количество стобцов
    if((rows == 0) || (cols == 0))
    {
        qDebug() << "Таблица пустая. Сортировка по количеству единиц невозможна!" << rows << cols;
        return;
    }
    QStringList skleikiList; // создаём лист для хранения склеек
    if(!getTWTextList(tableWidgetInput, skleikiList, ignoreCellWidgetInLastColumn))
    {
        qDebug() << "Не удалось получить данные из таблицы.";
        return;
    }
    int size = skleikiList.size(); // получаем размер списка
    if(size < 1)
    {
        qDebug() << "Нет склеек для сортировки по количеству единиц";
        return;
    }
    QStringList sortList; // список, в котором будут отсортированы все значения
    QStringList verticalHeaders; // список для хранения вертикальных хаголовков. В него будут заноситься данне о количестве единиц в числе
    for (int i=0; i<=cols; i++) // максимальное количество единиц равно количеству столбцов
    {
        for (int j=0; j<size; j++) // проходим по каждому значению
        {
             QString znachenie = skleikiList.at(j); // получаем значение
             int oneCount = 0; // переменная, которая будет считать, сколько единиц в этом значении
             for (int g=0; g<znachenie.count(); g++) // проходим по всем позициям значения
             {
                if(znachenie.at(g) == "1") // если число на позиции равно 1
                {
                    oneCount++; // прибавляем 1 к количеству
                }
             }
             if(oneCount == i) // если количество единиц в данном числе соответствует тому, сколько необходимо
             {
                 sortList.append(znachenie); // добавляем значение в список сортировки
                 verticalHeaders.append(QString::number(i)); // добавляем в список заголовков количество единиц
             }
        }
    }
    clearTW(tableWidgetInput); // очищаем таблицу
    addQStringListToTWOneSymwolInItem(tableWidgetInput, sortList); // выводим значения в tableWidget
    setVariablesToHeader(tableWidgetInput); // устанавливаем заголовки
    tableWidgetInput->setVerticalHeaderLabels(verticalHeaders); // задаём заголовки
}


QString MainWindow::getQStringByTableWidget(QTableWidget *tableWidget, bool saveLineColor, bool ignoreCellWidgetInLastColumn)
{
    QStringList list; // создаём список, в который запишем строки из таблицы
    getTWTextList(tableWidget, list, ignoreCellWidgetInLastColumn); // получаем список из строк из таблицы
    if(list.size() == 0) // если в списке ничего нет
    {
        return ""; // вызврпащаем пустоту
    }
    QString str; // строка, в которую запишем весь список
    for (int i=0; i<list.size(); i++) // проходим по всем элементам
    {
        str.append(list[i]); // добавляем каждую строку
        if(saveLineColor) // если включено сохранение цвета строк
        {
//            qDebug() << "Включено сохранение цвета";
//            qDebug() << "Цвет: " << tableWidget->item(i, 0)->background().color().red() << tableWidget->item(i, 0)->background().color().green() << tableWidget->item(i, 0)->background().color().blue();
            if(tableWidget->item(i, 0)->background().color() == SKLEYKA_BACKGROUNF_COLOR) // если цвет первого элемента строки совпадает с цветом для склеек
            {
                str.append("y"); // добавляем символ y, сокращение от yellow
            }
            else if(tableWidget->item(i, 0)->background().color() == *redColor) // если цвет первого элемента строки совпадает с цветом для повторящихся склеек
            {
                str.append("r"); // добавляем символ r, сокращение от red
            }
        }
        if(i != list.size()-1) // до -1, т.к. к полсденему элементу не надо добавлять знак разделения
        {
            str.append("|"); // добавляем разделитель
        }

    }
//    str.append(list[list.size()-1]); // добавляем последнюю строку
    return str;
}

void MainWindow::lockFormulaEditor(bool lock)
{
//    pushButton_INV->setEnabled(!lock);
    pushButton_A->setEnabled(!lock);
    pushButton_B->setEnabled(!lock);
    pushButton_C->setEnabled(!lock);
    pushButton_D->setEnabled(!lock);
    pushButton_AND->setEnabled(!lock);
    pushButton__OR->setEnabled(!lock);
    pushButton_SKOBKA_BEFORE->setEnabled(!lock);
    pushButton_SKOBKA_AFTER->setEnabled(!lock);
    pushButton_DEL->setEnabled(!lock);
    pushButton_CLEAR->setEnabled(!lock);
    pushButton_notA->setEnabled(!lock);
    pushButton_notB->setEnabled(!lock);
    pushButton_notC->setEnabled(!lock);
    pushButton_notD->setEnabled(!lock);
    checkBox_autoInputFormula->setEnabled(!lock);
    checkBox_editChoseItemFormula->setEnabled(!lock);

}

void MainWindow::setTWText(QTableWidget *&tableWidget, int row, int col, QString text, QColor itemColor)
{
    int rows = tableWidget->rowCount();
    int cols = tableWidget->columnCount();
    qDebug() << "Количество строк (Ф) = " << rows;
    qDebug() << "Количество столбцов (Ф) = " << cols;

    if((rows <= 0) || (cols <= 0))
    {
        qDebug() << "В таблице недолстаточно строк или столбцов";
        return;
    }
    if(row >= rows)
    {
        qDebug() << "Номер строки больше, чем количество столбцов";
        return;
    }
    if(col >= cols)
    {
        qDebug() << "Номер столбца больше, чем количество столбцов";
        return;
    }
    qDebug() << "Выставляем значение" << row << col << text;
//    if(tableWidget->item(row, col) == nullptr) // если нет элекмента, создаем его
//    {
//        tableWidget->setItem(row, col, new QTableWidgetItem); // выделяем память
//        tableWidget->item(row, col)->setBackground(itemColor); // устанавливаем цвет
//    }
    tableWidget->setItem(row, col, new QTableWidgetItem(text)); // записывем значение
    tableWidget->item(row, col)->setBackground(QBrush(itemColor)); // устанавливаем цвет
    qDebug() << "цвет установлен " << itemColor << itemColor.red() << itemColor.green() << itemColor.blue();
    if(tableWidget == tableWidgetOnesOnlyEditing)
    {
        setVariablesToHeader(tableWidgetOnesOnlyEditing); // устанавливаем переменные в загловки
    }
    center_text_in_table(tableWidget); // выравниваем ячейки
    if((tableWidget == tableWidgetOnesOnly) || (tableWidget == tableWidgetOnesOnlyEditing) || (tableWidget == tableWidgetsSkleyki[0]) || (tableWidget == tableWidgetsSkleykiEditing[0]) || (tableWidget == tableWidgetsSkleyki[1]) || (tableWidget == tableWidgetsSkleykiEditing[1]) || (tableWidget == tableWidgetsSkleyki[2]) || (tableWidget == tableWidgetsSkleykiEditing[2]))
    {
        setVariablesToHeader(tableWidget); // задаем заголовки
    }
}

