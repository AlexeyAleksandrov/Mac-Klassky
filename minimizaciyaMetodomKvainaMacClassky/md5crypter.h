#ifndef MD5CRYPTER_H
#define MD5CRYPTER_H

#include <QString>
#include <QStringList>
#include <QCryptographicHash>

#define HASH_TAG "HASH="    // тег хеша в строке

class md5crypter
{
public:
    md5crypter() {}

    // добавление хеша
    static QString crypt(QString sourcetext) // возвращает текст с добавленным в него хешем
    {
        QString outputtext = sourcetext;
        QString hash = QString(QCryptographicHash::hash(sourcetext.toLatin1(),QCryptographicHash::Md5).toHex()); // получаем хэш
        outputtext = sourcetext; // записываем исходный текст
        outputtext.append(HASH_TAG); // добавляем тэг хеш
        outputtext.append(hash); // добавляем сам хеш
        return outputtext;
    };

    inline static void cryptStr(QString &sourcetext) // добавление хеша в строку
    {
        sourcetext = crypt(sourcetext);
    };

    // извлечение хеша
    static QString decrypt(QString cryptedtext, bool &ok) // возвращает текст без хеша, проверяя корректность
    {
        if(!cryptedtext.contains(HASH_TAG)) // если строка не содержит тега хеша
        {
            ok = false;
            return cryptedtext;
        }
        QStringList fileHexList = cryptedtext.split(HASH_TAG); // разбиваем на до и после хэша
        if(fileHexList.size() != 2) // если не получилось разбить
        {
            ok = false;
            return cryptedtext;
        }
        QString hashFromFile = fileHexList.at(1); // получаем хэш из файла
        QString hash = QString(QCryptographicHash::hash(fileHexList.at(0).toLatin1(),QCryptographicHash::Md5).toHex()); // получаем хэш содержимого
        if(hashFromFile != hash) // если хеши различаются
        {
            ok = false;
            return cryptedtext;
        }
        QString outputtext = fileHexList.at(0); // возвращаем всё, что до хеша
        ok = true;
        return outputtext;
    };

    inline static void decryptStr(QString &sourcetext, bool &ok) // просто вытащить хеш из строки и проверить его правильность
    {
        sourcetext = decrypt(sourcetext, ok);
    };
};

#endif // MD5CRYPTER_H
