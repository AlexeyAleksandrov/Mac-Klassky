#include "mainwindow.h"

void MainWindow::setStep(int step)
{
    QString stepName[12]; // массив, хранящий название этапа
    // этап 1 (перевод в 2 ССЧ)
    stepName[0] = "перевод в 2 ССЧ";
    if(step == 1)
    {
        message("Ваша функция: " + lineEditFunction->text() + ". \nПереведите её в 2 систему счисления и введите результат в поле F_2.\nЧисло в 2 системе счисления вы можете ввести через пробелы, например, 0011 0101 1101 1100, либо без пробелов, например, 0011010111011100.");
    }
//    pushButton_func_2->setEnabled(step == 1 || step == 2);
//    lineEditFunc_2->setReadOnly(step >= 0 && step <= 2);
    // этап 2 (создание таблицы истинности)
    stepName[1] = "создание таблицы истинности";
    pushButton_checkTableOfTrue->setEnabled(step == 2);
    // этап 3 (выбор и првоерка значений функции, при которых она равна 1)
    stepName[2] = "выбор и проверка значений функции, при которых она равна 1";
    pushButton_tot_add->setEnabled(step == 3);
    pushButton_proverka_oneOnly->setEnabled(step == 3);
    for (int i=0; i<16; i++)
    {
        if(checkBoxes[i] != nullptr)
        {
             checkBoxes[i]->setEnabled(step == 3); // выключаем сначала все чекбоксы
//            checkBoxes[i]->setVisible(step == 3); // выключаем сначала все чекбоксы
        }
        else
            qDebug() << "Элемент = NULL" << i;
    }
    // этап 4 (переход к созданию 1й склейки)
    stepName[3] = "переход к созданию 1й склейки";
    pushButton_nextStep_tot->setEnabled(step == 4);
    // этап 5 (создание 1й склейки)
    stepName[4] = "создание 1й склейки";
    pushButton_tot_add_to_skeyki_1->setEnabled(step == 5);
    pushButton_clear_skleiki_1_editing->setEnabled(step == 5);
    pushButton_proverka_skleiki_1_editing->setEnabled(step == 5);
    pushButton_tot_move_to_skeyki_1->setEnabled(step == 5);
    if(step == 5)
    {
        message("На этом этапе Вам необходимо провести склейки элементов.\nДля выполнения склейки установите 2 галочки напротив тех элементов, которые необходимо \"склеить\" и нажмите кнопку Добавить.\nЕсли склейка удалась, оба элемента будут выделены цветом. Для завершения этапа необходимо получить максимальное количество склеек, задействовав все исходные значения.\nЕсли какой-либо элемент невозможно \"склеить\" с другими, то установите одну галочку напротив этого элемента и нажмите кнопку Добавить.\nЕсли склейки произвести невозможно, оставьте таблицу со склейками пустой и нажмите Проверить.");
        int rows = tableWidgetOnesOnly->rowCount(); // получаем количество втрок в новой таблице
        for (int i=0; i<rows; i++)
        {
            checkBoxes_ones[i]->setVisible(true); // включаем отображение
        }
    }
    else
    {
        for (int i=0; i<16; i++)
        {
            checkBoxes_ones[i]->setVisible(false); // выключаем отображение
        }
    }
    // этап 6 (переход к 2й склейке)
    stepName[5] = "переход к 2й склейке";
    pushButton_nextStep_onesOnly->setEnabled(step == 6);
    // этап 7 (создание 2й склейки)
    stepName[6] = "создание 2й склейки";
    pushButton_add_skleyki_1->setEnabled(step == 7);
    pushButton_clear_skleiki_2_editing->setEnabled(step == 7);
    pushButton_proverka_skleyki_3->setEnabled(step == 7);
    pushButton_tot_move_to_skeyki_2->setEnabled(step == 7);
    if(step == 7)
    {
        int rows = tableWidgetsSkleyki[0]->rowCount(); // получаем колитечтво строк
        for (int i=0; i<rows; i++)
        {
            checkBoxes_skleyki_1[i]->setVisible(true); // включаем отображение
        }
    }
    else
    {
        for (int i=0; i<16; i++)
        {
            checkBoxes_skleyki_1[i]->setVisible(false); // выключаем отображение
        }
    }

    // этап 8 (переход к 3й склейке)
    stepName[7] = "переход к 3й склейке";
    pushButton_nextStep_skleyki_2->setEnabled(step == 8);
    // этап 9 (создание 3й склейки)
    stepName[8] = "создание 3й склейки";
    pushButton_add_skleyki_2->setEnabled(step == 9);
    pushButton_clearSkleyki_2->setEnabled(step == 9);
    pushButton_proverka_skleyki_2->setEnabled(step == 9);
    pushButton_tot_move_to_skeyki_3->setEnabled(step == 9);
    if(step == 9)
    {
        int rows_2 = tableWidgetsSkleyki[1]->rowCount(); // получаем колитечтво строк
        for (int i = 0; i < rows_2; ++i)
        {
           checkBoxes_skleyki_2[i]->setVisible(true); // делаем видимыми
        }
    }
    else
    {
        for (int i=0; i<16; i++)
        {
            checkBoxes_skleyki_2[i]->setVisible(false);
        }
    }
    // этап 10 (переход к карте минимизации)
    stepName[9] = "переход к карте минимизации";
    pushButton_nextStep_onesOnly_3->setEnabled(step == 10);
    // этап 11 (проверка карты минимизации)
    stepName[10] = "проверка карты минимизации";
    pushButton_proverka_karti_minimizacii->setEnabled(step == 11);
    if(step == 11)
    {
        message("На этом этапе Вам необходимо сопоставить слейки из вертикального столбца со значениями функции из горизонтальбной строки. В тех ячейках, где склейка совпадает со значением необходимо поставить +.");
    }
    // этап 12 (проверка итоговой МДНФ)
    stepName[11] = "проверка итоговой МДНФ";
    pushButton_proverka_karta_minimizacii->setEnabled(step == 12);
    pushButton_setRedColor->setEnabled(step == 12);
    pushButton_setGreenColor->setEnabled(step == 12);
    pushButton_setWhiteColor->setEnabled(step == 12);
    lockFormulaEditor(step != 12);
    if(step == 12)
    {
        QString mnfText = ""; // текст в подсказке
        if(typeMin == 0) // если тип МКНФ
        {
            mnfText = "МКНФ";
        }
        else
        {
            mnfText = "МДНФ";
        }
        message("Теперь на основании карты покрытия, составьте " + mnfText + " с помощью редактора формул.\nДля добавления инверсии над переменной выделите мышкой поле над этой переменной и нажмите кнопку инверсии.");
    }
    int n = step-1; // номер в массиве текущего этапа
    if(n < 0)
        qDebug() << "Установлен этап: " << step;
    else
        qDebug() << "Установлен этап: " << step << stepName[n];
}

void MainWindow::nextStep()
{
    step++; // увеличиваем шаг на 1
    qDebug() << "переходим к этапу: " << step;
    setStep(step);// переходим к следующему шагу
}
