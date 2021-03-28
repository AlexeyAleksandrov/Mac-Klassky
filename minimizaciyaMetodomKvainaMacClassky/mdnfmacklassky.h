#ifndef MDNFMACKLASSKY_H
#define MDNFMACKLASSKY_H

#define MKNF_OR_MDNF_ONLY false

#include <QObject>
#include <QStringList>
#include <QFile>
#include <QApplication>

class mdnfMacKlassky : public QObject
{
    Q_OBJECT
public:
    explicit mdnfMacKlassky(QObject *parent = nullptr);


//    ~mdnfMacKlassky();

    // функции для пользователя:

    bool setFunction(QString function_16); // функция, устанавливающая вектор функции, возвращает false, если вектор некорректный
    bool isMnf(QString virazhenie, int type); // проверяет, является ли функция МДНФ данного выражения
    bool canCalculateMnf(int typeMf); // возвращает, можно-ли посчитать данный тип минимизации для этой функции
    int getOptimizeType(QString func_2); // возвращает, какой лучше тип (МКНФ/МДНФ) выбрать

    QStringList getSschTableList() const; // список таблицы истинности
    int getOptimizeType16(QString func_16); // возвращает, какой тип лучше использовать
    QStringList getListOnes(int type) const; // список со значениями, при которых функция равно 0 или 1
    QStringList getSkleyki1(int type) const; // список со склейками
    QStringList getSkleyki2(int type) const;
    QStringList getSkleyki3(int type) const;
    int getSkleykiMaxNumber(int typeMnf); // получить номер последнего этапа склейки
    int getVarsCount(int typeMnf); // получить количество логических переменных в МНФ
    int getMintermsCount(int typeMnf); // получить количество минтермов (макстермов)
    QString getCoresMfFormula(int typeMnf); // возвращает формулу ядровой МФ


    void makeSkleyki(QStringList numbersList, QStringList &skleykiList, bool &wasSkleyka); // функция, которая производит склейки, type 0 (МКНФ) 1 (МДНФ), wasSkleyka отвечает за то, была-ли произведена склейка элементов, или-же этап просто скопирован

    QString getError() const;

    QStringList getMdnfList(int type); // возвращаент список всех возможных МДНФ

//    int getType(QString func_2, QStringList listFunc); // возвращает определнный тип минимизации 0 - МКНФ, 1 - МДНФ, -1 - не удалось определить

private:
    // всё, что нужно для таблицы истинности
    bool to2ssch(QString number16, QString &number2); // перевод из 16 в 2ю
    bool to16ssch(QString number2, QString &number16); // перевод из 16 в 2ю
    void fillMassiveStepenTwo(QString **&mas, int rows, int cols); // 0 и 1, формируя таблицу истинности для n степени двойки
    bool calculateTot(QString function_16); // функция, считающая таблицу истинности для функции

    // рассчёт минимизации
    // ==== >>> CALCULATE <<< ==== Главная функция =====

    bool calculate(int type, QStringList &listFunc, QStringList &skleyka1, QStringList &skleyka2, QStringList &skleyka3, QStringList &minList, int &varsCount, int &skleykiCount, int &mintermsCount, QString &coreFormula, QString func_16 = ""); // type = 1 - МДНФ, type = 0 - МКНФ, minList - список, куда вернётся функция

    // =================================================
    static int getVarsCountInFunc(QString function); // возвращает количество переменных в данной функции
    int getMintermsCountInFunc(QString function, int type); // возвращает количество минтермов (макстермов) в зависимости от типа в заданной функции
    QStringList skleyki[3]; // храним рельтат склеек

    void clearData(); // очищает все переменные (кроме структур)

    // variables
    QString vector_16 = ""; // переменная, хранящая текущий вектор

    QStringList sschTableList;
    int rowsTot = 0; // количество строк в таблице истинности
    int colsTot = 0; // количество столбцов в таблице истинности
    QStringList funcList; // хранит значения функции в 2 ссч по одному символу
    QStringList listOnes;


    QStringList skleyki1;
    QStringList skleyki2;
    QStringList skleyki3;
    int sizeFunc = -1;
//    int perecritieCount = -1;
    bool onlyCores = false;
    int rang = -1; // ранг (количество различных переменных
//    QStringList mdnfList; // список из возможных тупиковых ДНФ
//    QStringList *mdnfList2 = nullptr; // дубль mdnfList, только указатель
//    bool **coresMdnf = nullptr; // массив, который будет хранить данные об наличии плюсиков в стобцах [номер стобца][номер строки]
    QStringList coresListMdnf; // список, который будет хранить ядра функции
    QString error = "Ошибок выполнения нет!"; // переменная, которая хранит текст ошибки
    bool allowTot = false; // хранит, рассчитана-ли таблица истинности
    bool allowMdnf = false; // хранит, можно-ли рассчитать МДНФ
    bool allowMknf = false; // хрнаит, можно-ли рассчитать МКНФ


protected slots:
    QString createMintermBySkleyka(QString skleyka, int type); // преобразует склейку в минтерм, например, 0XX1 в (!a ^ d), зависит от типа - 0 - МКНФ, 1 -МДНФ

signals:

public slots:

private: // структуры
    struct mnf // минимальная нормальная форма
    {
        int typeNf = 0; // тип МКНФ или МДНФ
        bool changedType = false; // переменная, которая фиксирует изменение типа, это костыль, чтобы нельзя было поменять  тип МДНФ на МКНФ и наоборот. То есть тип задаётся только один раз
        QStringList listFunc; // список значений, при которых функция принимает 0 или 1 (в зависимости от выбранного
        QStringList skleyka1; // список для склейки 1
        QStringList skleyka2; // список для склейки 2
        QStringList skleyka3; // список для склейки 3
        QString coreMfFormula = ""; // формула ядровой МФ
        QStringList minList; // список для хранения всех возможных минимальных функций
        bool allowCalculate = false; // отвечает, можно-ли вообще построить эту НФ
        int skleykiCount = -1; // переменная для хранения количсетва склеек в этой МНФ
        int varsCount = -1; // хранит количество логических переменных в данной МНФ
        int mintermsCount = -1; // хранит количество минтермов (макстермов) в данной МНФ


        QStringList getListFunc() const;
        QStringList getSkleyka1() const;
        QStringList getSkleyka2() const;
        QStringList getSkleyka3() const;
        QStringList getMinList() const;

        void setType(int type);
        bool isAllowCalculate() const;
        void setAllowCalculate(bool value);
        int getTypeNf() const;
        bool isNf(QString virazhenie); // функция, отвечающая за проверку, является ли переданное в функцию выражение минимальным для данного типа (МКНФ/МДНФ)
        void clear(); // очистка всех полей структуры
    };

    mnf *mdnf; // создаём структуру для МДНФ
    mnf *mknf; // создаём структуру для МКНФ
};

#endif // MDNFMACKLASSKY_H
