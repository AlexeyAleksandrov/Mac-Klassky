#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mdnfmacklassky.h"
//#include "formloadingvectors.h"
#include <QList>
#include <QLineEdit>
#include <QTableWidget>
#include <QMessageBox>
#include <qdebug.h>
#include "hexfunction.h"
#include "loadfunctions.h"
#include "presets.h"
#include "exeleditor.h"
#include <QObject>
#include <QCryptographicHash>
#include "md5crypter.h"
#include <QTime>

#define STR_SEPARATOR_1 "\r\n"
#define STR_SEPARATOR_2 "\t"
#define FILE_STUDENTS_NAME "students.mirea"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool needClose = false;
//    void loadFunctions(); // функция, вызываяемая на старте, для загрузки функций из файла

public slots:
    void on_pushButton_loadFuncs_clicked();

private slots:

    void on_pushButton_generate_2_clicked();

    void on_pushButton_saveToFile_clicked();

    void on_pushButton_loadStudentsFile_clicked();

    void on_pushButton_chose_clicked();

    void on_pushButton_noChose_clicked();

    void on_pushButton_clearAllChose_clicked();

    void on_radioButton_generateParam_forChoseGroup_clicked();

    void on_radioButton_generateParam_forChosedInTable_clicked();

    void on_radioButton_generateParam_forAll_clicked();

    void on_comboBox_generateParam_forChosedGroup_currentIndexChanged(int index);

    void on_lineEdit_mintermsCount_from_mdnf_textChanged(const QString &arg1);

    void on_lineEdit_mintermsCount_to_mdnf_textChanged(const QString &arg1);

    void on_lineEdit_varsCountInLastFunc_from_mdnf_textChanged(const QString &arg1);

    void on_lineEdit_varsCountInLastFunc_to_mdnf_textChanged(const QString &arg1);

    void on_lineEdit_mintermsCount_from_mknf_textChanged(const QString &arg1);

    void on_lineEdit_mintermsCount_to_mknf_textChanged(const QString &arg1);

    void on_lineEdit_varsCountInLastFunc_from_mknf_textChanged(const QString &arg1);

    void on_lineEdit_varsCountInLastFunc_to_mknf_textChanged(const QString &arg1);

    void on_pushButton_clearFilter_clicked();

    void updateDataText(QString text);

private:
    Ui::MainWindow *ui;

    struct studentStruct
    {
        QString name = ""; // имя
        QString group = ""; // группа
        QString function = ""; // функция
    };

    struct generateFilter
    {
        int vars = -1;
        int skleykiMdnf = -1;
        int skleykiMknf = -1;
        int mintermsCountFrom = -1;
        int mintermsCountTo = -1;
        int maxtermsCountFrom = -1;
        int maxtermsCountTo = -1;
        int logicalVarsCountFrom_mdnf = -1;
        int logicalVarsCountTo_mdnf = -1;
        int logicalVarsCountFrom_mknf = -1;
        int logicalVarsCountTo_mknf = -1;
    };

    QList<hexFunction> *list = nullptr; // для функций от 4х переменных
    QList<hexFunction> *listNoUsedFunc = nullptr; // для функций от 4х переменных
    QList<hexFunction> *list_2 = nullptr; // для функций от 2х переменных
    QList<hexFunction> *list_2_NoUsedFunc = nullptr; // для функций от 2х переменных

    QList<hexFunction> filterFunctionsList; // создаем список для рандома, куда закинем все функции, которое подходят под условие

    QList<studentStruct> *students = nullptr;
    QStringList groups; // список групп
    mdnfMacKlassky *mdnf = nullptr;

//    FormLoadingVectors *ff = nullptr;
    bool generatedFunctions = false;

    bool starting = true; // флаг, что программа только запустилась

    loadFunctions loader; // создаём загрузчик функций
    QList<presets> presetsList; // список, хранящий пресеты

    QString fileExelOutputDir; // файл вывода для Exel


    void message(QString text, QString title = "Генератор", int type = 0); // выдать сообщение
    //    QString randomFunction(int varsCount = 0, int skleykiCount = 0, QString ignoreGroup = ""); // генерация рандомной функции
    //    int getNotUsedCount(int varsCount = 0, int skleykiCount = 0, QString ignoreGroup = ""); // получить количество неиспользованных функций
    //    int getMinUsedCount(int varsCount = 0, int skleykiCount = 0, QString ignoreGroup = ""); // получить минимальное количество повторений одной функции
    bool isFuncContainsInGroup(QString function, QString group); // возвращает ,содержится ли функция в этой группе
    void setFunction(QString group, QString studentName, QString function); // устанавливаем функцию студенту
    void saveData(); // сохранить данные в файл
    void saveVariantsToExel(); // сохраняет варианты в exel
    void setStartUIData(); // устанавливает стартовые значения для всех элементов интерфейса
    void setLineColor(QTableWidget *tableWidgetInput, int rowNum, QBrush brush); // устанавливает цвет строки в таблице

    int getVarsCountByUser(); // возвращает количество переменных, которое выбрал пользователь
    int getSkleikyCountByUser(int type); // возвращает количество склеек, которое выбрал пользователь
    //    int getTypeMfByUser(int type); // возвращает тип минимизации, который выбрал пользователь
    void getMintermsCountByUser(int type, int &from, int &to); // возвращает количество минтермов (скобочек) в конечном ответе, которое выбрал пользователь
    void getLogicalVarsCountByUser(int type, int &from, int &to); // возвращает количество логических переменных в конечном ответе, которое выбрал пользователь

    bool getFromToData(QLineEdit *lineEditFrom, QLineEdit *lineEdit_to, int &from_var, int &to_var); // записыввет значения из LineEdit и возвращает true, если все нормально
    //    void getRandomParametsByUser(int &vars, int &skleyki, int &typeMf, int &mintermsCountFrom, int &mintermsCountTo, int &logicalVarsCountFrom, int &logicalVarsCountTo);
    bool acceptFilter(); // функция получения фильтра из интерфейса
    void generateListFunctionBy(generateFilter filter); // генерирует список randomList из функций, подходящих под фильтр
    void generateFunctionsForStudents(QList<studentStruct> &studentsListParam, bool fullRandomFunctions,  bool noPovtorInGroup, bool &error);
    bool isCorrectSkleykiCount(int skleykiCountFunc, int skleykiCountFilter); // возвращает, корректное-ли значение количества склеек, относительно фильтра
    bool isCorrectMintermsCount(int mintermsCountFunc, int mintermsCountFilterFrom, int mintermsCountFilterTo); // возвращает, корректное-ли значение количества минтермов (макстермов), относительно фильтра
    bool isCorrectLogicalVarsCountCount(int logicalVarsCountFunc, int logicalVarsCountFilterFrom, int logicalVarsCountFilterTo); // возвращает, корректное-ли значение количества логических переменных, относительно фильтра
    bool isCorrectMknfFilter(hexFunction func, generateFilter filter); // возвращает, подходит-ли данная функция под фильтр МКНФ
    bool isCorrectMdnfFilter(hexFunction func, generateFilter filter); // возвращает, подходит-ли данная функция под фильтр МДНФ
    void useFunc(QString func_16, QList<hexFunction> *&listInput); // изымает из списка неиспользованных функцию
    bool isEqval(hexFunction function_struct1, hexFunction function_struct2);

    QString getDataFromLineFile(QString line, QString key, bool *ok = nullptr); // возвращает значение из строки по ключу
    QList<studentStruct> getStudentsByGroup(QString group); // возврашает список студентов по их группе
    studentStruct getStudentBy(QString fio, QString group, QString function); // возвращает студента по его ФИО, группе и функции
    studentStruct getStudentBy(QString fio, QString group); // возвращает студента по его ФИО и группе
    studentStruct getStudentBy(QString function); // возвращает студента по его функции

    void loadPresetsFromFile(QList<presets> &presetsList, QString fileURL); // загрузить пресеты из файла
    void addPresets(QList<presets> &presetsList, presets addingPreset); // добавить пресет
    void removePreset(QList<presets> &presetsList, presets removingPreset); // удалить пресет
    void clearPresets(QList<presets> &presetsList); // очистить список пресетов
    void savePresetsToFile(QList<presets> &presetsList, QString fileURL); // сохранить пресеты в файл

    presets getPresetByUI(); // создаёт пресет на основе текущих данных с интерфейса
    void setUIPreset(presets presetToUI); // устанавливает на форму выбранный пресет
    void updatePresets(); // обновляет список пресетов на форме (в comboBox)

    int getGenerateType(); // возвращает тип генерации
    inline bool getFullNoPovtor();
    inline bool getNoPovtorInGroup();

private slots:
    void on_checkBox_fullNoPovtor_stateChanged(int arg1);
    void on_pushButton_presetsAdd_clicked();
    void on_pushButton_presetsDelete_clicked();
    void on_pushButton_presetsApplyName_clicked();
    void on_comboBox_chosePrests_currentIndexChanged(int index);
    void on_pushButton_save_choseFileExel_clicked();
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
