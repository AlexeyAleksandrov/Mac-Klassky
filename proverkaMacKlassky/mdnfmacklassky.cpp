#include "mdnfmacklassky.h"
#include "qdebug.h"
#include "math.h"


mdnfMacKlassky::mdnfMacKlassky(QObject *parent) : QObject(parent)
{
    //    mdnfList = new QStringList;

    // инициализируем МДНФ и МКНФ
    mdnf = new mnf;
    mdnf->setType(1);
    mknf = new mnf;
    mknf->setType(0);
}

bool mdnfMacKlassky::setFunction(QString function_16)
{
    if(function_16 == "") // проверяем на пустоту
    {
        //СКРЫТО qDebug() << "Полученный вектор пустой!";
        error = "Полученный вектор пустой!";
        return false;
    }
    // переводим число в 2 ССЧ
    QString ch2;
    if(!to2ssch(function_16, ch2)) // проверяем перевод в 2ю ССЧ
    {
        //СКРЫТО qDebug() << "Вектор не является 16-м числом!";
        error = "Вектор не является 16-м числом!";
        return false;
    }

    int zeroCount = 0; // счётчик нулей
    int oneCount = 0; // счётчик единиц
    for (int i=0; i<ch2.count(); i++)
    {
        if(ch2.at(i) == "0")
        {
            zeroCount++;
        }
        if(ch2.at(i) == "1")
        {
            oneCount++;
        }
    }
//    int f_16Count = ch2.count();
//    int vCount = 0;
//    if(zeroCount > oneCount)
//    {
//        vCount = zeroCount;
//    }
//    else
//    {
//        vCount = oneCount;
//    }
//    //СКРЫТО qDebug() << "vCount = " << vCount << "f_16Count = " << f_16Count;
//    double procent = (static_cast<double>(vCount) / static_cast<double>(f_16Count));
//    //СКРЫТО qDebug() << "Определён тип: " << vType << "Число 0: " << zeroCount << "Число 1: " << oneCount;
//    //СКРЫТО qDebug() << "Процент: " << procent << "Функция: " << function_16;
//    //    if((procent >= 0.75) || ((procent == 0.50) && (MKNF_OR_MDNF_ONLY)))
//    if(procent >= 0.90) // если больше 90% составляет 0 или 1, т.е. функция, в которой 2 нуля или 2 единицы ,а всё остальное противоположное значение
//    {
//        //СКРЫТО qDebug() << "Расчёт недопустим!"  << "Процент: " << procent << "Функция: " << function_16;
//        return false;
//    }
    if(zeroCount == 0 || oneCount == 0) // если функция состояит полностью из 0 или из 1
    {
        return false;;
    }
    vector_16 = function_16;
    // обнуляем все переменные
    clearData();
    mdnf->clear();
    mknf->clear();
    calculateTot(vector_16); // считаем таблицу истинности
    if(allowTot) // если таблица истинности рассчитывается
    {
        mdnf->allowCalculate = calculate(mdnf->typeNf, mdnf->listFunc, mdnf->skleyka1, mdnf->skleyka2, mdnf->skleyka3, mdnf->minList, mdnf->varsCount, mdnf->skleykiCount, mdnf->mintermsCount, mdnf->coreMfFormula);
        mknf->allowCalculate = calculate(mknf->typeNf, mknf->listFunc, mknf->skleyka1, mknf->skleyka2, mknf->skleyka3, mknf->minList, mknf->varsCount, mknf->skleykiCount, mknf->mintermsCount, mknf->coreMfFormula);
    }
    return true;
}

bool mdnfMacKlassky::calculate(int type, QStringList &listFunc, QStringList &skleyka1, QStringList &skleyka2, QStringList &skleyka3, QStringList &minList, int &varsCount, int &skleykiCount, int &mintermsCount, QString &coreFormula)
{
    if(!allowTot) // если таблица истинности не рассчитана
    {
        error = "Вызвана функция рассчёта минимизации без рассчёта таблицы истинности!";
        return false;
    }

    //    int optimizeType = getOptimizeType16(vector_16);
    //    if((optimizeType != 2) && (type != optimizeType))
    //    {
    //        return false;
    //    }
    // первичные проверки
    //СКРЫТО qDebug() << "закончено";

    // =============================
    // == РАЗВИЛКА ДЛЯ МДНФ/МКНФ ===
    // =============================

    // создаём список значений ,подходящих под условие
    //    QStringList listFunc; // все числа, которые в функции дают 1
    for (int i=0; i<rowsTot; i++)
    {
        //СКРЫТО qDebug() << funcList[i]; // значение функции i
        if(funcList[i].toInt() == type) // если значение функции равно искомому (0 или 1, в зависимости от МКНФ/МДНФ)
        {
            QString znach = sschTableList[i]; // получаем значяение из таблицы истинности
            int size = znach.count(); // получаем количество символов
            QString str = ""; // создаем новую переменную для получения значения без последнего символа (значения функции)
            for(int i=0; i<size-1; i++) // считаем до количества -1, чтобы не попал последний символ
            {
                str.append(znach.at(i)); // добавляем по символьно
            }
            listFunc.append(str); // добавляем число в список
            //СКРЫТО qDebug() << "sschTableList[" << i << "] = " << sschTableList[i];
        }
    }
    // формируем таблицу истинности
    //    int sschTableListSize = sschTableList.size();
    //    for (int i=0; i<sschTableListSize; i++)
    //    {
    //        sschTableList[i] = sschTableList[i] +  funcList[i]; // добавляем значение функции к элементу таблицы истинности
    //    }
    // ??-- заносим значения функции, где она равна (0 или 1) в таблицу
    sizeFunc = listFunc.size(); // получаем количество состояний, где функция даёт 0 или 1
    // производим склейки
    //    skleykiCount = 0;
    QStringList skleyki[3]; // храним рельтат склеек
    bool wasSkleyka = false;
    int skleykiCounter = 0;
    makeSkleyki(listFunc, skleyki[0], wasSkleyka); // производим склейки
    skleykiCounter += static_cast<int>(wasSkleyka);
    wasSkleyka = false;
    makeSkleyki(skleyki[0], skleyki[1], wasSkleyka);
    skleykiCounter += static_cast<int>(wasSkleyka);
    wasSkleyka = false;
    makeSkleyki(skleyki[1], skleyki[2], wasSkleyka);
    skleykiCounter += static_cast<int>(wasSkleyka);
//    wasSkleyka = false;

    if(skleykiCounter <= 0)
    {
        //СКРЫТО qDebug() << "Нет склеек. Все склейки имеют нулевой размер";
        qDebug() << "Нет склеек. Все склейки имеют нулевой размер";
        error = "Нет склеек. Все склейки имеют нулевой размер";
//        return false;
    }

    skleykiCount = skleykiCounter; // возвращаем количество склеек
    skleyka1 = skleyki[0]; // передаём списки со склейками по адресу
    skleyka2 = skleyki[1];
    skleyka3 = skleyki[2];
//    int skleykiLastNum = skleykiCounter-1; // номер
    QStringList vertikalList = skleyki[2]; // создаем список вертикальных элементов (результатов склейки)
    QStringList horizontalList = listFunc; // создаем список горизонтальных элементов (те значения, где функция равна 1)
    int vertikalSize = vertikalList.size(); // вертикальный размер будет равен количеству склеек
    int horizontalSize = horizontalList.size(); // горизонтальный размер равен колитчеству элементов в функции, равных 1

    // создаем массив ядер функции, точнее массив таблицы, в которой будем искать ядра
    //СКРЫТО qDebug() << "начинаем выделять память под ядра";
    if(horizontalSize <= 0)
    {
        //СКРЫТО qDebug() << "размер горизонтального массива меньше 0";
        qDebug() << "размер горизонтального массива меньше 0";
        return false;
    }
    if(vertikalSize <= 0)
    {
        //СКРЫТО qDebug() << "размер вертикального массива меньше 0";
        qDebug() << "размер вертикального массива меньше 0";
        return false;
    }
    //СКРЫТО qDebug() << "создал массив";
    bool **cores; // массив для хранения + в карте покрытия (true - есть +, false - нет плюсика)
    cores = new bool *[horizontalSize]; // выделяем память под столбцы
    //СКРЫТО qDebug() << "выделил память под первый массив";
    for (int i=0; i<horizontalSize; i++)
    {
        cores[i] = new bool [vertikalSize]; // выделяем память под строки в каждом столбце
        //СКРЫТО qDebug() << "Выделил память под " << i << " элемент массива";
    }
    // просчитываем таблицу
    //СКРЫТО qDebug() << "mdnf начинаем считать таблицу";
    for (int i=0; i<horizontalSize; i++)  // проходим по каждому столбцу
    {
        QStringList horizontalString = horizontalList[i].split("", Qt::SplitBehavior(Qt::SkipEmptyParts)); // разбиваем горизонтальный элемент (заголовк столбца) на символы
        int horizontalStringSize = horizontalString.size(); // получаем размер списка
        for (int j=0; j<vertikalSize; j++) // проходим по каждой строке каждого столбца
        {
            QStringList verticalString = vertikalList[j].split("", Qt::SplitBehavior(Qt::SkipEmptyParts)); // разбиваем вертикальный элемент (заголовок строки) на символы
            int verticalStringSize = verticalString.size(); // получаем размер списка
            if(horizontalStringSize != verticalStringSize) // на всякий случай проверяем, чтобы размеры были одинаковы
            {
                //СКРЫТО qDebug() << "Размер склейки не совпадает с исходным размером";
                for(int r=0; r<horizontalSize; r++)
                {
                    delete [] cores[r];
                }
                delete [] cores;
                return false;
            }
            bool ok = true; // создаем переменную, отвечающую за совместимость склейки со столбцом
            for (int k=0; k<horizontalStringSize; k++)
            {
                ok = (ok && ((verticalString[k] == "X") || (horizontalString[k] == verticalString[k]))); // если на месте данной переменной стоит Х или оба числа одинаковые, то ставим true
            }
            cores[i][j] = ok; // если склейка подходит под этот столбец, то в массив заносится это значение
        }
    }
    //СКРЫТО qDebug() << "horizontalSize = " << horizontalSize;
    bool *perecritie = new bool [horizontalSize]; // создаем массив, в котором будем хранить, сколько столбцов перекрыты
    for (int i=0; i<horizontalSize; i++) // обнуляем массив
    {
        perecritie[i] = false;
    }

    //    QStringList disunctList; // список, хранящий неядровые дизъюнкты, которые образуют МДНФ по формуле ДНФ(ядр) + К(i), где К(i) - это элементы списка
    //    QList <QStringList> disunctList; // список, хранящий неядровые дизъюнкты, которые образуют МДНФ по формуле ДНФ(ядр) + К(i), где К(i) - это элементы списка
    QStringList coresList; // список, где будет храниться ядровая ДНФ
    for (int i=0; i<horizontalSize; i++)
    {
        int plusCount = 0; // считаем количество + в столбце
        int n = -1; // номер последнего элемента, где есть +
        for (int j=0; j<vertikalSize; j++)
        {
            if(cores[i][j]) // если содержится +
            {
                plusCount++; // то добавляем
                n = j; // сохраняем номер этого элемента как последнего
            }
        }
        if((plusCount == 1) && (n >= 0)) // если в столбце только один +, так же на всякий случай проверяем n
        {
            // если в столбце только один +, то его номер будет последним и единсвенным
            perecritie[i] = true; // сохраняем, что этот столбец закрыт
            coresList.append(vertikalList[n]); // заносим ядро, т.е. склейку в список
            //            ui->tableWidget_kartaMinimizacii->item(n, i)->setBackgroundColor(Qt::green); // устанавливаем зеленый цвет ядру
            for (int k=0; k<horizontalSize; k++) // проходим по всем столбцам
            {
                perecritie[k] = (perecritie[k] || (cores[k][n])); // если есть + в столбце 0..max и строке, равной строке ядра, или если в этом столбце уже есть пересечение (==1), то ничего не изменится
            }
        }
    }
    int perecritieCount = 0; // количество перекрытий
    for (int i=0; i<horizontalSize; i++)
    {
        perecritieCount += static_cast<int>(perecritie[i]); // складываем все перекрытия
    }
    //СКРЫТО qDebug() << "Всего перекрытий: " << perecritieCount << " из " << horizontalSize;
    //===============================
    // рассчитываем ДНФ с ядрами
    // надо доработать тупиковые ДНФ
    //    onlyCores = !(perecritieCount < horizontalSize); // если количество перекрытий меньше, чем всего столбцов. Т.е. если все столбцы закрываются ядрами, будет TRUE
    // сначала считаем итоговую МДНФ только по ядрам
    QString itogMdnf; //итоговый МДНФ со скобками и т.д.
    //СКРЫТО qDebug() << "coresList.size() = " << coresList.size();
    int coresSize = coresList.size(); // получаем размер списка ядер
    QStringList minterms; // создаём список миндермов
    // тут считаем ТОЛЬКО ядровую ДНФ
    for (int i=0; i<coresSize; i++)
    {
        QString minterm = ""; // создаем минтерм
        //СКРЫТО qDebug() << "Ядровый элемент: " << coresList[i];
        minterm = createMintermBySkleyka(coresList[i], type); // создаём минтерм
        //СКРЫТО qDebug() << "itogMdnf = " << itogMdnf << " до добавления";
        if(!itogMdnf.contains(minterm))
        {
            if((itogMdnf.count() > 0) && (minterm != "")) // если в мднф уже содержатся скобки
            {
                if(type) // если МДНФ
                {
                    itogMdnf.append(" v "); // добавляем знак дизъюнкции
                    //                minterm = " v " + minterm;
                }
                else // если МКНФ
                {
                    itogMdnf.append(" ^ "); // добавляем знак коньюнцкию
                    //                minterm = " ^ " + minterm;
                }
            }
            //СКРЫТО qDebug() << "itogMdnf = " << itogMdnf << " после добавления, минтерма = " <<  minterm;
            itogMdnf.append(minterm); // добавляем минтерм в итоговую формулу мднф
            //СКРЫТО qDebug() << "itogMdnf = " << itogMdnf << " после добавления минтерма";
            minterms.append(minterm); // добавляем минтерм в список минтермов
        }


    }
    //    }
    //СКРЫТО qDebug() << "F(mdnf) = " << itogMdnf;
    if(itogMdnf.isEmpty())
    {
        //СКРЫТО qDebug() << "Не удалось посчитать МДНФ! - Нет ядровых функций";
        //        return false;
    }
    //    coreMdnf = itogMdnf;
    //=======================================================================================================
    // теперь создаем список из возможных тупиковых ДНФ, причем ядровая ДНФ уже построена
    minList.clear(); // очищаем список
    coreFormula = itogMdnf; // возвращаем ядровую функцию
    if(itogMdnf != "") // если ядровая ДНФ не пустая
    {
        minList.append(itogMdnf); // добавляем ядровую функцию
    }
    // ======
    //  рассчитываем все возможные МНФ
    // ======
    if(perecritieCount < horizontalSize) // если количество перекрытий меньше, чем всего столбцов
    {
        //СКРЫТО qDebug() << "Присутствует вариативная часть!";
        for (int i=0; i<horizontalSize; i++) // проверяем каждый столбец
        {
            if(!perecritie[i]) // если нет перекрытия этого столбца
            {
                QStringList mintermsForThis; // список, в который будут занесены все ментермы для текущего значения
                //СКРЫТО qDebug() << "Вариативная часть в значении " << horizontalList[i]; // выводим для дебага, в каком значении ест ьвариативная часть, т.е ДНФ туп.
                //                int n = 0; // это временная переменная. Сейчас она сохраняет последнюю склейку, которая подходит к этому столбцу, но в дальнейшем надо будет сделат ьпроверку на минимальную склейку и добавить различные варианты
                int countVars = 0; // хранит сколько макимально имеется переменных в склейке
                // === тут находим, какое максимальное количество Х может содержать склейка, которая подойдёт под это условие
                for (int j=0; j<vertikalSize; j++)
                {
                    if(cores[i][j]) // если в этом столбце есть +
                    {
                        QString skleyka = vertikalList[j]; // получаем склейку
                        QStringList skleykaList = skleyka.split("", Qt::SplitBehavior(Qt::SkipEmptyParts)); // разбиваем склейку на символы
                        int size = skleykaList.size(); // получаем количество элементов в склейке
                        int count = 0; // сохраняет, сколько переменых (Х) в склейке
                        for (int h=0; h<size; h++)
                        {
                            if(skleykaList[h] == "X") // если символ в этой склейке равен Х, т.е. переменной
                            {
                                count++; // увеличиваем свётчик
                            }
                        }
                        if(count > countVars) // если текующее количество Х больше, чем у предыдщего
                        {
                            //                            n = j; // то схраняем номер этой склейки
                            countVars = count;
                        }
                        //                        n = j;
                    }
                }
                // определили, какое максимальное количество Х содержится в склейках, которые подходят под это значение, теперь ищем количетсво склеек с максимальным количеством Х
                //                int n = 0; // это временная переменная. Сейчас она сохраняет последнюю склейку, которая подходит к этому столбцу, но в дальнейшем надо будет сделат ьпроверку на минимальную склейку и добавить различные варианты
                //                int countVars = 0; // хранит сколько макимально имеется переменных в склейке
                //                int countMaxX = 0; // эта переменная будет хранить сколько склеек содержат максимальное количество Х для этого значения
                QStringList skleykiX; // список, хранящий склейки, подходящие для этого значения
                for (int j=0; j<vertikalSize; j++)
                {
                    if(cores[i][j]) // если в этом столбце есть +
                    {
                        QString skleyka = vertikalList[j]; // получаем склейку
                        QStringList skleykaList = skleyka.split("", Qt::SplitBehavior(Qt::SkipEmptyParts)); // разбиваем склейку на символы
                        int size = skleykaList.size(); // получаем количество элементов в склейке
                        int count = 0; // сохраняет, сколько переменых (Х) в склейке
                        for (int h=0; h<size; h++)
                        {
                            if(skleykaList[h] == "X") // если символ в этой склейке равен Х, т.е. переменной
                            {
                                count++; // увеличиваем свётчик
                            }
                        }
                        if(count == countVars) // если текующее количество Х совпадает с максимальным, т.е. эта склейка подходит под условие минимальности содержания переменных
                        {
                            skleykiX.append(vertikalList[j]); // добавляем склейку в список
                        }
                    }
                }
                // выводим для дебага все склейки, подходящие под это условие
                for (int g=0; g<skleykiX.size(); g++)
                {
                    //СКРЫТО qDebug() << "Значение " << horizontalList[i] << "имеет вариативную часть " << skleykiX[g]; // выводим вариативную часть
                }
                //                disunctList.append(skleykiX); // добавляем список склеек в общий список диъюнктов
                //                coresList.append(vertikalList[n]); // добавляем склейку к ядрам
                //                int minListSize = minList.size(); // получаем количестево элементов в текущем списке
                int skleikiXListSize = skleykiX.size(); // получаем количество элементов в списке склеек, которые нам подходят
//                QString newCoresList; // создаём новый список с ядрами
                for (int j=0; j<skleikiXListSize; j++)
                {
                    // создаём минтерм на основе склейки
                    QString minterm = createMintermBySkleyka(skleykiX[j], type); // создаем минтерм
                    // обновляем лист с ядрами
                    //СКРЫТО qDebug() << "Минтерм: " << minterm; // выводим получившийся минтерм
                    mintermsForThis.append(minterm); // добавляем к списку минтермов
                }
                // сформирован список минтермов
                int mintermsCount = mintermsForThis.size(); // поулчаем, сколько всего минтермов мы получили
                if(mintermsCount == 0)
                {
                    //СКРЫТО qDebug() << "количество минтермов = 0";
                    continue;
                }
                QStringList *mintermsList = new QStringList [mintermsCount]; // создаем списки для хранения всех тупиковых ДНФ с этим минтермом
                int minListSize = minList.size(); // получаем количество элементов в списке МДНФ
                for (int g=0; g<mintermsCount; g++)
                {
                    //                    QString mdnfLine = ""; // переменная для хранения строки из МДНФ
                    for (int h=0; h<minListSize; h++)
                    {
                        QString radelitel = ""; // переменная, которая отвечает за знак между скобками, в зависимости от МДНФ или МКНФ
                        if(type)
                        {
                            radelitel = " v ";
                        }
                        else
                        {
                            radelitel = " ^ "  ;
                        }
                        if(!minList[h].contains(mintermsForThis[g])) // если такой минтерм еще не содержится в формуле
                        {
                            mintermsList[g].append(minList[h] + radelitel + mintermsForThis[g]); // создаем МДНФ с текущим минтермом
                        }
                        else // если содержится
                        {
                            mintermsList[g].append(minList[h]); // создаем МДНФ с текущим минтермом
                        }
//                        mintermsList[g].append(minList[h] + radelitel + mintermsForThis[g]); // создаем МДНФ с текущим минтермом
//                        qDebug() << "mintermsForThis[" + QString::number(g) +"] = " << mintermsForThis[g];

                        //                        mdnfLine.append(minList[h]);
                        //                        mdnfLine.append(" v ");
                    }
                }
                // получили все возможные варианты МДНФ с учетом текущего минтерма
                if(minList.size() == 0)
                {
                    minList = mintermsForThis; // приравниваем к текущим минтермам
                    //СКРЫТО qDebug() << "новый размер МДНФ - " << minList.size();
                }
                else
                {
                    minList.clear(); // ощищаем список МДНФ
                    for (int g=0; g<mintermsCount; g++)
                    {
                        minList.append(mintermsList[g]); // добавляем новые ДНФ, уже с учетом текущих тупиковых
                    }
                }
                delete [] mintermsList;
            }
        }
//        for (int g=0; g<minList.size(); g++)
//        {
//            //СКРЫТО qDebug() << "МДНФ " << g << " = " << minList[g]; // выводим, какие МДНФ получились
//        }
    }
    delete [] perecritie;
    // проверка, что все минимальные функции действительно минимальны
    int minFuncVarsCount = 999999999; // создаём переменную, в которой будем хранить минимальное количсетво переменных, среди всех функций
    for(int i=0; i<minList.size(); i++)
    {
        int count = getVarsCountInFunc(minList.at(i)); // временная переменная для подсчёта переменных внутри функции
        //        for(int j=0; j<minList.at(i).count(); j++) // проходим по всем символам данной функции
        //        {
        //            if((minList.at(i).toLower() >= "a") && (minList.at(i).toLower() <= "d")) // если это переменная
        //            {
        //                count++; // увеличиваем счётчик
        //            }
        //        }
        if(count < minFuncVarsCount) // если в этой функции переменных меньше, чем в предыдущей сохраненной
        {
            minFuncVarsCount = count; // сохраняем значение текущей переменной
        }
    }
    QStringList newMinList; // новый список с минимальными функциями
    for(int i=0; i<minList.size(); i++) // теперь проходим заново, но уже откидываем те функции, где количество переменных не равно минимальному
    {
        int count = getVarsCountInFunc(minList.at(i)); // временная переменная для подсчёта переменных внутри функции
        //        for(int j=0; j<minList.at(i).count(); j++) // проходим по всем символам данной функции
        //        {
        //            if((minList.at(i).toLower() >= "a") && (minList.at(i).toLower() <= "d")) // если это переменная
        //            {
        //                count++; // увеличиваем счётчик
        //            }
        //        }
        if(count == minFuncVarsCount) // если в этой функции переменных совпадает с минимальным
        {
            newMinList.append(minList.at(i)); // добавляем эту функцию в список
        }
    }
    varsCount = minFuncVarsCount; // передаём количество логических переменных по адресу (из параметра)
    minList.clear(); // очищаем старый список
    minList = newMinList; // приравневаем старый список к новому
    mintermsCount = getMintermsCountInFunc(newMinList.at(0), type); // возвращаем количество минтермов (макстермов)
    // очистка памяти
    for (int i=0; i<horizontalSize; i++) // отчищаем память
    {
        delete [] cores[i]; // удаляем память об ядрах
        cores[i] = nullptr;
    }
    delete [] cores;
    cores = nullptr;
    for (int i=0; i<3; i++)
    {
        skleyki[i].clear(); // отчищаем листы со склейками
    }
    return true;
}

int mdnfMacKlassky::getVarsCountInFunc(QString function)
{
    int count = 0; // временная переменная для подсчёта переменных внутри функции
    if(function.count() == 0)
    {
        return 0;
    }
    for(int j=0; j<function.count(); j++) // проходим по всем символам данной функции
    {
        if((function.at(j).toLower() >= "a") && (function.at(j).toLower() <= "d")) // если это переменная
        {
            count++; // увеличиваем счётчик
        }
    }
    return count;
}

int mdnfMacKlassky::getMintermsCountInFunc(QString function, int type)
{
    if(function.isEmpty() || (type < 0) || (type > 1)) // если функция пустая, или тип не 0 и не 1
    {
        return -1;
    }
    int count = 0; // переменная для подсчёта количества разделителей
    QString razdelitel;
    function.replace("v", "+"); // заменяем обозначения для удобства
    function.replace("^", "*");
    if(type == 0) // если МКНФ
    {
        razdelitel = "*";
    }
    else if(type == 1) // если МДНФ
    {
        razdelitel = "+";
    }
    for(int i=0; i<function.count(); i++)
    {
        if(function.at(i) == razdelitel) // если текущий символ являертся разделителем
        {
            count++; // увеличиваем значениме счётчика
        }
    }
    count += 1; // увеличиваем значение на 1, т.к. количество скобок всегда на 1 больше, чем количество знаков
    return count;
}

void mdnfMacKlassky::clearData()
{
    // очищаем все переменные
    allowTot = false;
    allowMdnf = false;
    allowMknf = false;
    sschTableList.clear();
    listOnes.clear();
    skleyki1.clear();
    skleyki2.clear();
    skleyki3.clear();
    sizeFunc = -1;
    onlyCores = false;
    rang = -1;
    for(int i=0; i<3; i++)
    {
        skleyki[i].clear();
    }
    //    skleykiCount = 0;
    rowsTot = 0;
    colsTot = 0;
    funcList.clear();
}

bool mdnfMacKlassky::to2ssch(QString number16, QString &number2)
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

bool mdnfMacKlassky::to16ssch(QString number2, QString &number16)
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

void mdnfMacKlassky::makeSkleyki(QStringList numbersList, QStringList &skleykiList, bool &wasSkleyka)
{
    QStringList listFunc = numbersList; // объявляем для удобства
    int sizeFunc = listFunc.size(); // поулчаем размер списка чисел
    // первичные проверки
    if(sizeFunc <= 0)
    {
        return;
    }
    int razryad = listFunc[0].count(); // получаем количество символов в одном числе
    if(razryad <= 0)
    {
        return;
    }
    for (int i=0; i<sizeFunc; i++)
    {
        if(numbersList[i].count() != razryad) // если где-то есть пустоты, значит данные ошибочны
        {
            //СКРЫТО qDebug() << "Разряды числа " << numbersList[i] << " не равны количеству разрядов " << razryad << i;
            //СКРЫТО qDebug() << "Разрядность: " << listFunc[0];
            return;
        }
    }
    // разбиваем на разряды для поиска тех, которые можно склеить
    QString **elementsOnes = new QString *[sizeFunc]; // создаём двумерный массив для хранения разрядов
    for (int i=0; i<sizeFunc; i++)
    {
        elementsOnes[i] = new QString [razryad];
    }
    for (int i=0; i<sizeFunc; i++)
    {
        QString chislo = listFunc[i]; // получаем число
        QStringList chisloList = chislo.split("", Qt::SplitBehavior(Qt::SkipEmptyParts));  // разбиваем число на символы
        int size = chisloList.size(); // получаем размер числа (количество символов)
        if(razryad != size) // если количество разрядов не совпадает, проверяем на всякий случай
        {
            //СКРЫТО qDebug() << "Разряд числа не совпадает с количеством символов в сисле таблицы!";
            for(int row=0; row<sizeFunc; row++)
            {
                delete [] elementsOnes[row];
            }
            delete [] elementsOnes;
            return;
        }
        for (int j=0; j<razryad; j++)
        {
            elementsOnes[i][j] = chisloList[j];   // сохраняем каждое число по символам
        }
    }
    // проверяем строки и создаем склейки
    QStringList skleyki1; // будем сохранять склейки
    bool *proshliSkleiku = new bool [sizeFunc]; // создаем массив, который будет хрнатиь информацию о том, прошли ли склейку элементы, размер равен количеству элементов
    for (int i=0; i<sizeFunc; i++)
    {
        proshliSkleiku[i] = false; // задаём, что никто не прошел склейку
    }
    for (int i=0; i<sizeFunc; i++) // проходим по каждому значению
    {
        for (int j=0; j<i; j++) // проходим по каждому разряду каждого значения
        {
            bool *ok = new bool [razryad]; // создаем логический массив для хранения даннх об одинаковости разрядов чисел
            int count = 0; // сохраняет количество элементов, которые отличаются
            for (int k=0; k<razryad; k++)
            {
                ok[k] = (elementsOnes[i][k] == elementsOnes[j][k]); // сохраняет, равны-ли два элемента
                count += static_cast<int>(!ok[k]); // прибавляем 0 или 1 в зависимости от того, равны элеменнты, или нет
            }
            //СКРЫТО qDebug() << "число 1: " << listFunc[i] << "число 2: " << listFunc[j] << "count = " << count;
            if(count == 1) // если разница только в одном разряде
            {
                proshliSkleiku[i] = true; // задаем, что 2 элемента прошли склейку
                proshliSkleiku[j] = true;
                QString skleyka; // для формирования склейки по символам
                for (int g=0; g<razryad; g++) // проверяем каждый разряд
                {
                    if(ok[g]) // если разряды совпадают
                    {
                        skleyka.append(elementsOnes[i][g]); // добавляем константу в склейку
                    }
                    else
                    {
                        skleyka.append("X"); // добавляем Х где разряд меняется
                    }
                }
                skleyki1.append(skleyka); // добавляем склейку в список
                //СКРЫТО qDebug() << "СКЛЕЙКА: " << skleyka << skleyki1[skleyki1.size() - 1];
            }
            delete [] ok;
            ok = nullptr;
//            count = 0;
        }
    }

    for(int row=0; row<sizeFunc; row++)
    {
        delete [] elementsOnes[row];
    }
    delete [] elementsOnes;

    if(!skleyki1.size()) // если не было склеек
    {
        //СКРЫТО qDebug() << "нет склеек";
        skleykiList = numbersList; // новая склейка = старая склейка
        wasSkleyka = false; // ставим, что не было склейки
        int s1 = numbersList.size();
        for(int i=0; i<s1; i++)
        {
            //СКРЫТО qDebug() << "numberList[" << i << "] = " << numbersList[i];
        }
        delete [] proshliSkleiku;
        return; // выходим из функции, т.к. дальше обрабатывать нет смысла
    }
    wasSkleyka = true; // ставим, склейка произошла
    for (int i=0; i<sizeFunc; i++)
    {
        if(!proshliSkleiku[i]) // если какой-то элемент не прошёл склейку
        {
            skleyki1.append(listFunc[i]); // добавляем число как константу
        }
    }

    delete [] proshliSkleiku;

    // удаляем повторяющиеся
    int skleyki1Size = skleyki1.size();
    for (int i=0; i < skleyki1Size; i++)
    {
        bool povtor = false; // определяет, повторяется ли элемент
        for (int j=0; j<i; j++)
        {
            if(skleyki1[i] == skleyki1[j])
            {
                povtor = true; // повторяется
            }
        }
        if(!povtor)
        {
            skleykiList.append(skleyki1[i]); // если элемент списка не повторяется, то добавляем его
        }
    }
    //    skleykiCount++;
}

void mdnfMacKlassky::fillMassiveStepenTwo(QString **&masive, int rows, int cols)
{
    //    int rows = ui->tableWidget_tableOfTrue->rowCount(); // получаем количество строк в таблице
    //    int cols = ui->tableWidget_tableOfTrue->columnCount(); // получаем количество колонок в таблице
    if(!rows || !cols)
    {
        //        QMessageBox::critical(this, "Ошибка", "Задайте размер таблицы истиности!");
        //        criticalError("Задайте размер таблицы истиности!");
        return;
    }
    //    QString *mas =
    // заполняем числами от 0 до N в 2й ССЧ
    //    int maxcol = cols-2; // крайняя правая колонка, которую надо заполнить (она будет нулевым разрядом числа)
    for (int i=0; i<cols+1; i++)
    {
        int n = static_cast<int>(pow(2, i)); // возводим 2 в степень
        int k = 0; // сколько строк уже заполнено
        bool chislo = false; // число 0 или 1, которое записывается в таблицу
        int colnumber = cols-i; // вычисляем, в какую колонку будем записывать
        //СКРЫТО qDebug() << "i = " << i << "colnumber = " << colnumber;
        for (int i=0; i<rows; i++)
        {
            if(k < n) // если число цифр меньше степени, то записываем число
            {
                //                setTWText(ui->tableWidget_tableOfTrue, i, colnumber, QString::number(static_cast<int>(chislo))); // записывем значение числа (0 или 1)
                masive[i][colnumber] = QString::number(static_cast<int>(chislo));
                k++; // увелдичивам счётчик на 1
            }
            else // иначе изменяем число на противоположное (0 или 1) и заполняем дальше
            {
                chislo = !chislo; // инвертируем
                k = 0; // обнуляем счётчик
                //                setTWText(ui->tableWidget_tableOfTrue, i, colnumber, QString::number(static_cast<int>(chislo))); // записывем значение числа (0 или 1)
                masive[i][colnumber] = QString::number(static_cast<int>(chislo));
                k++; // увелдичивам счётчик на 1
            }
        }
    }
}

bool mdnfMacKlassky::calculateTot(QString function_16)
{
    // первичные проверки
    //СКРЫТО qDebug() << "Число: " << function_16;
    if(function_16 == "") // проверяем на пустоту
    {
        //СКРЫТО qDebug() << "Поле ввода функции пустое!";
        error = "Поле ввода функции пустое!";
        return false;
    }
    // переводим число в 2 ССЧ
    QString ch2;
    if(!to2ssch(function_16, ch2)) // проверяем перевод в 2ю ССЧ
    {
        //СКРЫТО qDebug() << "Введена не 16-я функция!\nПроверьте правильность ввода!";
        error = "Введена не 16-я функция!\nПроверьте правильность ввода!";
        return false;
    }
    int razryad = function_16.count(); // поличество разрядов в исходном числе
    //СКРЫТО qDebug() << ch2; // выводим результат перевода
    //СКРЫТО qDebug() << "=======";
    // заносим в таблицу нашу функцию
    // определяем, сколько переменных нужно, чтобы описать функцию
    funcList = ch2.split("", Qt::SplitBehavior(Qt::SkipEmptyParts)); // разбиваем число в 2й ССЧ на символы
    rowsTot = static_cast<int>(4 * razryad); // умножаем 4 (т.к. у нас 16-я ССЧ и она содержит по 4 двоичных разряда в числе) на количество разрядов числа и получаем количество строк
    double stepen = log2(rowsTot); // получаем, в какую степень над овозвести числ о2, чтобы получить такую длину числа
    if(abs(stepen - static_cast<double>(static_cast<int>(stepen))) > 0.0) // если степень не целая
    {
        stepen = static_cast<double>(static_cast<int>(stepen)); // откидываем дробную часть
        stepen += 1.0; // прибавляем 1
    }
    colsTot = static_cast<int>(stepen) + 1;// количство столбцов равно степени в которую надо возвезти 2, чтобы получить количество строк + 1 для функции
    if(!rowsTot || !colsTot)
    {
        return false;
    }
    for (int i=0; i<rowsTot; i++)
    {
        QString num = QString::number(i, 2); // перевод из 10 в 16
        while (num.count() < colsTot-1) // пока количество разрядов меньше, чем количество колонок -1, т.к. последняя колонка это значение функции
        {
            num = "0" + num;
        }
        //СКРЫТО qDebug() << num;
        sschTableList.append(num);
    }
    int sschTableListSize = sschTableList.size();
    for (int i=0; i<sschTableListSize; i++)
    {
        sschTableList[i] = sschTableList[i] +  funcList[i]; // добавляем значение функции к элементу таблицы истинности
    }
    //СКРЫТО qDebug() << "закончено";
    allowTot = true; // сохраняем, что таблица истинности рассчитана
    return true;
}

QString mdnfMacKlassky::getError() const
{
    return error;
}

QStringList mdnfMacKlassky::getMdnfList(int type)
{
    if(type == 0)
    {
        if(mknf)
        {
            return mknf->getMinList();
        }
        else
        {
            return QStringList();
        }
    }
    else if(type == 1)
    {
        if(mdnf)
        {
            return mdnf->getMinList();
        }
        else
        {
            return QStringList();
        }
    }
    else
    {
        return QStringList();
    }
}

int mdnfMacKlassky::getOptimizeType(QString func_2)
{
    int count = func_2.count(); // получаем количество символов
    if(count == 0) // если нет символов
    {
        return -1;
    }
    int zeroCount = 0; // счётчик нулей
    int oneCount = 0; // счётчик единиц
    for (int i=0; i<count; i++)
    {
        if((func_2.at(i) != "0") && (func_2.at(i) != "1"))    // если символ не 0 и не 1
        {
            return -1;
        }
        if(func_2.at(i) == "0")
        {
            zeroCount++;
        }
        if(func_2.at(i) == "1")
        {
            oneCount++;
        }
    }
    //СКРЫТО qDebug() << "Исходная функция:" << func_2 << "Количество нулей: " << zeroCount << " Количество единиц: " << oneCount;
    if(zeroCount < oneCount) // если нулей меньше, чем единиц
    {
        //СКРЫТО qDebug() << "Количество нулей меньше, чем единиц";
        if(allowTot) // если объект мак класки создан
        {
            //СКРЫТО qDebug() << "Можно посчитать таблицу истинности";
            if(canCalculateMnf(0)) // проверяем, можно-ли рассчитать МКНФ
            {
                return 0;
            }
            else
            {
                //СКРЫТО qDebug() << "Нельзя посчитать МКНФ!";
            }
        }
    }
    if(zeroCount > oneCount) // если нулей больше, чем единиц
    {
        //СКРЫТО qDebug() << "Количество единиц меньше, чем нулей";
        if(allowTot) // если объект мак класки создан
        {
            //СКРЫТО qDebug() << "Можно посчитать таблицу истинности";
            if(canCalculateMnf(1)) // проверяем, можно-ли рассчитать МДНФ
            {
                return 1;
            }
            else
            {
                //СКРЫТО qDebug() << "Нельзя посчитать МДНФ!";
            }
        }
    }
    if(zeroCount == oneCount) // если количество равно
    {
        bool canCalculateMknf = false; // флаг возможности рассчёта МКНФ
        bool canCalculateMdnf = false; // флаг возможности рассчёта МДНФ
        if(allowTot) // если объект мак класки создан
        {
            canCalculateMknf = canCalculateMnf(0); // считаем возможность рассчёта МКНФ
            canCalculateMdnf = canCalculateMnf(1); // считаем возможность рассчёта МДНФ
            if(canCalculateMknf && canCalculateMdnf) // если можно рассчитать оба варианта
            {
                return 2;
            }
            else if(canCalculateMknf) // если МДНФ рассчитать нельзя, но МКНФ можно
            {
                return 3;
            }
            else if(canCalculateMdnf) // если МКНФ рассчитать нельзя, но МДНФ можно
            {
                return 4;
            }
        }
        return 2;
    }
    return -1;
}

int mdnfMacKlassky::getOptimizeType16(QString func_16)
{
    QString num_2;
    if(to2ssch(func_16, num_2)) // переводим функцию в 2 ссч
    {
        return getOptimizeType(num_2); // возвращаем значение переведенной в 2 ссч функции
    }
    else // если не удалось перевести
    {
        return -1;
    }
}

//QStringList *mdnfMacKlassky::getMdnfList2() const
//{
//    return mdnfList2;
//}

//QStringList mdnfMacKlassky::getMdnfListByFunction(QString function, int type)
//{
//    QStringList list; // создаем список, который вернём
//    if(!calculate(function, type, skleyki[0], skleyki[1], skleyki[2], coresMdnf, coresListMdnf, mdnfList)) // пытаемся рассчитать функцию
//    {
//        //СКРЫТО qDebug() << "Не удалось посчитать данную функцию!" << function;
//        return list;
//    }
//    list = getMdnfList(); // получаем список МДНФ текущей функции
//    return list;
//}

//bool mdnfMacKlassky::isMdnfByFunction(QString function, QString virazhenie, int type)
//{
//    setFunction(function);
//    if(!calculate(function, type)) // пытаемся рассчитать функцию
//    {
//        //СКРЫТО qDebug() << "Не удалось посчитать данную функцию!" << function;
//        return false;
//    }
//    return isMdnf(virazhenie);
//}

QString mdnfMacKlassky::createMintermBySkleyka(QString skleyka, int type)
{
    // создаём минтерм на основе склейки
    if(skleyka == "")
        return "";
    //    if(!skleyka.contains("X"))
    //    {
    //        return "";
    //    }
    QString minterm = ""; // создаем минтерм
    QStringList oneCoreList = skleyka.split("", Qt::SplitBehavior(Qt::SkipEmptyParts)); // разбиваем склейку на символы
    int count = oneCoreList.size(); // получаем размер склейки
    const char start_variable = 'a'; // буква первой переменной
    for (int k=0; k<count; k++)
    {
        if(oneCoreList[k] != "X") // если число
        {
            if(minterm.count()) // если в минтерме есть переменные
            {
                if(type) // если выбран МДНФ
                {
                    minterm.append(" ^ "); // добавляем знак коньюнкции
                }
                else
                {
                    minterm.append(" v "); // добавляем знак дизъюнкции
                }
            }
            if(oneCoreList[k].toInt() != type) // если число является противоположностью типа (0 для МДНФ и 1 - для МКНФ)
            {
                minterm.append("!"); // добавляем знак инверсии
            }
            minterm.append(start_variable + static_cast<char>(k)); // добавляем переменную
        }
    }
    minterm = "(" + minterm + ")"; // добавляем скобки к минтерму
    return minterm;
}

//QStringList mdnfMacKlassky::getCoresListMdnf() const
//{
//    return coresListMdnf;
//}

bool mdnfMacKlassky::isMnf(QString virazhenie, int type)
{
    if(type == 0) // если выбрана МКНФ
    {
        return mknf->isNf(virazhenie);
    }
    else // если выбрана МДНФ
    {
        return mdnf->isNf(virazhenie);
    }
}

bool mdnfMacKlassky::canCalculateMnf(int typeMf)
{
    if(!allowTot) // если не рассчитана таблица истинности, выходим
    {
        return false;
    }
    if(typeMf == 0) // если выбран тип МКНФ
    {
        return mknf->isAllowCalculate(); // возвращаем результат возможности рассчёта МКНФ
    }
    else if(typeMf == 1) // если выбран тип МДНФ
    {
        return mdnf->isAllowCalculate(); // возвращаем результат возможности рассчёта МДНФ
    }
    else // если указанно не 0 и не 1
    {
        return false;
    }
}


QStringList mdnfMacKlassky::getSschTableList() const
{
    return sschTableList;
}

int mdnfMacKlassky::getSkleykiMaxNumber(int typeMnf)
{
    if(typeMnf == 0)
    {
        return mknf->skleykiCount;
    }
    else if (typeMnf == 1)
    {
        return mdnf->skleykiCount;
    }
    else
    {
        return -1;
    }
}

int mdnfMacKlassky::getVarsCount(int typeMnf)
{
    if(typeMnf == 0)
    {
        return mknf->varsCount;
    }
    else if (typeMnf == 1)
    {
        return mdnf->varsCount;
    }
    else
    {
        return -1;
    }
}

int mdnfMacKlassky::getMintermsCount(int typeMnf)
{
    if(typeMnf == 0)
    {
        return mknf->mintermsCount;
    }
    else if (typeMnf == 1)
    {
        return mdnf->mintermsCount;
    }
    else
    {
        return -1;
    }
}

QString mdnfMacKlassky::getCoresMfFormula(int typeMnf)
{
    if(typeMnf == 0)
    {
        return mknf->coreMfFormula;
    }
    else if (typeMnf == 1)
    {
        return mdnf->coreMfFormula;
    }
    else
    {
        return "";
    }
}


//int mdnfMacKlassky::getSkleykiMaxNumber() // получить номер последнего этапа склейки
//{
//    return skleykiCount; // вызвращаем количество склеек
//}


QStringList mdnfMacKlassky::getSkleyki3(int type) const
{
    if(type == 0) // выбран МКНФ
    {
        return mknf->getSkleyka3();
    }
    else // если выбран МДНФ
    {
        return mdnf->getSkleyka3();
    }
}

QStringList mdnfMacKlassky::getSkleyki2(int type) const
{
    if(type == 0) // выбран МКНФ
    {
        return mknf->getSkleyka2();
    }
    else // если выбран МДНФ
    {
        return mdnf->getSkleyka2();
    }
}

QStringList mdnfMacKlassky::getSkleyki1(int type) const
{
    if(type == 0) // выбран МКНФ
    {
        return mknf->getSkleyka1();
    }
    else // если выбран МДНФ
    {
        return mdnf->getSkleyka1();
    }
}

QStringList mdnfMacKlassky::getListOnes(int type) const
{
    if(type == 0) // выбран МКНФ
    {
        return mknf->getListFunc(); // возвращает список значений, при которых функция равна 0
    }
    else // если выбран МДНФ
    {
        return mdnf->getListFunc(); // возвращает список значений, при которых функция равна 1
    }
}

QStringList mdnfMacKlassky::mnf::getMinList() const
{
    return minList;
}


bool mdnfMacKlassky::mnf::isAllowCalculate() const
{
    return allowCalculate;
}

void mdnfMacKlassky::mnf::setAllowCalculate(bool value)
{
    allowCalculate = value;
}

int mdnfMacKlassky::mnf::getTypeNf() const
{
    return typeNf;
}

bool mdnfMacKlassky::mnf::isNf(QString virazhenie)
{
    QString userMdnf = virazhenie; // получаем текст из строки, куда вводится МДНФ
    qDebug() << "МДНФ ввдённая пользователем:" << userMdnf;
    if(userMdnf.isEmpty()) // если строка пустая
    {
        qDebug() << "Не введена МДНФ";
        return false;
    }
    QString razdelitel = ""; // символ, который является разделителем. Для МКНФ это ^, для МДНФ это v
    if(typeNf == 0) // если тип МКНФ
    {
        razdelitel = "^"; // разделитель - коньюнкция
    }
    else // если тип МДНФ
    {
        razdelitel = "v"; // разделитель - дизъюнкция
    }
    userMdnf.replace('+', 'v'); // переводим + в дизъюнкцию
    userMdnf.replace('*', '^'); // переводим * в коньюнкцию
    int skobkaStartCount = 0; // переменная для подсчёта открывающихся скобок - (
    int skobkaEndCount = 0; // переменная для подсчёта закрывающихся скобок - )
    int razdelitelCount = 0; // количество разделителей
    for (int i=0; i<userMdnf.count(); i++)
    {
        qDebug() << razdelitel << userMdnf.at(i);
        if(userMdnf.at(i) == "(")
        {
            skobkaStartCount++;
        }
        if(userMdnf.at(i) == ")")
        {
            skobkaEndCount++;
        }
        if(userMdnf.at(i) == razdelitel)
        {
            razdelitelCount++;
        }
    }
    if(skobkaStartCount != skobkaEndCount) // если количество скобок разное
    {
        qDebug() << "Количество скобок разное " << skobkaStartCount << skobkaEndCount;
        //        warningError("Ошибка! Проверьте скобки!");
        return false;
    }
    if(typeNf == 0) // если тип МКНФ
    {
        if(razdelitelCount != (skobkaStartCount - 1)) // если количество разделителей не совпадает с количеством скобок (-1, т.к. между 2 скобками будет один разделитель), сравниваем равенство только с количество м одних скобок, т.к. количество разных скобок проверено до этого
        {
            qDebug() << "Не все макстермы выделены в скобки!" << razdelitelCount  << skobkaStartCount << skobkaStartCount-1;
            return false;
        }
    }
    //    QString mdnfReal = mdnf->getMdnf(); // получаем МДНФ, которая должна быть
    //    qDebug() << "МДНФ, которая должна быть:" << mdnfReal;
    qDebug() << "Удаляем лишнее";
    userMdnf.remove(" "); // удаляем пробелы
    userMdnf.remove("("); // убираем скобки
    userMdnf.remove(")");
    qDebug() << "Удалили. Переходим к обработке";
    userMdnf.remove("");
    QStringList userMdnfList = userMdnf.split(razdelitel, Qt::SplitBehavior(Qt::SkipEmptyParts)); // разбиваем строку по коньюнкциям, чтобы получить список с ДНФ
    qDebug() << "разбили на символы" << userMdnfList;
    int userMdnfListSize = userMdnfList.size(); // получаем количество элементов в пользовательском МДНФ
    // сортируем переменные по возрастанию внутри каждого минтерма
    for(int i=0; i<userMdnfListSize; i++) // проходим по каждому минтерму
    {
        if(mdnfMacKlassky::getVarsCountInFunc(userMdnfList.at(i)) > 4) // если в функции содержится больше 4 переменных
        {
            return false;
        }
        qDebug() << "Получили пользовательскую мднф: \t" << userMdnfList;
        int var[4]; // массив для хранения даннх о переменных в минтерме. -1 - переменная отсутствует, 0 - переменная с инверсией, 1 - переменная без инверсии
        for(int j=0; j<4; j++)
        {
            var[j] = -1;
        }
        for(int j=0; j<userMdnfList.at(i).size(); j++) // проходим по каждому значению в минтерме
        {
            if(userMdnfList.at(i).at(j).toLower() >= "a" && userMdnfList.at(i).at(j).toLower() <= "d") // если символ a-d
            {
                QString symvol = userMdnfList.at(i).at(j).toLower(); // сохраняем символ
                bool inversion = false; // флаг наличия инверсии у элемента
                if(j != 0) // если элемент не является первым
                {
                    if(userMdnfList.at(i).at(j-1) == "!") // если предыдущий символ - инверсия
                    {
                        inversion = true;
                    }
                }
                int n = 0; // переменная для хранения номер элемента
                if(symvol == "a")
                    n = 0;
                else if(symvol == "b")
                    n = 1;
                else if(symvol == "c")
                    n = 2;
                else if(symvol == "d")
                    n = 3;
                if(inversion) // если у элемента есть инверсия
                {
                    var[n] = 0; // ставим переменной значение с инверсией
                }
                else
                {
                    var[n] = 1; // ставим значение с инверсией
                }
            }
        }
        QString newMinterm = ""; // теперь создаем минтерм с отсортированными элементами
        for(int j=0; j<4; j++)
        {
            if(var[j] != -1)
            {
                char ch = 'a' + j;
                if(j != 0) // если элемент не первый
                {
                    QString obratniyRazdelitel; // если медлу скобками стоит конбюнкция, то внутри скобки дизъюнкцию, и наоборот
                    if(razdelitel == "^")
                    {
                        obratniyRazdelitel = "v";
                    }
                    else
                    {
                        obratniyRazdelitel = "^";
                    }
                    if(newMinterm != "")
                    {
                        newMinterm.append(obratniyRazdelitel); // добавляем разделитель
                    }
                }
                if(var[j] == 0) // если элемент с инверсией
                {
                    newMinterm.append("!"); // добавляем знак инверсии
                }
                newMinterm.append(ch); // добавляем значение
            }
            qDebug() << "j = " << j << " minterm = " << newMinterm;
        }
        userMdnfList.replace(i, newMinterm); // заменяем исходный минтерм на новый, отсортированный
        qDebug() << "Полученный минтерм: " << userMdnfList[i];
    }
    qDebug() << "получили размер";
    bool mdnfYes = false; // переменная, которая хранит, есть-ли совпадение с МДНФ из списка
    qDebug() << "начинаем считать";
    qDebug() << getMinList();
    QStringList mdnfList3 = getMinList(); // получаем список возможных минимальных функций
    int size = mdnfList3.size(); // получаем размер списка
    if(size == 0)
    {
        qDebug() << "size == 0";
        return false;
    }
    qDebug() << "Получили размер списка - " << size;
    for (int g=0; g<size; g++)
    {
        qDebug() << "Рассматриваем МДНФ номер " << g;
        QString mdnfReal = mdnfList3[g];
        if(mdnfMacKlassky::getVarsCountInFunc(userMdnf) != mdnfMacKlassky::getVarsCountInFunc(mdnfReal)) // если количество переменных в пользовательской функции не совпадает с количеством переменных в функции, построенной программой
        {
            qDebug() << "Количество переменных не совпадает: " << mdnfMacKlassky::getVarsCountInFunc(userMdnf) << mdnfMacKlassky::getVarsCountInFunc(mdnfReal);
            qDebug() << "МДНФ, которая должна быть: " << mdnfReal;
            continue;
        }
        mdnfReal.remove(" "); // убираем пробелы
        mdnfReal.remove("(");
        mdnfReal.remove(")");
        qDebug() << "Пользовательская МДНФ после обработки:" << userMdnf;
        qDebug() << "МДНФ, которая должна быть, после обработки:" << mdnfReal;
        // проверяем МДНФ
        // разбиваем МДНФ на коньюнкты
        QStringList mdnfRealList = mdnfReal.split(razdelitel, Qt::SplitBehavior(Qt::SkipEmptyParts)); // разбиваем МНФ, которая рассчитана программой на элементы
        qDebug() << "список реальной мднф: \t\t" << mdnfRealList;
        qDebug() << "список пользовательской мднф: \t" << userMdnf.split(razdelitel, Qt::SplitBehavior(Qt::SkipEmptyParts));
        qDebug() << "список пользовательской мднф 2: \t" << userMdnfList;
        int mdnfRealListSize = mdnfRealList.size(); // получаем количество элементов в МДНФ, которое должно получиться
        bool mdnfError = false; // переменная отвечающая за то, что все ДНФ совпадают
        for (int i=0; i<userMdnfListSize; i++) // проверяекм каждую конкретную ДНФ, которую ввёл ползователь
        {
            bool dnfOk = false; // переменная отвечающая за то, что конкретная ДНФ присутсвует
            for (int j=0; j<mdnfRealListSize; j++)
            {
                if(userMdnfList[i] == mdnfRealList[j]) // если ДНФ совпадают
                {
                    dnfOk = true; // устанавливаем, что ДНФ совпадают
                    qDebug() << "Элементы " << i << userMdnfList[i] << "совпадают";
                    continue; // завершаем текущий цикл
                }
                else
                {
                    qDebug() << "Элементы " << i << userMdnfList[i] << "НЕ совпадают";
                }
            }
            if(!dnfOk) // если совпадения не найдены
            {
                mdnfError = true; // устанавливаем ошибку
                break;
            }
        }
        if(!mdnfError) // если нет ошибки в проверке МДНФ
        {
            mdnfYes = true;
            break;
        }
    }
    return mdnfYes;
}

void mdnfMacKlassky::mnf::clear()
{
    listFunc.clear();
    skleyka1.clear();
    skleyka2.clear();
    skleyka3.clear();
    minList.clear();
    coreMfFormula.clear();
    allowCalculate = false;
    skleykiCount = -1;
    varsCount = -1;
}

QStringList mdnfMacKlassky::mnf::getListFunc() const
{
    return listFunc;
}

QStringList mdnfMacKlassky::mnf::getSkleyka1() const
{
    return skleyka1;
}

QStringList mdnfMacKlassky::mnf::getSkleyka2() const
{
    return skleyka2;
}

QStringList mdnfMacKlassky::mnf::getSkleyka3() const
{
    return skleyka3;
}

void mdnfMacKlassky::mnf::setType(int type)
{
    if(changedType)
    {
        //СКРЫТО qDebug() << "Не удалось сменить тип, он уже установлен!";
        return;
    }
    typeNf = type;
    changedType = true;
}
