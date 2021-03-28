#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QPushButton"

// срочно:
// ПРОВЕРИТЬ СОХРАНЕНИЕ!!!!!
// добавить фичу - автоматическое выделение ядер на карте
// доработать упрощенный ввод:
// - автоподстановка должна работать не только после буквы, но и после скобки и математического оператора
// - при редактировани конкретного элемента должен учитываться текущий выбранный
// деление на тетрады только по кнопке

// не критичное:
// сделать подтверждение выполнения задания, если найден файл сохранения
// сделать подтверждение на кнопку очистить формулу

// в конце
// проверить все всплывающие подсказки (а лучше вынести их текст в отдельный *.cpp файл)
// для каждой подсказки сделать номер, чтобы было проще редактировать
// сделать флаг показа подсказки, чтобы она больше не всплывала
// разделить автоматическое и ручное сохранение
// при открытии решения сообщать, что ранее сохраненное решение будет перезаписано
// создать окно настроек (диалог сохранить/отмена)

// проверить функцию D277, и вообще, есть-ли функции с 3 склейками и с наличием таких склеек, которые повторяются
// 2F35

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ставим запрет на изменение размера
//    auto sizeui = ui->centralWidget->geometry();
    this->setFixedWidth(geometry().width());
    this->setFixedHeight(geometry().height());
    this->setMaximumWidth(geometry().width());
    this->setMaximumHeight(geometry().height());

    editor = new logicEditor(ui->tableWidget_formulaEditor);
//    editor->setEditChoseItem(true);
//    editor->setAutoInput(false); // включаем автоматический ввод выражения
    setAutoInput(false); // включаем автоматический ввод выражения


    qDebug() << "задаем Nullptr";
    pushButton_nextStep_tot = nullptr;
    lineEditFunc_2 = nullptr;
    lineEditFunction = nullptr;
    tableWidgetTot = nullptr;
    tableWidgetOnesOnly = nullptr;
    tableWidgetsSkleyki = nullptr;
    tableWidgetOnesOnlyEditing = nullptr;
    tableWidgetKartaMinimizacii = nullptr;
    tabWidget = nullptr;


    qDebug() << "Прираниваем значения";
    pushButton_nextStep_tot = ui->pushButton_nextStep_tot;
//    pushButton_delete_last_ones = ui->pushButton_delete_last_ones;
    pushButton_proverka_karta_minimizacii = ui->pushButton_proverka_karta_minimizacii;
    pushButton_add_skleyki_2 = ui->pushButton_add_skleyki_2;
    pushButton_proverka_karti_minimizacii = ui->pushButton_proverka_karti_minimizacii;

    tableWidgetTot = ui->tableWidget_tableOfTrue;
    tableWidgetOnesOnly = ui->tableWidget_one_only;
    tableWidgetOnesOnlyEditing = ui->tableWidget_one_only_editing;
    tableWidgetKartaMinimizacii = ui->tableWidget_kartaMinimizacii;

    lineEditFunction = ui->lineEdit_function;
    lineEditFunc_2 = ui->lineEdit_func_2;
//    lineEdit_itogMdnf = ui->lineEdit_itogMdnf;

    tabWidget = ui->tabWidget;

    qDebug() << "Conntects tot";
    // таблица истинности
    connect(ui->pushButton_enterFunction, SIGNAL(clicked()), this, SLOT(pushButtonFunctionClicked())); // соединяем кнопку со слотом
    qDebug() << "pushButton_enterFunction";
    connect(ui->pushButton_tot_add, SIGNAL(clicked()), this, SLOT(pushButtonTotAddClicked()));
    connect(ui->pushButton_checkTableOfTrue, SIGNAL(clicked()), this, SLOT(pushButtonCheckTableOfTrueClicked()));
//    connect(ui->pushButton_func_2, SIGNAL(clicked()), this, SLOT(pushButtonFunc2Clicked()));
    qDebug() << "pushButton_func_2";
    connect(pushButton_nextStep_tot, SIGNAL(clicked()), this, SLOT(pushButton_nextStep_totClicked()));
//    connect(pushButton_delete_last_ones, SIGNAL(clicked()), this, SLOT(pushButton_delete_last_onesClicked()));
    qDebug() << "Conntects f = 1";
    // таблица со значениями, где функция принимает значение 1
//    connect(ui->pushButton_clearTableOneOnly, SIGNAL(clicked()), this, SLOT(pushButtonClearOneOnlyClicked()));
    connect(ui->pushButton_proverka_oneOnly, SIGNAL(clicked()), this, SLOT(pushButtonProverkaClicked()));
    connect(ui->pushButton_tot_add_to_skeyki_1, SIGNAL(clicked()), this, SLOT(pushButtonAddTSkeyki_1()));
    // таблица со склейками 1
    qDebug() << "Conntects skleiki 1";
    connect(ui->pushButton_add_skleyki_1, SIGNAL(clicked()), this, SLOT(pushButton_add_skleyki_2_clicked()));
//    connect(ui->pushButton_clear_skleiki_1_editing, SIGNAL(clicked()), this, SLOT(pushButton_clear_skleyki_1()));
    connect(ui->pushButton_proverka_skleiki_1_editing, SIGNAL(clicked()), this, SLOT(pushButton_proverka_skleyki_1_clicked()));
//    connect(ui->pushButton_delete_last_skleiki_1_editing, SIGNAL(clicked()), this, SLOT(pushButton_delete_last_skleiki_1_editingClicked()));
    connect(ui->pushButton_nextStep_onesOnly, SIGNAL(clicked()), this, SLOT(pushButton_nextStep_tot_2Clicked()));

    // таблица со склейками 2
    qDebug() << "Conntects skleiki 2";
//    connect(ui->pushButton_clearSkleyki_2, SIGNAL(clicked()), this, SLOT(pushButton_clear_skleyki_2()));
    connect(ui->pushButton_proverka_skleyki_3, SIGNAL(clicked()), this, SLOT(pushButton_proverka_skleyki_2_clicked()));
    connect(ui->pushButton_nextStep_skleyki_2, SIGNAL(clicked()), this, SLOT(pushButton_nextStep_skleyki_2Clicked()));
    connect(pushButton_add_skleyki_2, SIGNAL(clicked()), this, SLOT(pushButton_add_skleyki_3_clicked()));

    // таблица для поиска ДНФ
    qDebug() << "Conntects poisk dnf";
//    connect(ui->pushButton_move_skleyki_to_coresTable, SIGNAL(clicked()), this, SLOT(pusbButton_move_skleyki_to_coresTable()));
//    connect(ui->pushButton_move_skleyki_to_coresTable_2, SIGNAL(clicked()), this, SLOT(pusbButton_move_skleyki_to_coresTable()));
//    connect(ui->pushButton_move_skleyki_to_coresTable_3, SIGNAL(clicked()), this, SLOT(pusbButton_move_skleyki_to_coresTable()));
//    connect(pushButton_proverka_karti_minimizacii, SIGNAL(clicked()), this, SLOT(pushButton_proverka_karta_minimizaciiClicked));

//    checkBoxes;
    // таблица истинности
    qDebug() << "checkboxes";
    const int size = 16; // константа размера массива
    checkBoxes = new QCheckBox *[size];
    for (int i=0; i<size; i++)
    {
        checkBoxes[i] = nullptr;
    }
    checkBoxes[0] = ui->checkBox_tot_0;
    checkBoxes[0] = ui->checkBox_tot_0; // заносим в массив все чекбоксы
    checkBoxes[1] = ui->checkBox_tot_1;
    checkBoxes[2] = ui->checkBox_tot_2;
    checkBoxes[3] = ui->checkBox_tot_3;
    checkBoxes[4] = ui->checkBox_tot_4;
    checkBoxes[5] = ui->checkBox_tot_5;
    checkBoxes[6] = ui->checkBox_tot_6;
    checkBoxes[7] = ui->checkBox_tot_7;
    checkBoxes[8] = ui->checkBox_tot_8;
    checkBoxes[9] = ui->checkBox_tot_9;
    checkBoxes[10] = ui->checkBox_tot_10;
    checkBoxes[11] = ui->checkBox_tot_11;
    checkBoxes[12] = ui->checkBox_tot_12;
    checkBoxes[13] = ui->checkBox_tot_13;
    checkBoxes[14] = ui->checkBox_tot_14;
    checkBoxes[15] = ui->checkBox_tot_15;
//    checkBoxes = new QCheckBox *[size];

    // таблица, где функция равна 1
//    checkBoxes_ones = new QCheckBox *[16];
    checkBoxes_ones = new QCheckBox *[size];
    for (int i=0; i<size; i++)
    {
        checkBoxes_ones[i] = nullptr;
    }
    checkBoxes_ones[0] = ui->checkBox_ones_0;
    checkBoxes_ones[1] = ui->checkBox_ones_1;
    checkBoxes_ones[2] = ui->checkBox_ones_2;
    checkBoxes_ones[3] = ui->checkBox_ones_3;
    checkBoxes_ones[4] = ui->checkBox_ones_4;
    checkBoxes_ones[5] = ui->checkBox_ones_5;
    checkBoxes_ones[6] = ui->checkBox_ones_6;
    checkBoxes_ones[7] = ui->checkBox_ones_7;
    checkBoxes_ones[8] = ui->checkBox_ones_8;
    checkBoxes_ones[9] = ui->checkBox_ones_9;
    checkBoxes_ones[10] = ui->checkBox_ones_10;
    checkBoxes_ones[11] = ui->checkBox_ones_11;
    checkBoxes_ones[12] = ui->checkBox_ones_12;
    checkBoxes_ones[13] = ui->checkBox_ones_13;
    checkBoxes_ones[14] = ui->checkBox_ones_14;
    checkBoxes_ones[15] = ui->checkBox_ones_15;


    // склейки 1
    checkBoxes_skleyki_1 = new QCheckBox *[size]; // выделяем память под указатели на QCheckBox для склейки 1
    for (int i=0; i<size; i++)
    {
        checkBoxes_skleyki_1[i] = nullptr;
    }
    qDebug() << "Заполняем массив";
    checkBoxes_skleyki_1[0] = ui->checkBox_skleyka_1_0;
    checkBoxes_skleyki_1[1] = ui->checkBox_skleyka_1_1;
    checkBoxes_skleyki_1[2] = ui->checkBox_skleyka_1_2;
    checkBoxes_skleyki_1[3] = ui->checkBox_skleyka_1_3;
    checkBoxes_skleyki_1[4] = ui->checkBox_skleyka_1_4;
    checkBoxes_skleyki_1[5] = ui->checkBox_skleyka_1_5;
    checkBoxes_skleyki_1[6] = ui->checkBox_skleyka_1_6;
    checkBoxes_skleyki_1[7] = ui->checkBox_skleyka_1_7;
    checkBoxes_skleyki_1[8] = ui->checkBox_skleyka_1_8;
    checkBoxes_skleyki_1[9] = ui->checkBox_skleyka_1_9;
    checkBoxes_skleyki_1[10] = ui->checkBox_skleyka_1_10;
    checkBoxes_skleyki_1[11] = ui->checkBox_skleyka_1_11;
    checkBoxes_skleyki_1[12] = ui->checkBox_skleyka_1_12;
    checkBoxes_skleyki_1[13] = ui->checkBox_skleyka_1_13;
    checkBoxes_skleyki_1[14] = ui->checkBox_skleyka_1_14;
    checkBoxes_skleyki_1[15] = ui->checkBox_skleyka_1_15;
    qDebug() << "Заполнили массив";

    checkBoxes_skleyki_2 = new QCheckBox *[size];
    for (int i=0; i<size; i++)
    {
        checkBoxes_skleyki_2[i] = nullptr;
    }
    checkBoxes_skleyki_2[0] = ui->checkBox_skleyka_2_0;
    checkBoxes_skleyki_2[1] = ui->checkBox_skleyka_2_1;
    checkBoxes_skleyki_2[2] = ui->checkBox_skleyka_2_2;
    checkBoxes_skleyki_2[3] = ui->checkBox_skleyka_2_3;
    checkBoxes_skleyki_2[4] = ui->checkBox_skleyka_2_4;
    checkBoxes_skleyki_2[5] = ui->checkBox_skleyka_2_5;
    checkBoxes_skleyki_2[6] = ui->checkBox_skleyka_2_6;
    checkBoxes_skleyki_2[7] = ui->checkBox_skleyka_2_7;
    checkBoxes_skleyki_2[8] = ui->checkBox_skleyka_2_8;
    checkBoxes_skleyki_2[9] = ui->checkBox_skleyka_2_9;
    checkBoxes_skleyki_2[10] = ui->checkBox_skleyka_2_10;
    checkBoxes_skleyki_2[11] = ui->checkBox_skleyka_2_11;
    checkBoxes_skleyki_2[12] = ui->checkBox_skleyka_2_12;
    checkBoxes_skleyki_2[13] = ui->checkBox_skleyka_2_13;
    checkBoxes_skleyki_2[14] = ui->checkBox_skleyka_2_14;
    checkBoxes_skleyki_2[15] = ui->checkBox_skleyka_2_15;
//    checkBoxes_skleyki_2 = new QCheckBox *[size];

    for (int i=0; i<size; i++)
    {
        if(checkBoxes[i] != nullptr)
            checkBoxes[i]->setText(CHECK_BOXES_TEXT);
        if(checkBoxes_ones[i] != nullptr)
            checkBoxes_ones[i]->setText(CHECK_BOXES_TEXT);
        if(checkBoxes_skleyki_1[i] != nullptr)
            checkBoxes_skleyki_1[i]->setText(CHECK_BOXES_TEXT);
        if(checkBoxes_skleyki_2[i] != nullptr)
            checkBoxes_skleyki_2[i]->setText(CHECK_BOXES_TEXT);
    }

    checkBox_autoInputFormula = ui->checkBox_autoInputFormula;
    checkBox_editChoseItemFormula = ui->checkBox_editChoseItemFormula;

    tableWidgetsSkleyki = new QTableWidget *[2]; // выделяем память под 3 таблицы
    tableWidgetsSkleyki[0] = ui->tableWidget_skleiki_1;
    tableWidgetsSkleyki[1] = ui->tableWidget_skleiki_2;
//    tableWidgetsSkleyki[2] = ui->tableWidget_skleiki_3;

    tableWidgetsSkleykiEditing = new QTableWidget *[3];
    tableWidgetsSkleykiEditing[0] = ui->tableWidget_skleiki_1_editing;
    tableWidgetsSkleykiEditing[1] = ui->tableWidget_skleiki_2_editing;
    tableWidgetsSkleykiEditing[2] = ui->tableWidget_skleiki_3_editing;

    tableWidget_formulaEditor = ui->tableWidget_formulaEditor;
    tableWidget_kartaMinimizacii = ui->tableWidget_kartaMinimizacii;



    qDebug() << "Задаем видимость";

    qDebug() << "МДНФ";
    mdnf = nullptr;
    qDebug() << "Обнуляем MDNF";
    tabWidget->setCurrentIndex(0);

//    pushButton_func_2 = ui->pushButton_func_2;
    pushButton_checkTableOfTrue = ui->pushButton_checkTableOfTrue;
    pushButton_tot_add = ui->pushButton_tot_add;
    pushButton_proverka_oneOnly = ui->pushButton_proverka_oneOnly;
    pushButton_tot_add_to_skeyki_1 = ui->pushButton_tot_add_to_skeyki_1;
    pushButton_clear_skleiki_1_editing = ui->pushButton_clear_skleiki_1_editing;
    pushButton_proverka_skleiki_1_editing = ui->pushButton_proverka_skleiki_1_editing;
    pushButton_nextStep_onesOnly = ui->pushButton_nextStep_onesOnly;
    pushButton_add_skleyki_1 = ui->pushButton_add_skleyki_1;
    pushButton_clear_skleiki_2_editing = ui->pushButton_clear_skleiki_2_editing;
    pushButton_proverka_skleyki_3 = ui->pushButton_proverka_skleyki_3;
    pushButton_nextStep_skleyki_2 = ui->pushButton_nextStep_skleyki_2;
    pushButton_add_skleyki_2 = ui->pushButton_add_skleyki_2;
    pushButton_clearSkleyki_2 = ui->pushButton_clearSkleyki_2;
    pushButton_proverka_skleyki_2 = ui->pushButton_proverka_skleyki_3_0;
    pushButton_nextStep_onesOnly_3 = ui->pushButton_nextStep_onesOnly_3;
    pushButton_tot_move_to_skeyki_1 = ui->pushButton_tot_move_to_skeyki_1;
    pushButton_tot_move_to_skeyki_2 = ui->pushButton_tot_move_to_skeyki_2;
    pushButton_tot_move_to_skeyki_3 = ui->pushButton_tot_move_to_skeyki_3;
    pushButton_setRedColor = ui->pushButton_setRedColor;
    pushButton_setGreenColor = ui->pushButton_setGreenColor;
    pushButton_setWhiteColor = ui->pushButton_setWhiteColor;
//    pushButton_loadDataFromFile = ui->pushButton_loadDataFromFile;

    // редактор формул
    pushButton_INV = ui->pushButton_INV;
    pushButton_INV->setVisible(false); // выключаем кнопку показа инверсии, т.к. она была заменена на символы с инверсией
    pushButton_A = ui->pushButton_A;
    pushButton_B = ui->pushButton_B;
    pushButton_C = ui->pushButton_C;
    pushButton_D = ui->pushButton_D;
    pushButton_AND = ui->pushButton_AND;
    pushButton__OR = ui->pushButton__OR;
    pushButton_SKOBKA_BEFORE = ui->pushButton_SKOBKA_BEFORE;
    pushButton_SKOBKA_AFTER = ui->pushButton_SKOBKA_AFTER;
    pushButton_DEL = ui->pushButton_DEL;
    pushButton_CLEAR = ui->pushButton_CLEAR;
    pushButton_notA = ui->pushButton_notA;
    pushButton_notB = ui->pushButton_notB;
    pushButton_notC = ui->pushButton_notC;
    pushButton_notD = ui->pushButton_notD;




    // блокируем все кнопки до выбора варианта
    // этап 0 (ввод функции в векторном формате)

    setStep(step); // задаём нулевой этап, т.е. ничего не выбрано
//    ui->pushButton_

    ui->pushButton_enterFunction->setVisible(false); // отключаем отображение кнопки ,в связи с ненадобностью
    lineEditFunction->setReadOnly(true); // блокируем ввод в поле функции

    // для карты покрытия
    redColor = new QColor(255,101,96); // красный цвет
    greenColor = new QColor(185,255,141); // зеленый цвет

    pushButton_clear_skleiki_1_editing->setVisible(false); // выключаем видимость кнопки удаления склеек 1
    pushButton_clear_skleiki_2_editing->setVisible(false); // выключаем видимость кнопки удаления склеек 2
    pushButton_clearSkleyki_2->setVisible(false); // выключаем видимость кнопки удаления склеек 3

//    pushButton_loadDataFromFile->setVisible(false); // выключаем видимость кнопки загрузки решения из уже начатой работы

}

MainWindow::~MainWindow()
{
    if(allowSave) // если разрешено сохранение
    {
        saveDataToFile(); // сохраняем данные
    }
    delete ui;
}

void MainWindow::setFunction(QString function)
{
//    QString text = lineEditFunction->text(); // берем текст функции
    if(function == "") // проверяем на пустоту
    {
//        QMessageBox::critical(this, "Ошибка", "Поле ввода функции пустое!");
        criticalError("Поле ввода функции пустое!");
        return;
    }
    QString ch2;
    if(!to2ssch(function, ch2)) // проверяем перевод в 2ю ССЧ
    {
        QMessageBox::critical(this, "Ошибка", "Введена не 16-я функция!\nПроверьте правильность ввода!");
        return;
    }
    function_16 = function; // сохраняем функцию
    lineEditFunction->setText(function); // выводим функцию
    int razryad = function.count(); // поличество разрядов в исходном числе
    qDebug() << ch2; // выводим результат перевода
    qDebug() << "=======";
    setTableStepenTwo(razryad);
    qDebug() << "Количество строк " <<  tableWidgetTot->rowCount();
    qDebug() << "Количество столбцов " <<  tableWidgetTot->columnCount();

    setTableOfTrueNumbers(); // создаем таблицу истиности (заполняем числами от 0 до N в 2й ССЧ)
    // заносим в таблицу нашу функцию
    QStringList funcList = ch2.split(""); // разбиваем число в 2й ССЧ на символы
    if(mdnf != nullptr)
    {
        delete mdnf;
        mdnf = nullptr;
    }
    qDebug() << "Проверили mdnf";
    mdnf = new mdnfMacKlassky; // создаем МДНФ
    if(!mdnf->setFunction(function)) // если не удаётся рассчитать
    {
        QMessageBox::critical(this, "Program ERROR", "Неожиданная ошибка при вычислении МНФ!");
        qDebug() << "Ошибка";
        return;
    }
    // =================================================================
    qDebug() << "Отчищаем талицы";
    clearTW(tableWidgetOnesOnly);
    qDebug() << "Таблица с 1 очищена";
    clearTW(tableWidgetOnesOnlyEditing);
    qDebug() << "Таблица с 1 для редактирования очищена";
    for (int i=0; i<2; i++)
    {
        clearTW(tableWidgetsSkleyki[i]);
        qDebug() << "Таблица со склейками " << i << "очищена";
        clearTW(tableWidgetsSkleykiEditing[i]);
        qDebug() << "Таблица со склейками для редактирования" << i << "очищена";
    }
    clearTW(tableWidgetsSkleykiEditing[2]);
    clearTW(tableWidgetKartaMinimizacii);
    qDebug() << "Таблица карты минимизации очищена";
    qDebug() << "Талицы отчищены";
    step = 0;
    setStep(step);// переходим к следующему шагу
    nextStep();
    nextStep(); // увеличваем шаг на 2, из-за ошибки
}

void MainWindow::criticalError(QString error)
{
    if(noMessage) // если включена блокировка на отправлку сообщений
    {
        return;
    }
    QMessageBox::critical(this, "Ошибка", error);
}

void MainWindow::warningError(QString error)
{
    if(noMessage) // если включена блокировка на отправлку сообщений
    {
        return;
    }
    QMessageBox::warning(this, "Ошибка", error);
}

void MainWindow::message(QString message, QString title)
{
    if(noMessage) // если включена блокировка на отправлку сообщений
    {
        return;
    }
    QMessageBox::information(this, title, message);
}

bool MainWindow::isAllElementsFirstQStrlingListContainsInSecondQStringList(QStringList firstList, QStringList secondList)
{
    bool ok = true; // флаг проверки наличия данной склейки в рассчитанном списке
    for (int i=0; i<firstList.size(); i++)  // сравниваем каждый элемент первого списка
    {
        bool contained = false;
        for (int j=0; j<secondList.size(); j++)  // с каждым элементом второго списка
        {
           if(firstList.at(i) == secondList.at(j)) // если элементы равны
           {
               contained = true; // сохраняем, что этот элемент найден
           }
        }
        if(!contained) // если элемент не содержится
        {
            ok = false; // то сохрняем, что есть ошибка
            break; // дальше нет смысла проверять
        }
    }
    return ok;
}

void MainWindow::setStudentName(const QString &value)
{
    studentName = value;
}

void MainWindow::setStudentGroup(const QString &value)
{
    StudentGroup = value;
}

void MainWindow::loadDataFromFile()
{
    noMessage = true; // отключаем показ подсказок
    readDataFromFile(); // считываем данные из файла
    qDebug() << "выводим выражение в редактор " << itogMdnfTextLine;
    editor->setVirazhenie(itogMdnfTextLine); // выводим выражение в редактор
    noMessage = false; // включаем показ подсказок
    allowSave = true; // разрешаем сохранение
}

void MainWindow::setNoMessage(bool value)
{
    noMessage = value;
}


void MainWindow::pushButtonAddTSkeyki_1() // кнопка добавления склейки на основе таблицы со значениями, где функция принимает значение 1
{
    createSkleyka(tableWidgetOnesOnly, checkBoxes_ones, tableWidgetsSkleykiEditing[0], 1); // создаем склейку и добавляем её в таблицу
}

void MainWindow::pushButton_add_skleyki_2_clicked() // кнопка добаления склеек на основе таблицы склеек 1
{
    createSkleyka(tableWidgetsSkleyki[0], checkBoxes_skleyki_1, tableWidgetsSkleykiEditing[1], 2); // создаем склейку
}

void MainWindow::pushButton_clear_skleyki_1() // кнопка отчистки таблицы со склейками 1
{
    clearTW(tableWidgetsSkleykiEditing[0]); // отчищаем таблицу
}

void MainWindow::pushButton_proverka_skleyki_1_clicked() // кнопка проверки склеек 1
{

    if(proverkaSkleyki1()) // сравниваем таблицу со значениями склейки
    {
        message();
        nextStep();
    }
    else
    {
        warningError();
    }
}

void MainWindow::pushButton_delete_last_skleiki_1_editingClicked() // кнопка удаления последней добавленной склейки в таблице склеек 1
{
    deletelastRow(tableWidgetsSkleykiEditing[0]); // удалаяем последнюю строку в таблице склеек 1
}

void MainWindow::pushButton_nextStep_tot_2Clicked() // переход от 1й склейки ко 2й
{
    goToNextStep(tableWidgetsSkleykiEditing[0], tableWidgetsSkleyki[0], 2);
//    sortSkleiki(tableWidgetsSkleyki[0]);
    int rows = tableWidgetsSkleyki[0]->rowCount(); // получаем колитечтво строк
    if(rows <= 0)
        return;
    int cols = tableWidgetsSkleyki[0]->columnCount(); // получаем количество столбцов
    if(cols <= 0)
        return;
    // создаем массив
    QString **tableText = nullptr; // массив для хранения элементов таблицы
    tableText = new QString *[rows]; // выделяем память под строки
    for (int i=0; i<rows; i++)
    {
        tableText[i] = new QString [cols]; // выделяем память под столбцы
    }
    // получаем данные из таблицы
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<cols; j++)
        {
            tableText[i][j] = tableWidgetsSkleyki[0]->item(i, j)->text(); // сохраняем данные в массив
        }
    }
    // сортируем склейки
    QStringList sortSkleikyList; // создаем список для отсортированных склеек
//    int lastRow = 0; // номер строки, в которую последний раз были занесены данные (склейка)
    for (int p=0; p<cols; p++) // p = номер позиции Х в строке
    {
        qDebug() << "Ищем Х в позиции " << p;
        for (int i=0; i<rows; i++)
        {
            for (int j=0; j<cols; j++)
            {
                if((tableText[i][j] == "X") && (j == p))  // если позиция Х совпадает с текущим номером
                {
                    qDebug() << "В строке" << i << "в столбце" << j << "X находится на позиции" << p;
                    QString skleyka;
                    for (int h=0; h<cols; h++) // проходим по всем элементам строки
                    {
                        skleyka.append(tableText[i][h]);
//                        tableWidgetsSkleyki[0]->setItem(lastRow, h, new QTableWidgetItem((tableText[i][h]))); // выводим в таблицу
//                        lastRow++; // увеличиваем на 1 номер последней строки
                    }
                    qDebug() << "Создана склейка " << skleyka;
                    sortSkleikyList.append(skleyka); // обавляем склейку в список
                }
            }
        }
    }
    // ищём повторящиеся элементы (из-за несовсем корректной работы группировщика)
    for (int i=0; i<sortSkleikyList.size(); i++)
    {
        for (int j=0; j<i; j++)
        {
            if(sortSkleikyList[j] == sortSkleikyList[i]) // если существует
            {
                sortSkleikyList.removeAt(i); // удаляем
            }
        }
    }
    QStringList ishodniyList;
    if(!getTWTextList(tableWidgetsSkleyki[0], ishodniyList))
    {
        qDebug() << "Не получилось получить данные из " << tableWidgetsSkleyki[0];
        return;
    }
    for (int i=0; i<ishodniyList.size(); i++)
    {
        if(!(ishodniyList[i].contains("X"))) // если элемент не содержит Х, то добавляем его в конец
        {
            sortSkleikyList.append(ishodniyList[i]);
        }
    }
    int size = sortSkleikyList.size(); // получаем количество склеек в списке
    for (int i=0; i<size; i++)
    {
        QString skleyka = sortSkleikyList[i]; // получаем склейку
        for (int j=0; j<skleyka.count(); j++)
        {
            tableWidgetsSkleyki[0]->setItem(i, j, new QTableWidgetItem(skleyka.at(j))); // выподим каждый элемент
        }
    }
    nextStep();
}

void MainWindow::pushButton_add_skleyki_3_clicked() // кнопка создания 3й склейки
{
    createSkleyka(tableWidgetsSkleyki[1], checkBoxes_skleyki_2, tableWidgetsSkleykiEditing[2], 3); // создаем склейки 3
}

void MainWindow::pushButton_clear_skleyki_2()
{
    clearTW(tableWidgetsSkleykiEditing[1]); // отчищаем таблицу
}

void MainWindow::pushButton_proverka_skleyki_2_clicked()
{
    if(proverkaSkleyki2()) // если все нормально при проверке
    {
        message();
        nextStep();
    }
    else
    {
        warningError();
    }
}

void MainWindow::pushButton_delete_last_skleiki_2_editingClicked()
{
    deletelastRow(tableWidgetsSkleykiEditing[1]); // удалаяем последнюю строку в таблице склеек 2
}

void MainWindow::pushButton_nextStep_skleyki_2Clicked()
{
    goToNextStep(tableWidgetsSkleykiEditing[1], tableWidgetsSkleyki[1], 3);
    int rows = tableWidgetsSkleyki[1]->rowCount(); // получаем колитечтво строк
    if(rows <= 0)
        return;
    int cols = tableWidgetsSkleyki[1]->columnCount(); // получаем количество столбцов
    if(cols <= 0)
        return;
    // создаем массив
    QString **tableText = nullptr; // массив для хранения элементов таблицы
    tableText = new QString *[rows]; // выделяем память под строки
    for (int i=0; i<rows; i++)
    {
        tableText[i] = new QString [cols]; // выделяем память под столбцы
    }
    // получаем данные из таблицы
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<cols; j++)
        {
            tableText[i][j] = tableWidgetsSkleyki[1]->item(i, j)->text(); // сохраняем данные в массив
        }
    }
    // сортируем склейки
    QStringList sortSkleikyList; // создаем список для отсортированных склеек
//    int lastRow = 0; // номер строки, в которую последний раз были занесены данные (склейка)
    for (int p=0; p<cols; p++) // p = номер позиции Х в строке
    {
        for (int r = 0; r < cols; ++r) // r = номер второй позиции Х в строке
        {
            qDebug() << "Ищем Х в позиции " << p << "и" << r;
            for (int i=0; i<rows; i++)
            {
                for (int j=0; j<cols; j++) // ищем первое совпадение Х
                {
                    if((tableText[i][j] == "X") && (j == p))  // если позиция Х совпадает с текущим номером
                    {
                        qDebug() << "В строке" << i << "в столбце" << j << "X находится на позиции" << p;
                        for (int k=0; k<cols; k++)
                        {
                            if((tableText[i][k] == "X") && (k == p) && (j != k))  // если позиция Х совпадает с текущим вторым номером Х
                            {
                                QString skleyka;
                                for (int h=0; h<cols; h++) // проходим по всем элементам строки
                                {
                                    skleyka.append(tableText[i][h]);
                                    //tableWidgetsSkleyki[0]->setItem(lastRow, h, new QTableWidgetItem((tableText[i][h]))); // выводим в таблицу
                                    //lastRow++; // увеличиваем на 1 номер последней строки
                                }
                                qDebug() << "Создана склейка " << skleyka;
                                sortSkleikyList.append(skleyka); // обавляем склейку в список
                            }
                        }
                    }
                }
            }
        }
    }
    int size = sortSkleikyList.size(); // получаем количество склеек в списке
    for (int i=0; i<size; i++)
    {
        QString skleyka = sortSkleikyList[i]; // получаем склейку
        for (int j=0; j<skleyka.count(); j++)
        {
            tableWidgetsSkleyki[1]->setItem(i, j, new QTableWidgetItem(skleyka.at(j))); // выподим каждый элемент
        }
    }
    nextStep();
//    int rows = tableWidgetsSkleyki[1]->rowCount(); // получаем количество строк


}

void MainWindow::pushButton_clear_skleyki_3()
{
    clearTW(tableWidgetsSkleykiEditing[2]);
}

void MainWindow::pushButton_proverka_skleyki_3_clicked()
{
    proverkaTable(tableWidgetsSkleykiEditing[2], mdnf->getSkleyki3(typeMin));
}

void MainWindow::pushButton_delete_last_skleiki_3_editingClicked()
{
    deletelastRow(tableWidgetsSkleykiEditing[2]); // удалаяем последнюю строку в таблице склеек 3
}

void MainWindow::setKartaColor(QColor color)
{
    if(tableWidget_kartaMinimizacii == nullptr)
    {
        return;
    }
    int rows = tableWidget_kartaMinimizacii->rowCount(); // получаем количество строк
    int cols = tableWidget_kartaMinimizacii->columnCount(); // получаем колдичество колонок
    if((rows == 0) || (cols == 0))
    {
        return;
    }
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if(tableWidget_kartaMinimizacii->item(i, j) != nullptr) // если ячейка существует
            {
                if(tableWidget_kartaMinimizacii->item(i, j)->isSelected()) // если выделен
                {
                    if((color == *redColor) || (color == Qt::white) || ((color == *greenColor) && (tableWidget_kartaMinimizacii->item(i, j)->background() != *redColor))) // необходимо установить белый или красный цвет, или зеленый, но при том, что ячейка не выделена красным
                    {
                        tableWidget_kartaMinimizacii->item(i, j)->setBackground(color); // устанавливаем цвет
                    }
                }
            }
        }
    }
}

void MainWindow::pusbButton_move_skleyki_to_coresTable() // кнопка переноса склеек в таблицу
{
    qDebug() << "END << Start";
    int nomerSkleyki = 0; // Хранит номер последней склейки
//    if(tableWidgetsSkleyki[2]->rowCount() > 0) // если количество строк в таблице склеек больше 0, т.е. склейки есть
//    {
//        nomerSkleyki = 3; // запоминаем 3ю склейку
//    }
//    else
    if(tableWidgetsSkleyki[1]->rowCount() > 0)
    {
        nomerSkleyki = 2;
    }
    else if (tableWidgetsSkleyki[0]->rowCount() > 0)
    {
        nomerSkleyki = 1;
    }
    if(nomerSkleyki == 0)
    {
        criticalError("Вы не выполнили ни одной склейки!");
        qDebug() << "Нет склеек!";
        return;
    }
    qDebug() << "END << Получили номер склейки";
//    int nomerSkleykiMDNF = 0; // хранит сколько склеек должно быть вообще
//    nomerSkleykiMDNF = mdnf->getSkleykiMaxNumber(); // получаем номер последнего этапа
//    if(nomerSkleyki != nomerSkleykiMDNF)
//    {
//        qDebug() << "Не совпадает количество этапов склейки:" << nomerSkleyki << nomerSkleykiMDNF;
//        criticalError();
//        return;
//    }
    QStringList headers;
    if(!getTWTextList(tableWidgetOnesOnly, headers))
    {
        qDebug() << "В таблице, где функция равна 1 нет ни одной строки";
        criticalError("Вы не выбрали значения функции, при которых она принимает значние 1!");
        return;
    }
    qDebug() << "END << Получаем заголовки";
    QStringList vertical;
    if(!getTWTextList(tableWidgetsSkleyki[nomerSkleyki-1], vertical))
    {
        qDebug() << "Не удалось получить данные из таблцы " << nomerSkleyki-1;
        return;
    }
    if(vertical.size() == 0)
    {
        qDebug() << "END << Тублица пустая!";
        return;
    }
    qDebug() << "END << Получили заголовки";
    clearTW(tableWidgetKartaMinimizacii); // очищаем таблицу минимизации
    tableWidgetKartaMinimizacii->setRowCount(vertical.size()); // задаём количество строк равное количеству склеек
    tableWidgetKartaMinimizacii->setColumnCount(headers.size()); // задаем количество столбцов
    tableWidgetKartaMinimizacii->setHorizontalHeaderLabels(headers); // устаналиваем горизонтальные заголовки (значения равные 1)
    tableWidgetKartaMinimizacii->setVerticalHeaderLabels(vertical); // устаналиваем вертикальные заголовки (склейки)
    int rows = tableWidgetKartaMinimizacii->rowCount(); // получаем количество строк в таблице
    int cols = tableWidgetKartaMinimizacii->columnCount(); // получаем количество столбцов в таблице
    for (int i=0; i<rows; i++)  // проходим по всем строкам
    {
        for (int j=0; j<cols; j++) // проходим по всем столбцам
        {
            if(tableWidgetKartaMinimizacii->item(i, j) != nullptr) // если память уже выделена
            {
                delete tableWidgetKartaMinimizacii->item(i, j); // удаляем
            }
            tableWidgetKartaMinimizacii->setItem(i, j, new QTableWidgetItem("")); // выделяем память под новую ячейку
        }
    }
    qDebug() << "END << END";
//    int indexes = tabWidget->count(); // получаем количество элементов TabWidget
    tabWidget->setCurrentIndex(4);

}

void MainWindow::setTableStepenTwo(int stepen)
{
    if(stepen < 1)
    {
//        QMessageBox::critical(this, "Ошибка", "Система сичтления не может быть меньше 2");
        criticalError("Система сичтления не может быть меньше 2");
        return;
    }
    int rows = static_cast<int>(4 * stepen); // умножаем 4 (т.к. у нас 16-я ССЧ и она содержит по 4 двоичных разряда в числе) на количество разрядов числа и получаем количество строк
//    int cols = static_cast<int>(log2(rows) + 1); // количство столбцов равно степени в которую надо возвезти 2, чтобы получить количество строк + 1 для функции
    double step = log2(rows); // получаем, в какую степень над овозвести числ о2, чтобы получить такую длину числа
    if(abs(step - static_cast<double>(static_cast<int>(step))) > 0.0) // если степень не целая
    {
        step = static_cast<double>(static_cast<int>(step)); // откидываем дробную часть
        step += 1.0; // прибавляем 1
    }
    int cols = static_cast<int>(step) + 2;// количство столбцов равно степени в которую надо возвезти 2, чтобы получить количество строк + 1 для функции и +1 для "галочек"
    tableWidgetTot->setRowCount(rows);
    tableWidgetTot->setColumnCount(cols);
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<cols; j++)
        {
            setTWText(tableWidgetTot, i, j, "");
        }
    }
//    ui->tableWidget_tableOfTrue->setColumnCount(6);
    for (int i=0; i<rows; i++)
    {
        QWidget *checkBoxWidget = new QWidget(); //create QWidget
        QHBoxLayout *layoutCheckBox = new QHBoxLayout(checkBoxWidget); //create QHBoxLayout
        layoutCheckBox->addWidget(checkBoxes[i]);     //add QCheckBox to layout
        layoutCheckBox->setAlignment(Qt::AlignCenter); //set Alignment layout
        layoutCheckBox->setContentsMargins(0,0,0,0);

        ui->tableWidget_tableOfTrue->setCellWidget(i, cols-1, checkBoxWidget);
    }
    ui->tableWidget_tableOfTrue->setColumnWidth(cols-1, 90);
    QStringList headerList;
    for (int i=0; i<cols-2; i++) // добавляем переменные
    {
        char ch = 'a' + static_cast<char>(i);
        QString str;
        str.append(ch);
        headerList.append(str);
    }
    headerList.append("F"); // добавляем знак функции
    headerList.append("Выборка"); // добавляем знак функции
//    ui->tableWidget_tableOfTrue
    tableWidgetTot->setHorizontalHeaderLabels(headerList); // заполняем заголовок таблицы
    QStringList verticalList;
    for (int i=0; i<rows; i++)
    {
        verticalList.append(QString::number(i, 16).toUpper()); // добавляем числа в 16 ССЧ
        qDebug() << "Загооловк: " << verticalList[i];
//        qDebug() << i << QString::number(i, 16);
    }
    tableWidgetTot->setVerticalHeaderLabels(verticalList); // заполняем вертикальные заголовки
    for (int i=0; i<rows; i++)
    {
//        checkBoxes[i]->setVisible(true); // делаем видимыми нужное количество чекбоксов
        checkBoxes[i]->setEnabled(true); // делаем видимыми нужное количество чекбоксов
    }
}

void MainWindow::setTableOfTrueNumbers()
{
    int rows = tableWidgetTot->rowCount(); // получаем количество строк в таблице
    int cols = tableWidgetTot->columnCount(); // получаем количество колонок в таблице
    if(!rows || !cols)
    {
//        QMessageBox::critical(this, "Ошибка", "Задайте размер таблицы истиности!");
        criticalError("Задайте размер таблицы истиности!");
        return;
    }
    // заполняем числами от 0 до N в 2й ССЧ
    int maxcol = cols-3; // крайняя правая колонка, которую надо заполнить (она будет нулевым разрядом числа)
    for (int i=0; i<maxcol+1; i++)
    {
        int n = static_cast<int>(pow(2, i)); // возводим 2 в степень
        int k = 0; // сколько строк уже заполнено
        bool chislo = false; // число 0 или 1, которое записывается в таблицу
        int colnumber = maxcol-i; // вычисляем, в какую колонку будем записывать
        qDebug() << "i = " << i << "colnumber = " << colnumber;
        for (int i=0; i<rows; i++)
        {
            if(k < n) // если число цифр меньше степени, то записываем число
            {
                setTWText(tableWidgetTot, i, colnumber, QString::number(static_cast<int>(chislo))); // записывем значение числа (0 или 1)
                k++; // увелдичивам счётчик на 1
            }
            else // иначе изменяем число на противоположное (0 или 1) и заполняем дальше
            {
                chislo = !chislo; // инвертируем
                k = 0; // обнуляем счётчик
                setTWText(tableWidgetTot, i, colnumber, QString::number(static_cast<int>(chislo))); // записывем значение числа (0 или 1)
                k++; // увелдичивам счётчик на 1
            }
        }
    }

}

void MainWindow::on_pushButton_proverka_karta_minimizacii_clicked()
{
    if(proverkaItogMdnf())
    {
        message();
    }
    else
    {
        warningError();
    }
}

void MainWindow::on_pushButton_proverka_karti_minimizacii_clicked()
{
    if(proverkaKartaMinimizacii())
    {
        tableWidget_kartaMinimizacii->setEditTriggers(QAbstractItemView::NoEditTriggers); // блокируем ввод данных в таблицу, если она правильная
        message();
        nextStep();
    }
    else
    {
        warningError();
    }
//    message("Правильно!*\n\n* - Правильно при текущих входных параметрах.\nПри проверке не была учтена правильность склеек."); // если программа дошла до этого момента, значит все верно
//    nextStep();
}

void MainWindow::on_pushButton_clear_skleiki_1_editing_clicked() // удалить последний добавленный элемент
{
    deletelastRow(ui->tableWidget_skleiki_1_editing); // удаляет последнюю обавленную склейку
}

void MainWindow::on_pushButton_clear_skleiki_2_editing_clicked()
{
    deletelastRow(ui->tableWidget_skleiki_2_editing); // удаляет последнюю обавленную склейку
}

void MainWindow::on_pushButton_clearSkleyki_2_clicked()
{
    deletelastRow(ui->tableWidget_skleiki_3_editing); // удаляет последнюю обавленную склейку
}

//void MainWindow::on_pushButton_proverka_skleyki_2_clicked()
//{
//    if(proverkaTable(tableWidgetsSkleykiEditing[1], mdnf->getSkleyki2(typeMin)))
//    {
//        message();
//        nextStep();
//        pusbButton_move_skleyki_to_coresTable();
//    }
//    else
//    {
//        warningError();
//    }

//}

void MainWindow::on_pushButton_proverka_skleyki_3_0_clicked()
{
    if(proverkaSkleyki3())
    {
        message();
        nextStep();
//        pusbButton_move_skleyki_to_coresTable();
    }
    else
    {
        warningError();
    }
}

//void MainWindow::on_pushButton_loadDataFromFile_clicked()
//{
//    loadDataFromFile();
//}

void MainWindow::on_pushButton_nextStep_onesOnly_3_clicked()
{
    pusbButton_move_skleyki_to_coresTable();
    nextStep();
}


void MainWindow::on_pushButton_INV_clicked()
{
    editor->addInv();
}

void MainWindow::on_pushButton_A_clicked()
{
    addSymvol("A");
}

void MainWindow::on_pushButton_B_clicked()
{
    addSymvol("B");
}

void MainWindow::on_pushButton_C_clicked()
{
    addSymvol("C");
}

void MainWindow::on_pushButton_D_clicked()
{
    addSymvol("D");
}

void MainWindow::on_pushButton_notA_clicked()
{
    addSymvol("A", true);
}

void MainWindow::on_pushButton_notB_clicked()
{
    addSymvol("B", true);
}

void MainWindow::on_pushButton_notC_clicked()
{
    addSymvol("C", true);
}

void MainWindow::on_pushButton_notD_clicked()
{
    addSymvol("D", true);
}

void MainWindow::on_pushButton_AND_clicked()
{
    addSymvol("*");
}

void MainWindow::on_pushButton__OR_clicked()
{
    addSymvol("+");
}

void MainWindow::on_pushButton_SKOBKA_BEFORE_clicked()
{
    addSymvol("(");
}

void MainWindow::on_pushButton_SKOBKA_AFTER_clicked()
{
    addSymvol(")");
}

void MainWindow::on_pushButton_DEL_clicked()
{
    editor->deleteLastSymvol();
}

void MainWindow::on_pushButton_CLEAR_clicked()
{
    editor->clearTable();
}

//void MainWindow::on_tableWidget_formulaEditor_itemActivated(QTableWidgetItem *item)
//{
//    Q_UNUSED(item);
//}

//void MainWindow::on_tableWidget_formulaEditor_itemChanged(QTableWidgetItem *item)
//{
//    Q_UNUSED(item);
//}

void MainWindow::center_text_in_table(QTableWidget *tbw)
{
    if(tbw == nullptr)
    {
        return;
    }
    int rows = tbw->rowCount(); // получили количество строк
    int cols = tbw->columnCount(); // получаем количество столбцов
    qDebug() << "Выравниваем. Количество строк: "  << rows << "Количество столбцов: "  << cols;
    if(rows == 0 || cols == 0)
    {
        return;
    }
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<cols; j++)
        {
            if(tbw->item(i, j) != nullptr)
            {
                tbw->item(i, j)->setTextAlignment(Qt::AlignCenter); // выравниаем все элементы по центру
            }
        }
    }
}

void MainWindow::setVariablesToHeader(QTableWidget *tbw)
{
    if(tbw == nullptr)
    {
        return;
    }
    int cols = tbw->columnCount(); // получаем количество столбцов
    if(cols == 0) // если нет столбцов
    {
        return;
    }
    QStringList headerList; // создаем список, который будет хранить хаголовки
    for (int i=0; i<cols; i++) // добавляем переменные
    {
        char ch = 'a' + static_cast<char>(i); // формируем переменных путём прибавления 1 к char
        QString str;
        str.append(ch);
        headerList.append(str);
    }
    tbw->setHorizontalHeaderLabels(headerList); // устаналиваем заголовки
}


void MainWindow::pushButtonFunctionClicked()
{
    QString text = lineEditFunction->text(); // берем текст функции
    setFunction(text); // задаём функцию
}

void MainWindow::pushButtonTotAddClicked()
{

    qDebug() << "call pushButtonTotAddClicked()";
    bool add[16]; // проверяем, добавляет ли пользователь строку или нет для склейки
    int countChecked = 0; // переменная для подсчёта, сколько элементов выбрано
    for (int i=0; i<16; i++)
    {
        add[i] = checkBoxes[i]->isChecked(); // Заносим данные о состоянии чекбоксов
        countChecked += static_cast<int>(checkBoxes[i]->isChecked()); // если галочка стоит, то будет +1
//        qDebug() << i << add[i] << ui->checkBox_tot_0->isChecked() << ui->checkBox_tot_0->isDown();
    }
    if(countChecked == 0) // если не выбрано ни одной галочки
    {
        warningError("Ошибка! Выберите значения!");
        return;
    }
    int rows = tableWidgetTot->rowCount();
    int cols = tableWidgetTot->columnCount() - 1;
//    ui->tableWidget_one_only->setRowCount(rows);
    tableWidgetOnesOnlyEditing->setColumnCount(cols);
    int c = 0; // счетчкис заполненых строк
    for (int i=0; i<rows; i++)
    {
//        if(ui->tableWidget_tableOfTrue->item(i, cols)->text().toInt() == 1)  // если в строчке 1, то обрабатываем
        if(add[i]) // если пользователь выбрал эту строку
        {
            tableWidgetOnesOnlyEditing->setRowCount(c + 1);
            for (int j=0; j<cols; j++)
            {
                setTWText(tableWidgetOnesOnlyEditing, c, j, tableWidgetTot->item(i, j)->text()); // переносим значение ячейки в новую таблицу
            }
            c++; // увеличиваем значение счётчика
        }
    }
}

void MainWindow::pushButtonFunc2Clicked()
{
    QString chislo = lineEditFunc_2->text(); // получаем число из LineEdit
    if(chislo.isEmpty()) // если введена пустая строка
    {
        qDebug() << "Нет числа в функуии 2";
        warningError("Введите число!"); // выдаем ошибка
        return;
    }
    if(!chislo.contains("0") && !chislo.contains("1")) // если не содержит 0 и не содержит 1
    {
        qDebug() << "Число должно содержать нули и единицы";
        warningError("Число должно быть записано в 2й системе счисления!");
        return;
    }
    chislo.remove(" ");
    int count = chislo.count();  // получаем количество символов в строке
    int rows = tableWidgetTot->rowCount(); // получаем количество строк в таблице истинности
    if(count != rows) // если количество введенных символов не совпадает с количеством строк в таблице
    {
        qDebug() << "Количество символов в число не совпадает с количеством строк";
        warningError("Проверьте количество введеннх символов!");
        return;
    }
    int cols = tableWidgetTot->columnCount(); // получаем количетсво солбцов в таблице истинности
    QStringList list = chislo.split(""); // разбиваем число на символы
    for (int i=0; i<rows; i++)
    {
         setTWText(tableWidgetTot, i, cols-1, chislo.at(i));   // выводим значения в таблицу истинности
    }
//    message("Число, полученное в 2 системе счисления добавлено в таблицу истинности. Для проверки правильности воспользуйтесь кнопкой проверка табилцы истинности.");
    //    nextStep(); // переходим к следующему шагу
    function_2 = chislo; // сохраняем 2ю запись функции
}

int MainWindow::getType(QString func_2, QStringList listFunc)
{
    if(func_2.count() < listFunc.size())
    {
        qDebug() << "Размер функции меньше размера списка, для определния типа!";
        return -1;
    }
    int funcSize = func_2.count(); // получаем количество элементов в функции
    int listSize = listFunc.size(); // получаем количество элементов в списке для получения типа
    int zeroCount = 0; // количество выбранных значений, при которых функция принимает значение 0
    int oneCount = 0; // количество выбранных значений, при которых функция принимает значение 1
    for (int i=0; i<listSize; i++)
    {
        bool flag = false; // флаг перевода
        int chislo = listFunc.at(i).toInt(&flag, 2); // переводим число из 2й в 10 ссч
        if(!flag) // если число не переводится
        {
            qDebug() << "Число " << listFunc.at(i) << "не удалось перевести в 10ю ссч!";
            return -1;
        }
        if(chislo >= funcSize) // если номер больше или равен количеству элементов в функции, значит что-то не так
        {
            qDebug() << "Номер " << chislo << "значения" << listFunc.at(i) << " больше, чем размер" << funcSize << " функции" << func_2;
            return -1;
        }
        if(func_2.at(chislo) == "0") // если функция при этом значении даёт 0
        {
            zeroCount++; // увеличиваем счётчик значений, при которых F=0
        }
        if(func_2.at(chislo) == "1") // если функция при этом значении даёт 1
        {
            oneCount++; // увеличиваем счётчик значений, при которых F=1
        }
    }
    if(((zeroCount != 0) && (oneCount != 0)) || ((zeroCount == 0) && (oneCount == 0))) // если присутствуют и значения, при которых 0, и те, при которых 1, т.е. нельзя определить тип - МКНФ или МДНФ
    {
        qDebug() << "Не удалось определить тип! Значений для F=0 - " << zeroCount << ", значений для F=1 - " << oneCount;
        return -1;
    }
    int funcZeroCount = 0; // переменная для подсчёта количества нулей в исходной функции
    int funcOneCount = 0; // переменная для подсчёта количества единиц в исходной функции
    for (int i=0; i<funcSize; i++)
    {
        if(func_2.at(i) == "0") // если значение функции равно 0
        {
            funcZeroCount++; // увеличиваем счётчик нулей
        }
        if(func_2.at(i) == "1") // если значение функции равно 1
        {
            funcOneCount++; // увеличиваем счётчик единиц
        }
    }
    if((funcZeroCount == zeroCount) && (zeroCount > 0)) // если количество нулей совпадает с данными в функции
    {
        return 0;
    }
    else if((funcOneCount == oneCount) && (oneCount > 0)) // если количество единиц совпадает с данными в функции
    {
        return 1;
    }
    else // если ничего не совпадает
    {
        return -1;
    }
}

int MainWindow::getOptimizeType(QString func_2)
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
    qDebug() << "Исходная функция:" << func_2 << "Количество нулей: " << zeroCount << " Количество единиц: " << oneCount;
    if(zeroCount < oneCount) // если нулей больше, чем единиц
    {
        if(mdnf != nullptr) // если объект мак класки создан
        {
            if(mdnf->canCalculateMnf(0)) // проверяем, можно-ли рассчитать МКНФ
            {
                return 0;
            }
        }
    }
    if(zeroCount > oneCount) // если нулей меньше, чем единиц
    {
        if(mdnf != nullptr) // если объект мак класки создан
        {
            if(mdnf->canCalculateMnf(1)) // проверяем, можно-ли рассчитать МДНФ
            {
                return 1;
            }
        }
    }
    if(zeroCount == oneCount) // если количество равно
    {
        bool canCalculateMknf = false; // флаг возможности рассчёта МКНФ
        bool canCalculateMdnf = false; // флаг возможности рассчёта МДНФ
        if(mdnf != nullptr) // если объект мак класки создан
        {
            canCalculateMknf = mdnf->canCalculateMnf(0); // считаем возможность рассчёта МКНФ
            canCalculateMdnf = mdnf->canCalculateMnf(1); // считаем возможность рассчёта МДНФ
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

void MainWindow::pushButtonCheckTableOfTrueClicked() // проверка таблицы истинности
{
//    proverkaTable(tableWidgetTot, mdnf->getSschTableList());
    if(proverkaTableOfTrue()) // сравниваем таблицу со значениями МДНФ
    {
        message(); // если правильно
        step++; // увеличиваем шаг на 1
        setStep(step);// переходим к следующему шагу
//        ui->tableWidget_tableOfTrue->setColumnCount(6);
//        for (int i=0; i<16; i++)
//        {
//            QWidget *checkBoxWidget = new QWidget(); //create QWidget
//            QHBoxLayout *layoutCheckBox = new QHBoxLayout(checkBoxWidget); //create QHBoxLayout
//            layoutCheckBox->addWidget(checkBoxes[i]);     //add QCheckBox to layout
//            layoutCheckBox->setAlignment(Qt::AlignCenter); //set Alignment layout
//            layoutCheckBox->setContentsMargins(0,0,0,0);

//            ui->tableWidget_tableOfTrue->setCellWidget(i,5, checkBoxWidget);
//        }
//        ui->tableWidget_tableOfTrue->setColumnWidth(5, 90);
//        center_text_in_table(ui->tableWidget_tableOfTrue);
    }
    else
    {
        warningError(); // если ошибка
    }
}

void MainWindow::pushButton_delete_last_onesClicked()
{
    deletelastRow(tableWidgetOnesOnlyEditing);
}


void MainWindow::pushButton_nextStep_totClicked()
{
    goToNextStep(tableWidgetOnesOnlyEditing, tableWidgetOnesOnly, 1); // переходим к следующему действию
    nextStep();
    sortOnesCount(tableWidgetOnesOnly); // сортируем значения по количеству единиц
}

void MainWindow::pushButtonClearOneOnlyClicked()
{
    qDebug() << "call pushButtonClearOneOnlyClicked()";
    tableWidgetOnesOnlyEditing->clear();
    tableWidgetOnesOnlyEditing->setRowCount(0);
    tableWidgetOnesOnlyEditing->setColumnCount(0);

}

void MainWindow::pushButtonProverkaClicked() // проверка правильности где функция равна 1
{
    QStringList listFunc;
    bool ok = getTWTextList(tableWidgetOnesOnlyEditing, listFunc); // получаем списолк значений из таблицы
    if(!ok)
    {
        warningError("Таблица пуста!");
        return;
    }
    int type = getType(function_2, listFunc); // пытаемся определить тип
    if(type == -1) // если не удалось определить тип
    {
       warningError("Ошибка! Не удалось определить тип минимизации!\nДобавьте в таблицу все значения при которых функция принимает 0 или 1!");
       return;
    }
    int optimizeType = getOptimizeType(function_2); // получаем ,какой тип минимизации предпочтительнее
    qDebug() << "Тип: " << type << " Оптимальный тип: " << optimizeType;
    if((optimizeType == -1) || (optimizeType == 3) || (optimizeType == 4)) // проверяем, если вдруг в программе получится ошибка
    {
        if(optimizeType == -1)
        {
            criticalError("Произошла непредвиденная ошибка!");
            return;
        }
        if(optimizeType == 3)
        {
            criticalError("Произошла непредвиденная ошибка!\nПопробуйте использовать МКНФ для минимизации.");
            return;
        }
        if(optimizeType == 4)
        {
            criticalError("Произошла непредвиденная ошибка!\nПопробуйте использовать МДНФ для минимизации.");
            return;
        }
    }
    if(optimizeType != 2) // если не 2й тип, т.е. тот, когда можно использовать, и МКНФ, и МДНФ, т.к. одинаковое количество 0 и 1
    {
        if(type != optimizeType)
        {
            warningError("Ошибка! Ещё раз посчитайте количество 0 и 1 в функции!");
            return;
        }
    }
    if(type == 0)
    {
        message("Определён тип: МКНФ");
        qDebug() << "Определён тип: МКНФ";
    }
    else
    {
        message("Определён тип: МДНФ");
        qDebug() << "Определён тип: МДНФ";
    }
    typeMin = type; // сохраняем тип
    QStringList mmkList = mdnf->getListOnes(typeMin);
    for (int i=0; i<mmkList.size(); i++)
    {
        qDebug() << "MMK List [" << i << "] = " << mmkList.at(i);
    }
    if(proverkaOnesOnly())
    {
        editor->setAutoInputType(typeMin); // устанавливаем тип автоматического ввода
        message();
        step++; // увеличиваем шаг на 1
        setStep(step);// переходим к следующему шагу
        skleyki[0] = mdnf->getSkleyki1(typeMin);
        skleyki[1] = mdnf->getSkleyki2(typeMin);
        skleyki[2] = mdnf->getSkleyki3(typeMin);
        qDebug() << "Склека 3: " << mdnf->getSkleyki3(typeMin);
    }
    else
    {
        warningError();
    }

}

void MainWindow::on_pushButton_tot_move_to_skeyki_1_clicked()
{
    qDebug() << "Нажато!";
    moveSkleyka(tableWidgetOnesOnly, checkBoxes_ones, tableWidgetsSkleykiEditing[0], 1); // переносим выделенные элементы без склейки
}

void MainWindow::on_pushButton_tot_move_to_skeyki_2_clicked()
{
    moveSkleyka(tableWidgetsSkleyki[0], checkBoxes_skleyki_1, tableWidgetsSkleykiEditing[1], 2); // переносим выделенные элементы без склейки
}

void MainWindow::on_pushButton_tot_move_to_skeyki_3_clicked()
{
    moveSkleyka(tableWidgetsSkleyki[1], checkBoxes_skleyki_2, tableWidgetsSkleykiEditing[2], 3); // переносим выделенные элементы без склейки
}

void MainWindow::on_pushButton_setRedColor_clicked()
{
    setKartaColor(*redColor); // задём красный цвет выделенным значениям
}

void MainWindow::on_pushButton_setGreenColor_clicked()
{
    setKartaColor(*greenColor); // задём зелёный цвет выделенным значениям
}

void MainWindow::on_pushButton_setWhiteColor_clicked()
{
    setKartaColor(Qt::white); // задём белый цвет выделенным значениям
}

void MainWindow::addSymvol(QString symvol, bool inversiya)
{
    if(editor == nullptr) // если не создан объект редакора формул
    {
        return;
    }
    symvol = symvol.toUpper();
    qDebug() << symvol;
    if(autoInput) // если включена функция автоматического ввода
    {
        if(((symvol >= "A") && (symvol <= "D"))/* || (symvol == ")")*/) // если символ - буква или символ
        {
            QString formula = editor->getFormulaText(); // получаем формулу
            qDebug() << "formula = " << formula;
            if(formula != "ERROR")
            {
                qDebug() << "count =" << formula.count();
                formula = formula.toUpper();
                QString lastSymvol = formula.at(formula.count() -1); // получаем последний символ
                if(editor->isEditChoseItem()) // если происходит редактирование выбранного элемента
                {
                    int selectedItem = -1;
                    int selectedCount = 0;
                    editor->getSelectedItemNumber(tableWidget_formulaEditor, selectedItem, &selectedCount);
                    if(selectedCount > 1)
                    {
                        warningError("Выберите только один символ, после которого необходимо добавление олперанда");
                        return;
                    }
                    qDebug() << "номер выбранного столбца: " << selectedItem;
                    qDebug() << "Всего символов в строке: " << formula.count();
                    lastSymvol = tableWidget_formulaEditor->item(1, selectedItem)->text();
                }
                qDebug() << "last symvol = " << lastSymvol;
                if((lastSymvol >= "A") && (lastSymvol <= "D")/* && (symvol >= "A") && (symvol <= "D")*/) // если предыдущий символ - буква
                {
                    if(symvol > lastSymvol)
                    {
                        if(typeMin == 0)
                        {
                            editor->addSymvol("+");
                        }
                        else if(typeMin == 1)
                        {
                            editor->addSymvol("*");
                        }
                    }
                    else
                    {
                        editor->addSymvol(")");
                        if(typeMin == 0)
                        {
                            editor->addSymvol("*");
                        }
                        else if(typeMin == 1)
                        {
                            editor->addSymvol("+");
                        }
                        editor->addSymvol("(");
                    }
                }
                else if (lastSymvol == ")") // если закрывающаяся скобка
                {
                    if(typeMin == 0)
                    {
                        editor->addSymvol("*");
                    }
                    else if(typeMin == 1)
                    {
                        editor->addSymvol("+");
                    }
                    editor->addSymvol("(");
                }
            }
            else // если у нас первый символ, то ставим открывающуюся скобку
            {
                editor->addSymvol("(");
            }
        }
    }
    qDebug() << "formula = " << editor->getFormulaText();
    editor->addSymvol(symvol, inversiya);
    if(symvol == "D") // если символ D, то сразу закрываем скобку
    {
        editor->addSymvol(")");
    }
}

void MainWindow::setAutoInput(bool value)
{
    autoInput = value;
}

//void MainWindow::on_pushButton_proverkaAll_clicked()
//{
//    QString hardLevel = "";
//    int count = function_16.count(); // получаем, сколько значений в исходном 16м числе
//    hardLevel = QString::number(count) + " переменных";
//    fpl = new formProverkaAll(this);
//    fpl->show();
//    fpl->setData(proverkaTableOfTrue(), proverkaOnesOnly(), proverkaSkleyki1(), proverkaSkleyki2(), proverkaSkleyki3(), proverkaKartaMinimizacii(), proverkaItogMdnf(), hardLevel);
//}

void MainWindow::on_checkBox_autoInputFormula_stateChanged(int arg1)
{
    setAutoInput(static_cast<bool>(arg1)); // устанавливаем значение автоматического ввода
}

void MainWindow::on_checkBox_editChoseItemFormula_stateChanged(int arg1)
{
    if(editor != nullptr)
    {
        editor->setEditChoseItem(static_cast<bool>(arg1)); // устанавливаем значение редактирования выбранного элемента
    }
}

void MainWindow::on_lineEdit_func_2_textChanged(const QString &arg1)
{
    qDebug() << "строка" << arg1;
//    if(!splitToTetrads)
//    {
//        return;
//    }
//    if(arg1.isEmpty())
//    {
//        return;
//    }
//    if(arg1.at(arg1.size()-1) == " ")
//    {
//        return;
//    }
    QString inputString = arg1;
    inputString = inputString.remove(" ");
    int cols = tableWidgetTot->columnCount(); // получаем количетсво солбцов в таблице истинности
    int rows = tableWidgetTot->rowCount();
    if(!splitToTetrads)
    {
        for (int i=0; i<rows; i++)
        {
            QString itemText = "";
            if(i<inputString.count())
            {
                itemText = inputString.at(i);
            }
             setTWText(tableWidgetTot, i, cols-2, itemText);   // выводим значения в таблицу истинности
        }
//        ui->lineEdit_func_2->blockSignals(true);
//        ui->lineEdit_func_2->setText(inputString);
//        ui->lineEdit_func_2->blockSignals(false);
    }
    else
    {
        QString outputString;
    //    QString tempString;
        for (int i=0; i<inputString.count(); i++)
        {
            qDebug() << "i = " << i;
            outputString.append(inputString.at(i));
            if((i+1) % 4 == 0 && i != inputString.count()-1) // делится без остатка на 4
            {
                qDebug() << ((i+1) % 4 == 0) << "i = " << i;
                outputString.append(" "); // Добавляем пробел
            }
        }
    //    int outPutSize = outputString.size();
    //    if(outPutSize > 0)
    //    {
    //        if(outputString.at(outPutSize-1) == " ")
    //        {
    //            outputString.remove(outPutSize-1);
    //        }
    //    }
        ui->lineEdit_func_2->blockSignals(true);
        ui->lineEdit_func_2->setText(outputString);
        ui->lineEdit_func_2->blockSignals(false);

        QStringList list = inputString.split(""); // разбиваем число на символы
        for (int i=0; i<rows; i++)
        {
            QString itemText = "";
            if(i<inputString.count())
            {
                itemText = inputString.at(i);
            }
             setTWText(tableWidgetTot, i, cols-2, itemText);   // выводим значения в таблицу истинности
        }
    //    message("Число, полученное в 2 системе счисления добавлено в таблицу истинности. Для проверки правильности воспользуйтесь кнопкой проверка табилцы истинности.");
        //    nextStep(); // переходим к следующему шагу
        function_2 = inputString; // сохраняем 2ю запись функции
    }

}

void MainWindow::on_checkBox_spiltToTetrads_stateChanged(int arg1)
{
    splitToTetrads = arg1;
    on_lineEdit_func_2_textChanged(ui->lineEdit_func_2->text());
}
