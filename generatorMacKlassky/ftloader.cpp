#include "ftloader.h"

#define VARS_COUNT_EASY     0
#define VARS_COUNT_MEDIUM   3
#define VARS_COUNT_HARD     4

ftloader::ftloader(QObject *parent) : QObject(parent)
{
}

void ftloader::run()
{
    qDebug() << "Поток запущен";
    QFile *file = NULL;
    try {
        file = new QFile("vectors.txt"); // создаем файл
    }  catch (std::exception &e) {
        qDebug() << "fgenerator = new functionsGenerator" << e.what();

        emit finish();
        return;
    }

    if(file->exists()) // если файл существует
    {
        if(!file->open(QIODevice::ReadOnly))
        {
            message("Не удалось открыть файл векторов!");
            emit finish();
            return;
        }
        QString text = file->readAll(); // считываем весь файл
        file->close();
//        bool okhesh = false;
//        md5crypter::decryptStr(text, okhesh); // проверяем хеш
//        if(!okhesh)
//        {
//            message("Не удалось открыть файл векторов!");
//            emit finish();
//            return;
//        }
        QStringList fileList = text.split("\r\n", Qt::SplitBehavior(Qt::SkipEmptyParts)); // разбиваем на строчки
        qDebug() << "Размер файла: " << fileList.size();

        label->setText("Идёт загрузка функций...");
        qDebug() << "Идёт загрузка функций...";

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

        for (int i=0; i<fileList.size(); i++) // проходим по каждой строке
        {
            QStringList line =  fileList[i].split("\t", Qt::SplitBehavior(Qt::SkipEmptyParts)); // разбиваем строку
            if(line.size() < 2) // если меньше 2 элементов, пропускаем
            {
                qDebug() << "Строка " << i << "содержит меньше 2х элементов!" << fileList[i];
                continue;
            }
            hexFunction *func = new hexFunction;

            func->func16 = getDataFromLineFile(fileList[i], "func16"); // считываем функцию
            func->varsFuncCount = func->func16.count();
            func->canCalculateMknf = getDataFromLineFile(fileList[i], "canCalculateMknf").toInt();
            func->canCalculateMdnf = getDataFromLineFile(fileList[i], "canCalculateMdnf").toInt();
            func->varsCountMknf = getDataFromLineFile(fileList[i], "varsCountMknf").toInt();
            func->varsCountMdnf = getDataFromLineFile(fileList[i], "varsCountMdnf").toInt();
            func->skleykiCountMknf = getDataFromLineFile(fileList[i], "skleykiCountMknf").toInt();
            func->skleykiCountMdnf = getDataFromLineFile(fileList[i], "skleykiCountMdnf").toInt();
            func->maxtermsCount = getDataFromLineFile(fileList[i], "maxtermsCount").toInt();
            func->mintermsCount = getDataFromLineFile(fileList[i], "mintermsCount").toInt();
            func->used = 0; // задаем как не использовалась

            if(func->varsFuncCount == VARS_COUNT_MEDIUM)
            {
                list_2->append(*func);
            }
            else if(func->varsFuncCount == VARS_COUNT_HARD)
            {
                list->append(*func); // добавляем функцию в лист
            }
            delete func;
            func = nullptr;
        }
        label->setText("Удаление временных файлов...");
    }
    delete file;
    file = nullptr;
    qDebug() << "Функции загружены";
    if(list->size() == 0 || list_2->size() == 0)
    {
        message("Не удалось получить функции!");
        label->setText("Не удалось получить функции!");
        qDebug() << list->size() << list_2->size();
        emit finish();
        return;
    }
    label->setText("Загрузка списка функций завершена!");
    QString styleSheet = "background-color: none;";
    label->setStyleSheet(styleSheet);
    qDebug() << "Поток завершён";
    emit finish();
}

void ftloader::setData(/*QProgressBar *fprogressBar, */QList<hexFunction> *flist, QList<hexFunction> *flist_2, QLabel *flabel)
{
//    progressBar = fprogressBar;
    list = flist;
    list_2 = flist_2;
    label = flabel;
}

QString ftloader::getDataFromLineFile(QString line, QString key, bool *ok)
{
    QStringList lineList =  line.split("\t", Qt::SplitBehavior(Qt::SkipEmptyParts)); // разбиваем строку
    for (int i=0; i<lineList.size(); i++)
    {
        QStringList list = lineList.at(i).split(":", Qt::SplitBehavior(Qt::SkipEmptyParts)); // разбиваем каждую пару на ключ : значение
        if(list.size() >= 2)
        {
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
        else
        {
            qDebug() << "Ошибка в строке: " << lineList.at(i);
        }
    }
    if(ok != nullptr)
    {
        *ok = false;
    }
    return "";
}

void ftloader::message(QString text, QString title)
{

    QMessageBox msg;
    msg.setText(text);
    msg.setToolTip(title);
    msg.show();
}
