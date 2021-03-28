#include "loadfunctions.h"
#include  <QMessageBox>

#define VARS_COUNT_EASY     0
#define VARS_COUNT_MEDIUM   3
#define VARS_COUNT_HARD     4

#define TYPEMF_MKNF 0
#define TYPEMF_MDNF 1
#define TYPEMF_ANY  2

#define ALLOW_VARS_COUNT_1  3
#define ALLOW_VARS_COUNT_2  4

loadFunctions::loadFunctions(QObject *parent) : QObject(parent)
{

}

loadFunctions::~loadFunctions()
{

}

bool loadFunctions::load(QLabel *flabel, /*QProgressBar *fprogressBar,*/ QList<hexFunction> *list, QList<hexFunction> *list_2, bool &fgeneratedFunctions, bool isStarting)
{
//    if(fgeneratedFunctions)
//    {
//        return;
//    }
    isLoading = true;
    generatedFunctions = &fgeneratedFunctions;
    label = flabel;
//    progressBar = fprogressBar;
    file = new QFile("vectors.txt"); // создаем файл
    bool existFile;
    if(file->exists()) // если файл существует
    {
        if(!file->open(QIODevice::ReadOnly))
        {
            //                    QMessageBox::critical(this, "Ошибка!", "Не удалось открыть файл с вариантами!");
            isLoading = false;
            return false;
        }
        file->close();
        existFile = true;
    }
    else
    {
        existFile = false;
    }
    if(!existFile)
    {
        qDebug() << "Файл не найден";

        if(isStarting)
        {
            isLoading = false;
            return false;
        }

        // очищаем все списки
        if(list != nullptr)
        {
            list->clear();
        }
        else
        {
            list = new QList<hexFunction>;
        }
        if(list_2 != nullptr)
        {
            list_2->clear();
        }
        else
        {
            list_2 = new QList<hexFunction>;
        }

        // генерирует функции
        list_local = new QList<hexFunction>;

        label->setText("Генерация функций. Это может занять некоторое время...");
        QApplication::processEvents();

        min = 0; // 0000
        max = 65535; // FFFF

        // диапазоны расчё1тов для 4х потоков
        from[0] = min;
        to[0] = max/THREAD_COUNT;
        for(int i=1; i<THREAD_COUNT; i++)
        {
            from[i] = (to[0] * i) + 1;
            to[i] = to[0] * (i + 1);
        }
        to[THREAD_COUNT-1] = max + 1;

        // перемещаем генераторы в потоки
        qDebug() << "Главный поток: " << QThread::currentThreadId();

//         проверяем на пустоту потоки генераторов
        if(fthread != NULL)
        {
            try {
                delete [] fthread;
                fthread = NULL;
            }  catch (std::bad_alloc &e) {
                qDebug() << "delete [] fthread" << e.what();
            }
//            delete [] fthread;
//            fthread = NULL;
            qDebug() << "Удаляем старые потоки";
        }

        qDebug() << "создаём потоки";

        try {
            fthread = new QThread [THREAD_COUNT];
        }  catch (std::bad_alloc &e) {
            qDebug() << "fthread = new QThread" << e.what();
        }
//        fthread = new QThread [THREAD_COUNT];


        // проверяем на пустоту сами генераторы
        if(fgenerator != NULL)
        {
            try {
                delete [] fgenerator;
                fgenerator = NULL;
            }  catch (std::bad_alloc &e) {
                qDebug() << "delete [] fgenerator" << e.what();
            }
//            delete [] fgenerator;
//            fgenerator = NULL;
            qDebug() << "Удаляем старые генераторы";
        }

        qDebug() << "создаём генераторы";
//        fgenerator = new functionsGenerator [THREAD_COUNT];
        try {
            fgenerator = new functionsGenerator [THREAD_COUNT];
        }  catch (std::bad_alloc &e) {
            qDebug() << "fgenerator = new functionsGenerator" << e.what();
        }

        qDebug() << "всё создано";

        if(fthread == NULL)
        {
            qDebug() << "Потоки не выделены";
            isLoading = false;
            return false;
        }

        list->clear();
        list_2->clear();
        list_local->clear();
        threadFinishedCount = 0;
;
        for(int i=0; i<THREAD_COUNT; i++)
        {
            // сигналы
            connect(&fthread[i], &QThread::started, &fgenerator[i], &functionsGenerator::run);
            connect(&fgenerator[i], &functionsGenerator::finish, &fthread[i], &QThread::quit); // соединяем сигнал окончания
//            connect(&fgenerator[i], &functionsGenerator::updateProgressBar, this, &loadFunctions::updateProgressBar);
            connect(&fthread[i], &QThread::finished, this, &loadFunctions::onThreadFinished);
            // устанавливаем параметры и запускаем
            fgenerator[i].setData(from[i], to[i], list, list_2, list_local, functionsCalculateCount);
            fgenerator[i].moveToThread(&fthread[i]); // перемещаем объект в поток
            fthread[i].start();
            qDebug() << "Вызван поток #" + QString::number(i+1);
        }
    }
    else
    {
        qDebug() << "Файл найден";

        if(fthread != NULL)
        {
            try {
                delete fthread;
                fthread = NULL;
            }  catch (std::bad_alloc &e) {
                qDebug() << "delete fthread" << e.what();
            }
            qDebug() << "Удаляем старые потоки";
        }

        if(floader != NULL)
        {
            try {
                delete floader;
            }  catch (std::bad_alloc &e) {
                qDebug() << "delete floader; " << e.what();
            }
        }

        // создаём поток и загрузчик
        fthread = new QThread;
        floader = new ftloader;

        // соединяем сигналы и слоты
        connect(fthread, &QThread::started, floader, &ftloader::run);
        connect(floader, &ftloader::finish, fthread, &QThread::quit);
        connect(fthread, &QThread::finished, this, &loadFunctions::onLoadFromFileFinished);

        // настраиваем загрузчик, перемещаем в поток и заупскаем
        floader->setData(list, list_2, label);
        floader->moveToThread(fthread);
        fthread->start();
    }
    isLoading = false;
    return true;
}

void loadFunctions::message(QString text, QString title/*, int type*/)
{
    //    if(type == 0)
    {
        //        QMessageBox::information(this, title, text);
        QMessageBox msg;
        msg.setText(text);
        msg.setToolTip(title);
        msg.show();
    }
    //    else if(type == 1)
    //    {
    //        QMessageBox::warning(this, title, text);
    //    }
    //    else if(type == 2)
    //    {
    //        QMessageBox::critical(this, title, text);
    //    }
}

QString loadFunctions::getDataFromLineFile(QString line, QString key, bool *ok)
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

bool loadFunctions::getIsLoading() const
{
    return isLoading;
}

void loadFunctions::onThreadFinished()
{
    threadFinishedCount++;
    qDebug() << "Завершено " + QString::number(threadFinishedCount) + " потоков из " << QString::number(THREAD_COUNT);

    if(threadFinishedCount == THREAD_COUNT)
    {
//        for(int i=0; i<THREAD_COUNT; i++)
//        {
//            fthread[i].terminate();
//        }
//         очищаем потоки
        if(fthread != NULL)
        {
            try {
                delete [] fthread;
                fthread = NULL;
            }  catch (std::bad_alloc &e) {
                qDebug() << "delete [] fthread in end" << e.what();
            }
//            delete [] fthread;
//            fthread = NULL;
        }
        if(fgenerator != NULL)
        {
            try {
                delete [] fgenerator;
                fgenerator = NULL;
            }  catch (std::bad_alloc &e) {
                qDebug() << "delete [] fgenerator in end" << e.what();
            }
//            delete [] fgenerator;
//            fgenerator = NULL;
        }


//        for(int i=0; i<THREAD_COUNT; i++)
//        {

//        }

        qDebug() << "Все потоки завершены";
        int size = list_local->size(); // получаем количество элементов
        if(!file->open(QIODevice::WriteOnly))
        {
            qDebug() << "Не удалось открыть файл";
            *generatedFunctions = true;
            label->setText("Функции расчитаны");
            qDebug() << "Функции расчитаны";
            progressBar->setVisible(false);
            delete file;
            file = nullptr;
            list_local->clear();
            delete list_local;
            list_local = nullptr;
            return;
        }
        QTextStream *ts = new QTextStream(file);
        for (int i=0; i<size; i++)
        {
            *ts << "func16 : " << list_local->at(i).func16 << "\t"
                   //            << list->at(i).typeMnf << "\t"
                << "canCalculateMknf : " << list_local->at(i).canCalculateMknf << "\t"
                << "canCalculateMdnf : " << list_local->at(i).canCalculateMdnf << "\t"
                << "varsCountMknf : " << list_local->at(i).varsCountMknf << "\t"
                << "varsCountMdnf : " << list_local->at(i).varsCountMdnf << "\t"
                << "skleykiCountMknf : " << list_local->at(i).skleykiCountMknf << "\t"
                << "skleykiCountMdnf : " << list_local->at(i).skleykiCountMdnf << "\t"
                << "maxtermsCount : " << list_local->at(i).maxtermsCount << "\t"
                << "mintermsCount : " << list_local->at(i).mintermsCount << "\t"

                << "\r\n";
        }
            qDebug() << "3";

        *generatedFunctions = true;
        label->setText("Функции расчитаны");
        qDebug() << "Функции расчитаны";
        if(progressBar)
        {
            progressBar->setVisible(false);
        }
        delete ts;
        ts = nullptr;
        file->close();
        delete file;
        file = nullptr;
        list_local->clear();
        delete list_local;
        list_local = nullptr;
    }
}

//void loadFunctions::updateProgressBar()
//{
//    progressBar->setValue(functionsCalculateCount);
////    qDebug() << "updateProgressBar()";
//    //    QApplication::processEvents();
//}

void loadFunctions::onLoadFromFileFinished()
{
    qDebug() << "Удаление потока";
    delete fthread;
    delete floader;
    qDebug() << "Всё удалено";
    *generatedFunctions = true;
}
