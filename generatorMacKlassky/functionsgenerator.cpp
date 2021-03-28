#include "functionsgenerator.h"
#include <QApplication>


functionsGenerator::functionsGenerator(QObject *parent) : QObject(parent)
{

}

void functionsGenerator::generate(int i, funcList *list, funcList *list_2, funcList *list_local)
{
    QString func = QString::number(i, 16).toUpper(); // переводим из 10 в 16 ссч
    if((func.count() != ALLOW_VARS_COUNT_1) && (func.count() != ALLOW_VARS_COUNT_2)) // если один или 3 символа
    {
//        continue; // пропускаем
        return ;
    }
     mdnfMacKlassky *mdnf_local = NULL;
    try
    {
        mdnf_local = new mdnfMacKlassky;
    }
    catch (std::exception &e)
    {
        qDebug() << "new mdnfMacKlassky " << e.what();
    }

//    if(mdnf_local != nullptr)
//    {
////        delete mdnf_local;
////        mdnf_local = new mdnfMacKlassky();
//        try
//        {
//            delete mdnf_local;
//        }
//        catch (std::exception &e)
//        {
//            qDebug() << "delete mdnf_local " << e.what();
//        }
//        try
//        {
//            mdnf_local = new mdnfMacKlassky;
//        }
//        catch (std::exception &e)
//        {
//            qDebug() << "new mdnfMacKlassky " << e.what();
//        }
//    }
//    else
//    {
////        mdnf_local = new mdnfMacKlassky();
//        try
//        {
//            mdnf_local = new mdnfMacKlassky;
//        }
//        catch (std::exception &e)
//        {
//            qDebug() << "new mdnfMacKlassky " << e.what();
//        }
//    }
     if(func == "5FFB")
     {
         qDebug() << "Пропускаем 5FFB";
         delete mdnf_local;
         return;
     }

    if(mdnf_local->setFunction(func)) // проверяем, возможно ли посчитать
    {
        hexFunction *function = NULL; // создаем экземпляр функции
        try
        {
            function = new hexFunction;
        }
        catch (std::exception &e)
        {
            qDebug() << "new hexFunction " << e.what();
        }
        function->varsFuncCount = func.count(); // сохраняем количество переменных
        function->func16 = func; // сохраняем саму фнкцию
        //                function->typeMnf = mdnf->getOptimizeType16(func); // получаем оптимальный тип минимизации
        function->canCalculateMknf = mdnf_local->canCalculateMnf(TYPEMF_MKNF);
        function->canCalculateMdnf = mdnf_local->canCalculateMnf(TYPEMF_MDNF);

        // формируем параметры МКНФ
        if(function->canCalculateMknf)
        {
            function->varsCountMknf = mdnf_local->getVarsCount(TYPEMF_MKNF);
            function->skleykiCountMknf = mdnf_local->getSkleykiMaxNumber(TYPEMF_MKNF);
            function->maxtermsCount = mdnf_local->getMintermsCount(TYPEMF_MKNF);
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
            function->varsCountMdnf = mdnf_local->getVarsCount(TYPEMF_MDNF);
            function->skleykiCountMdnf = mdnf_local->getSkleykiMaxNumber(TYPEMF_MDNF);
            function->mintermsCount = mdnf_local->getMintermsCount(TYPEMF_MDNF);
        }
        else
        {
            function->varsCountMdnf = -1;
            function->skleykiCountMdnf = -1;
            function->mintermsCount = -1;
        }
        function->used = 0; // ставим, что не использовалась
        fmutex.lock(); // блокирем поток
        list_local->append(*function); // добавляем функцию в лист
        if(function->varsFuncCount == ALLOW_VARS_COUNT_1)
        {
            list_2->append(*function);
        }
        else if (function->varsFuncCount == ALLOW_VARS_COUNT_2)
        {
            list->append(*function);
        }
        fmutex.unlock(); // снимаем блокировку потока

        try {
            delete function; // удаляем память
            function = nullptr; // зануляем указатель
            delete mdnf_local;
            mdnf_local = nullptr;
        }
        catch (std::exception &e)
        {
            qDebug() << " delete mdnf_local" << e.what();
        }
//        delete function; // удаляем память
//        function = nullptr; // зануляем указатель
//        delete mdnf_local;
//        mdnf_local = nullptr;
    }
}

void functionsGenerator::generateBetween(/*int from, int to, funcList *list, funcList *list_2, funcList *list_local*/)
{
    qDebug() << "Поток: " << QThread::currentThreadId();
    for(int i=from; i<to; i++)
    {
        generate(i, list, list_2, list_local);
    }
    emit finish();
}

void functionsGenerator::setData(int ffrom, int fto, funcList *flist, funcList *flist_2, funcList *flist_local, int &ffunctionsCalculateCount)
{
    from = ffrom;
    to = fto;
    list = flist;
    list_2 = flist_2;
    list_local = flist_local;
    functionsCalculateCount = &ffunctionsCalculateCount;
}

void functionsGenerator::run()
{
//    generateBetween();


    qDebug() << "Поток: " << QThread::currentThreadId() << " запущен";
    QApplication::processEvents();

    mdnfMacKlassky *mdnf_local = new mdnfMacKlassky;
    hexFunction *function = new hexFunction; // создаем экземпляр функции

    for(int i=from; i<to; i++)
    {
        fmutex.lock();
        functionsCalculateCount++;
        emit updateProgressBar();
        fmutex.unlock();

//        qDebug() << "Поток: " << QThread::currentThreadId() << " рассчитывает #" + QString::number(i);
        QApplication::processEvents();
        QString func = QString::number(i, 16).toUpper(); // переводим из 10 в 16 ссч
//        qDebug() << "Func: " << func;
//        if(func == "5FFB")
//        {
//            continue;
//        }
        if((func.count() != ALLOW_VARS_COUNT_1) && (func.count() != ALLOW_VARS_COUNT_2)) // если один или 3 символа
        {
            continue; // пропускаем
//            return ;
        }
//        mdnfMacKlassky *mdnf_local = new mdnfMacKlassky;
//        if(mdnf_local != nullptr)
//        {
//            delete mdnf_local;
//            mdnf_local = new mdnfMacKlassky();
//        }
//        else
//        {
//            mdnf_local = new mdnfMacKlassky();
//        }
//        QStringList ignoreFuncs;
//        ignoreFuncs << "5FFB" << "5FFE" << "7674" << "759F" << "7548" << "74AD" << "75EF" << "7641" << "75D0" << "7617" << "75E7";
//        if(ignoreFuncs.contains(func))
//        {
////            qDebug() << "5FFB: Начинаем считать";
//            qDebug() << "Пропускаем" << func;
//            continue;

//        }
        if(mdnf_local->setFunction(func)) // проверяем, возможно ли посчитать
        {
            if(func == "5FFB") qDebug() << "5FFB: Установлено";
//            hexFunction *function = new hexFunction; // создаем экземпляр функции
            function->varsFuncCount = func.count(); // сохраняем количество переменных
            function->func16 = func; // сохраняем саму фнкцию
            //                function->typeMnf = mdnf->getOptimizeType16(func); // получаем оптимальный тип минимизации
            function->canCalculateMknf = mdnf_local->canCalculateMnf(TYPEMF_MKNF);
            function->canCalculateMdnf = mdnf_local->canCalculateMnf(TYPEMF_MDNF);

            // формируем параметры МКНФ
            if(function->canCalculateMknf)
            {
                function->varsCountMknf = mdnf_local->getVarsCount(TYPEMF_MKNF);
                function->skleykiCountMknf = mdnf_local->getSkleykiMaxNumber(TYPEMF_MKNF);
                function->maxtermsCount = mdnf_local->getMintermsCount(TYPEMF_MKNF);
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
                function->varsCountMdnf = mdnf_local->getVarsCount(TYPEMF_MDNF);
                function->skleykiCountMdnf = mdnf_local->getSkleykiMaxNumber(TYPEMF_MDNF);
                function->mintermsCount = mdnf_local->getMintermsCount(TYPEMF_MDNF);
            }
            else
            {
                function->varsCountMdnf = -1;
                function->skleykiCountMdnf = -1;
                function->mintermsCount = -1;
            }
            function->used = 0; // ставим, что не использовалась
            fmutex.lock(); // блокирем поток
            if(list_local == NULL)
            {
                qDebug() << "list_local is NULL";
                emit finish();
            }
            list_local->append(*function); // добавляем функцию в лист
            if(function->varsFuncCount == ALLOW_VARS_COUNT_1)
            {
                if(list_2 == NULL)
                {
                    qDebug() << "list_2 is NULL";
                    emit finish();
                }
                list_2->append(*function);
            }
            else if (function->varsFuncCount == ALLOW_VARS_COUNT_2)
            {
                if(list == NULL)
                {
                    qDebug() << "list is NULL";
                    emit finish();
                }
                list->append(*function);
            }
            fmutex.unlock(); // снимаем блокировку потока

//            delete function; // удаляем память
//            function = nullptr; // зануляем указатель
//            delete mdnf_local;
//            mdnf_local = nullptr;
        }
    }

    delete function; // удаляем память
    function = nullptr; // зануляем указатель
    delete mdnf_local;
    mdnf_local = nullptr;


    qDebug() << "Поток: " << QThread::currentThreadId() << " завершён";
    QApplication::processEvents();
    emit finish();




//    emit terminate();

//    quit();
//    quit();
}
