#include "mainwindow.h"

bool MainWindow::to2ssch(QString number16, QString &number2)
{
    int razryad = number16.count(); // получаем количество разрядов числа
    if(razryad == 0) // если пустая строка
    {
        return false;
    }
    bool ok; // флаг перевода
    int ch10 = number16.toInt(&ok, 16); // переводим число из 16й в 10ю ССЧ
    if(!ok) // если число не переводится выдаем ошибку
    {
        return false; // не переводится
    }
    QString ch2 = QString::number(ch10, 2); // переводим число из 10й в 2ю ССЧ
    int count = ch2.count(); // определяем количество знаков
    int razryad2;
//    razryad2 = static_cast<int>(pow(2, razryad)); // получаем количество разрядов, которое должно быть в 2й записи числа
    razryad2 = 4 * razryad; // получаем количество разрядов, которое должно быть в 2й записи числа
    if(count < razryad2) // если количество разрядов после перевода меньше (int отбрасывает незначащие нули), то добавляем нули вперед до нужного количества
    {
        int k = razryad2 - count; // считаем количество нулей, котрое надо добавить
        for (int i=0; i<k; i++)
        {
            ch2 = "0" + ch2; // добавляем нули
        }
    }
    number2 = ch2; // возвращаем переведенное число
    return true; // перевод успешный
}

bool MainWindow::to16ssch(QString number2, QString &number16)
{
    if(number2 == "")
    {
        return false;
    }
    bool ok = false;
    int num10 = number2.toInt(&ok, 2); // переводим в 10 ССЧ
    if(!ok)
    {
        return false;
    }
    number16 = QString::number(num10, 16);
    return true;
}
