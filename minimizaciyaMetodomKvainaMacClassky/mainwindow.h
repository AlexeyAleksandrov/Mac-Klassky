#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <QTableWidget>
#include "QMessageBox"
#include "QPushButton"
#include <QDebug>
#include <QLineEdit>
#include <QTabWidget>
#include <math.h>
#include "mdnfmacklassky.h"
#include <QFile>
#include <QTextStream>
#include "logiceditor.h"
#include <QTableWidgetItem>
//#include "formproverkaall.h"

#include "md5crypter.h"

#define STR_SEPARATOR_1 "\r\n"
#define STR_SEPARATOR_2 "\t"
#define FILE_STUDENT_WORK (QString)"mmkwork.mirea"

#define SKLEYKA_BACKGROUNF_COLOR QColor(255, 253, 208)
#define CHECK_BOXES_TEXT "Выбрать"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setFunction(QString function);

    bool to2ssch(QString number16, QString &number2); // перевод из 16 в 2ю
    bool to16ssch(QString number2, QString &number16); // перевод из 2 в 16ю

    void setTableStepenTwo(int stepen); // задаём таблице истинности количество строк и столбцов в зависимости от степени 2
    void setTableOfTrueNumbers(); // заполняем таблицу истинности числами от 0 до N в 2й ССЧ
    void criticalError(QString error = "Ошибка"); // отправляем QMessageBox с ошибикой
    void warningError(QString error = "Ошибка"); // выводим некритическую ошибку
    void message(QString message = "Правильно!", QString title = "Правильно");
    bool isAllElementsFirstQStrlingListContainsInSecondQStringList(QStringList firstList, QStringList secondList); // функция проверяет, содержатся ли все элементы первого спика во втором (может быть так. что первый список имеет 3 элемента, а второй 5, но если все 3 содержатся, то вернёт True)

    void setStudentName(const QString &value);
    void setStudentGroup(const QString &value);

    void loadDataFromFile(); // загружает данные из файла

    void setNoMessage(bool value); // устанавливает флаг блокировки вывода всплывающих сообщений

    void setAutoInput(bool value);

private:
    Ui::MainWindow *ui;
    // vars
    int typeMin = 0; // тип минимизации - 0 для МКНФ и 1 для МДНФ
    bool noMessage = false; // флаг отключения подсказок
    QString studentName = ""; // хранит ФИО студента
    QString StudentGroup = "";  // хранит группу студента
    QString function_16 = ""; // хранит исходную 16ю функцию
    QString function_2 = ""; // хранит 2ю запись главной функции
    QString itogMdnfTextLine = ""; // текст итоговой МДНФ
    QStringList skleyki[3]; // храним рельтат склеек
    bool allowSave = false;
    bool splitToTetrads = false;


    // checkBoxes
    QCheckBox **checkBoxes = nullptr; // галочки для переноса строк из таблицы истинности в таблицу с 1
    QCheckBox **checkBoxes_ones = nullptr; // галочки для создания склеек из таблицы с 1
    QCheckBox **checkBoxes_skleyki_1 = nullptr; // галочки для склеек
    QCheckBox **checkBoxes_skleyki_2 = nullptr; // галочки для склеек
//    QCheckBox **checkBoxes_skleyki_3 = nullptr; // галочки для склеек
    QCheckBox *checkBox_autoInputFormula = nullptr; // галочка автоматического ввода синтаксиса
    QCheckBox *checkBox_editChoseItemFormula = nullptr; // галоска выбора редактирования выбранного элемента

    // pushButtons
    QPushButton *pushButton_nextStep_tot = nullptr; // кнопка для перехода к слеюдущему действию от таблицы истинности
    QPushButton *pushButton_delete_last_ones = nullptr; // унопка удаления последнего элемента в таблиице, где
    QPushButton *pushButton_proverka_karta_minimizacii = nullptr; // кнопка проверки итоговой МДНФ
    QPushButton *pushButton_add_skleyki_2 = nullptr; // кнопка добавления склеек 2 (последние склейки)
    QPushButton *pushButton_proverka_karti_minimizacii = nullptr; // кнопка проверки карты минимизации
//    QPushButton *pushButton_func_2 = nullptr; // кнопка добавления функции записанной в 2м коде в таблицу истинности
    QPushButton *pushButton_checkTableOfTrue = nullptr; // кнопка првоерки таблицы истинности
    QPushButton *pushButton_tot_add = nullptr; // кнопка переноса значений, при которых функция равна 1
    QPushButton *pushButton_proverka_oneOnly = nullptr; // кнопка проверки значенйи при которых функция равна 1
    QPushButton *pushButton_tot_add_to_skeyki_1 = nullptr; // кнопкка добавления элементов для склейки 1
    QPushButton *pushButton_clear_skleiki_1_editing = nullptr; // кнопка удаления последней добавленной склейки
    QPushButton *pushButton_proverka_skleiki_1_editing = nullptr; // кнопка проверки 1й склейки
    QPushButton *pushButton_nextStep_onesOnly = nullptr; // кнопка перехода к склейке 2
    QPushButton *pushButton_add_skleyki_1 = nullptr; // кнопка добавления элементов для склейки 2
    QPushButton *pushButton_clear_skleiki_2_editing = nullptr; // кнопка удаления последней строки в склейке 2
    QPushButton *pushButton_proverka_skleyki_3 = nullptr; // кнопка проверки склеек 2 (тут ошибка в номере, но не буду переделывать)
    QPushButton *pushButton_nextStep_skleyki_2 = nullptr; // кнопка перехода к склейке 3
    QPushButton *pushButton_clearSkleyki_2 = nullptr; // кнопка удаления последне строки в склейке 3
    QPushButton *pushButton_proverka_skleyki_2 = nullptr; // кнопка проверки склеек 3
    QPushButton *pushButton_nextStep_onesOnly_3 = nullptr; // кнопка перехода к карте минимизации
    QPushButton *pushButton_INV = nullptr; // кнопка инверсии
    QPushButton *pushButton_A = nullptr; // кнопка добавления переменной A
    QPushButton *pushButton_B = nullptr; // кнопка добавления переменной B
    QPushButton *pushButton_C = nullptr; // кнопка добавления переменной C
    QPushButton *pushButton_D = nullptr; // кнопка добавления переменной D
    QPushButton *pushButton_AND = nullptr;
    QPushButton *pushButton__OR = nullptr;
    QPushButton *pushButton_SKOBKA_BEFORE = nullptr;
    QPushButton *pushButton_SKOBKA_AFTER = nullptr;
    QPushButton *pushButton_DEL = nullptr;
    QPushButton *pushButton_CLEAR = nullptr;
    QPushButton *pushButton_tot_move_to_skeyki_1 = nullptr;
    QPushButton *pushButton_tot_move_to_skeyki_2 = nullptr;
    QPushButton *pushButton_tot_move_to_skeyki_3 = nullptr;
    QPushButton *pushButton_setRedColor = nullptr;
    QPushButton *pushButton_setGreenColor = nullptr;
    QPushButton *pushButton_setWhiteColor = nullptr;
    QPushButton *pushButton_notA = nullptr;
    QPushButton *pushButton_notB = nullptr;
    QPushButton *pushButton_notC = nullptr;
    QPushButton *pushButton_notD = nullptr;
    QPushButton *pushButton_loadDataFromFile = nullptr;


    // tableWidgets
    QTableWidget **tableWidgetsSkleyki = nullptr; // таблицы со склейками
    QTableWidget **tableWidgetsSkleykiEditing = nullptr; // таблицы со склейками
    QTableWidget *tableWidgetTot = nullptr; // таблиц истинности
    QTableWidget *tableWidgetOnesOnly = nullptr; // таблица со значениями, при которых функция принимает значение 1
    QTableWidget *tableWidgetOnesOnlyEditing = nullptr; // та же таблица, но около таблицы истинности
    QTableWidget *tableWidgetKartaMinimizacii = nullptr; // таблица с картой минимазации
    QTableWidget *tableWidget_formulaEditor = nullptr; // таблица редактора формул
    QTableWidget *tableWidget_kartaMinimizacii = nullptr;

    // lineEdits
    QLineEdit *lineEditFunction = nullptr;
    QLineEdit *lineEditFunc_2 = nullptr;
//    QLineEdit *lineEdit_itogMdnf = nullptr;

    // tabWidget
    QTabWidget *tabWidget = nullptr;

    mdnfMacKlassky *mdnf = nullptr; // создаем экзампляр МДНФ
    logicEditor *editor = nullptr;

    QColor *redColor = nullptr;
    QColor *greenColor = nullptr;

    // работа с таблицей истинности
    int getType(QString func_2, QStringList listFunc); // возвращает определнный тип минимизации 0 - МКНФ, 1 - МДНФ, -1 - не удалось определить
    int getOptimizeType(QString func_2); // возвращает, какой лучше тип (МКНФ/МДНФ) выбрать

    // работа со склейками
//    void makeSkleyki(QStringList numbersList, QStringList &skleykiList); // функция, которая производит склейки
    void createSkleyka(QTableWidget *tableWidgetInput, QCheckBox **checkBoxesInput, QTableWidget *&tableWidgetOutput, int numSkleyka, bool ignoreCellWidgetInLastColumn = true); // создать склейку на основе таблицыи и галочек и выводим в другую таблицу результат, numSkleika - это номер этапа склейки
    bool isContainsSkleyki(QStringList skleykiList, int numSkleyka); // проверяет, все-ли склейки из списка содержатся в правильном варианте (true - все нормально, false - хотя бы одна не содержится)
    void moveSkleyka(QTableWidget *tableWidgetInput, QCheckBox **checkBoxesInput, QTableWidget *&tableWidgetOutput, int numSkleyka, bool ignoreCellWidgetInLastColumn = true); // перемещение выделенных элементов без склейки
    bool proverkaTable(QTableWidget *tableWidgetInput, QStringList listOfSkeyki, bool ignoreRedColor = false, bool ignoreCellWidgetInLastColumn = true); // проверка склеек, ignoreRedColor - игнорировать строки с красным цветом (для повторящихся склеек)
    void goToNextStep(QTableWidget *tableWidgetInput, QTableWidget *&tableWidgetOutput, int nextTabIndex,bool ignoreCellWidgetInLastColumn = true);
    void sortSkleiki(QTableWidget *skleikiTableWidget, bool ignoreCellWidgetInLastColumn = true); // сортировка склеек по количеству в них Х
    void sortOnesCount(QTableWidget *tableWidgetInput, bool ignoreCellWidgetInLastColumn = true); // сортировка значений по количеству в них 1


    // перемещение по этапам работы
    void setStep(int step); // устанавливает текущий этап, соответственно блокируя или разблокируя определенные поля
    int step = 0; // переменная, хранящая текущий этап
    void nextStep(); // процедура, ведличиваящая шаг на 1 и переводящая интерфейс к следующему шагу

    // всё, что связано с TableWidget
    void setTWText(QTableWidget *&tableWidget, int row, int col, QString text, QColor itemColor = Qt::white); // устанавливает текст в tableWidget
    void setQStringListToTW(QTableWidget *&tableWidget, QStringList list, bool containsColor = false); // добавить QStringList по символам в таблицу
    void addQStringToTWOneSymwolInItem(QTableWidget *&tableWidget, QString str); // добавить строку в таблицу по символам
    void addQStringListToTWOneSymwolInItem(QTableWidget *&tableWidget, QStringList list); // добавить QStringList по символам в таблицу
    void clearTW(QTableWidget *&tableWidget); // очищает tableWidget
    bool getTWItemText(QTableWidget *&tableWidget, int row, int col, QString &outputString); // получает текст из конкретной ячейки таблицы
    bool getTWTextList(QTableWidget *&tableWidget, QStringList &outputList, bool ignoreCellWidgetInLastColumn = true); // значение строк таблицы как QStringList
    void addRow(QTableWidget *&tableWidget, QStringList rowList, int colorRed = -1, int colorGreen = -1, int colorBlue = -1); // создает в таблице строку и выводит в неё элементы списка. Дополнительно можно передать цвет для закраски строки
    void copyTableWidget(QTableWidget *tableWidgetInput, QTableWidget *&tableWidgetOutput, bool skipIdenticalLines, bool ignoreCellWidgetInLastColumn); //  skipIdenticalLines - Пропуск одинаковых стролк
    void deletelastRow(QTableWidget *&tableWidget); // удаляет последнюю строчку в таблице
    QString getQStringByTableWidget(QTableWidget *tableWidget, bool saveLineColor = false, bool ignoreCellWidgetInLastColumn = true); // получить данные из таблицы в виде строки
    void lockFormulaEditor(bool lock); // заблокировать / разблокировать редактор формул
    void center_text_in_table(QTableWidget *tbw); // центрировать текст в ячейках таблицы
    void setVariablesToHeader(QTableWidget *tbw); // функция, устаналивающая в качестве заголовков таблицы переменные (a, b, c, d)
    void setCellCheckBox(QTableWidget *tableWidget, QCheckBox *checkBox, int row, int col); // устанавливает checkbox
    void setCheckBoxes(QTableWidget *tableWidget, QCheckBox **checkBox);

    // сохранение данных в файл
    void saveDataToFile(); // функция сохранения данных в файл
    void readDataFromFile(); // функция чтения данных из файла

    // работа с редактором формул
    void addSymvol(QString symvol, bool inversiya = false); // добавление символа
    bool autoInput = false; // автоматический ввод синтаксиса формулы


private slots:
    // таблица истинности
    void pushButtonFunctionClicked(); // кнопка выбора функции
    void pushButtonTotAddClicked(); // кнопка добавляения строк из таблицы истинности в таблицу, где только 1
    void pushButtonCheckTableOfTrueClicked(); // кнопка проверяющая правильность таблицы истинности
    void pushButtonFunc2Clicked(); // кнопка ввода числа в 2 ССЧ

    // таблица, где функция принимает 0 или 1
    void pushButtonClearOneOnlyClicked(); // отчищает таблицу, где все 1
    void pushButtonProverkaClicked(); // кнопка проверки таблиц с 1
    void pushButtonAddTSkeyki_1(); // кнопка делающая склейки из таблицы с 1
    void pushButton_delete_last_onesClicked(); // кнопка, удаляющая посдлеюнюю строку
    void pushButton_nextStep_totClicked(); // кнопка для переноса данных с ледующую таблицу и перехода к седующему действию

    // таблица с 1й склейкой
    void pushButton_add_skleyki_2_clicked(); // кнопка добавления склейки из склеек 1
    void pushButton_clear_skleyki_1(); // кнопка отчистки таблицы со склейками 1
    void pushButton_proverka_skleyki_1_clicked(); // кнопка проверки склейки 1
    void pushButton_delete_last_skleiki_1_editingClicked(); // кнопка удаления последней добавленной склейки
    void pushButton_nextStep_tot_2Clicked(); // кнопка перехода к следующей склейке

    // таблица с 2й склейкой
    void pushButton_add_skleyki_3_clicked(); // кнопка добавления склейки из склеек 1
    void pushButton_clear_skleyki_2(); // кнопка отчистки таблицы со склейками 1
    void pushButton_proverka_skleyki_2_clicked(); // кнопка проверки склейки 1
    void pushButton_delete_last_skleiki_2_editingClicked(); // кнопка удаления последней добавленной склейки
    void pushButton_nextStep_skleyki_2Clicked(); // кнопка перехода к склейке 3

    // таблица с 3й склейкой
    void pushButton_clear_skleyki_3(); // кнопка отчистки таблицы со склейками 1
    void pushButton_proverka_skleyki_3_clicked(); // кнопка проверки склейки 1
    void pushButton_delete_last_skleiki_3_editingClicked(); // кнопка удаления последней добавленной склейки
    // карта покрытия
    void setKartaColor(QColor color); // задаёт цвет выделенным ячейкам карты покрытия
    void pusbButton_move_skleyki_to_coresTable(); // кнопка переноса склеек в таблицу для нахождения ядер
    void on_pushButton_setRedColor_clicked(); // кнопка для выделения красным ячеек, для упрощения построения минимальной функции
    void on_pushButton_setGreenColor_clicked(); // то же самое, толлько зеленым не выделяются крысные ячейки
    void on_pushButton_setWhiteColor_clicked(); // белый нужен для сброса, если значение выделено по ошибке
//    void pushButton_proverka_karta_minimizaciiClicked(); // кнопка проверки итоговой функции
    void on_pushButton_proverka_karta_minimizacii_clicked();
    void on_pushButton_proverka_karti_minimizacii_clicked();

    void on_pushButton_clear_skleiki_1_editing_clicked();
    void on_pushButton_clear_skleiki_2_editing_clicked();
    void on_pushButton_clearSkleyki_2_clicked();
//    void on_pushButton_proverka_skleyki_2_clicked();
    void on_pushButton_proverka_skleyki_3_0_clicked();
//    void on_pushButton_loadDataFromFile_clicked();
//    void on_pushButton_nextStep_skleyki_2_clicked();
    void on_pushButton_nextStep_onesOnly_3_clicked();

    void on_pushButton_INV_clicked();
    void on_pushButton_A_clicked();
    void on_pushButton_B_clicked();
    void on_pushButton_C_clicked();
    void on_pushButton_D_clicked();
    void on_pushButton_AND_clicked();
    void on_pushButton__OR_clicked();
    void on_pushButton_SKOBKA_BEFORE_clicked();
    void on_pushButton_SKOBKA_AFTER_clicked();
    void on_pushButton_DEL_clicked();
    void on_pushButton_CLEAR_clicked();
//    void on_tableWidget_formulaEditor_itemActivated(QTableWidgetItem *item);
//    void on_tableWidget_formulaEditor_itemChanged(QTableWidgetItem *item);

    void on_pushButton_tot_move_to_skeyki_1_clicked();
    void on_pushButton_tot_move_to_skeyki_2_clicked();
    void on_pushButton_tot_move_to_skeyki_3_clicked();


//    void on_pushButton_proverkaAll_clicked();
    void on_pushButton_notA_clicked();
    void on_pushButton_notB_clicked();
    void on_pushButton_notC_clicked();
    void on_pushButton_notD_clicked();
    void on_checkBox_autoInputFormula_stateChanged(int arg1);
    void on_checkBox_editChoseItemFormula_stateChanged(int arg1);

    void on_lineEdit_func_2_textChanged(const QString &arg1);

    void on_checkBox_spiltToTetrads_stateChanged(int arg1);

    void on_pushButton_checkTableOfTrue_clicked();

    void on_temp_pushButton_clicked();

private: // функции проверки
    bool proverkaTableOfTrue(); // функция проверки таблицы истинности
    bool proverkaOnesOnly(); // фунция проверки таблицы, где F(abcd) = 1
    bool proverkaSkleyki1(); // функция проверки 1й склейки
    bool proverkaSkleyki2(); // функция проверки 2й склейки
    bool proverkaSkleyki3(); // функция проверки 3й склейки
    bool proverkaKartaMinimizacii(); //функция проверки карты минимизации
    bool proverkaItogMdnf(); // функция проверки итоговой функции МДНФ

private:
    struct skleyka
    {
        QStringList text;
        int type;
        int size;
    };

//    formProverkaAll *fpl = nullptr;
};

#endif // MAINWINDOW_H
