#include "formlogin.h"
#include "ui_formlogin.h"

FormLogin::FormLogin(MainWindow &w, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormLogin)
{
    ui->setupUi(this);
//    currentFunction = &function;
    mw = &w; // сохраянем указатель

    auto closeLambda = [&]() // функция закрытия окна
    {
        closetimer = new QTimer(this);
        connect(closetimer, &QTimer::timeout, this, &FormLogin::closeWindow);
        closetimer->start(1);
    };

    QFile *fileStudents = new QFile(FILE_STUDENTS_NAME);
    if(!fileStudents->exists())
    {
        message("Не найден файл с вариантами!");
        closeLambda();
        return;
    }
    if(!fileStudents->open(QIODevice::ReadOnly)) // если не открывается
    {
        message("Не удаётся открыть файл со студентами!");
        closeLambda();
        return;
    }
    qDebug() << "Проверили файл";
    QString text = fileStudents->readAll(); // считываем весь файл
    fileStudents->close(); // закрываем файл
    delete fileStudents;
    fileStudents = nullptr;
    bool ok = false; // флаг хеша
    md5crypter::decryptStr(text, ok); // убираем и проверяем хеш
    if(!ok)
    {
        QMessageBox::critical(this, "Критическая ошибка", "Критическая ошибка!\nФайл вариантов повреждён!");
        closeLambda();
        return;
    }
    QStringList studentsQStringList = text.split(STR_SEPARATOR_1); // разбиваем по строкам
    if(studentsQStringList.size() <= 1) // если в файле ничего нет
    {
        message("Файл пустой или повреждён!");
        closeLambda();
        return;
    }
    qDebug() << "Прочли файл";
    students = new QList<studentStruct>; // выделяем память
    qDebug() << "Создали список студентов";
    for (int i=0; i<studentsQStringList.size(); i++)
    {
        QStringList listLine = studentsQStringList[i].split(STR_SEPARATOR_2); // ищем данные
        qDebug() << listLine;
        if(listLine.size() < 3)
        {
            continue;
        }
        qDebug() << "i = " << i;
        studentStruct *student = new studentStruct;
        student->group = listLine[0];
        student->name = listLine[1];
        if(listLine.size() > 2)
        {
            student->function = listLine[2];
        }
        student->function = listLine[2];
        qDebug() << "Добавли данные в структуру";
        students->append(*student);
        qDebug() << "удалили указатель";
        bool existGroup = false; // хранит, существует ли уже эта группа
        for (int j=0; j<groups.size(); j++)
        {
            if(groups[j] == student->group)  // если группа есть
            {
                existGroup = true;
                break;
            }
        }
        qDebug() << "Проверили наличие группы " << existGroup;
        if(!existGroup) // если группа не найдена
        {
            groups.append(student->group); // добавляем группу
            qDebug() << "Добавили группу";
        }
        qDebug() << "Идём дальше";
        delete student;
        student = nullptr;
    }
    ui->comboBox_group->addItems(groups);
}

FormLogin::~FormLogin()
{
    delete ui;
}

void FormLogin::on_pushButton_accept_clicked()
{
    this->hide();
    mw->show();
    mw->setFunction(currentFunction); // устанавливаем функцию
    mw->setStudentName(ui->comboBox_student->currentText()); // передаём имя
    mw->setStudentGroup(ui->comboBox_group->currentText()); // передаём группу
}

void FormLogin::message(QString text, int type, QString title)
{
    if(type == 0)
    {
        QMessageBox::information(this, title, text);
    }
    else if(type == 1)
    {
        QMessageBox::warning(this, title, text);
    }
    else if(type == 2)
    {
        QMessageBox::critical(this, title, text);
    }
}

void FormLogin::on_comboBox_group_currentIndexChanged(const QString &arg1)
{
    ui->comboBox_student->clear(); // очищаем comboBox
    for (int i=0; i<students->size(); i++)
    {
        if(students->at(i).group == arg1) // если группа совпадает
        {
            ui->comboBox_student->addItem(students->at(i).name);
        }
    }
}

void FormLogin::on_comboBox_student_currentIndexChanged(const QString &arg1)
{
    for (int i=0; i<students->size(); i++)
    {
        if((students->at(i).name == arg1) && (students->at(i).group == ui->comboBox_group->currentText())) // если имя и группа совпадает
        {
            currentFunction = students->at(i).function; // сохраняем текущую функцию
            qDebug() << " Функция: " << students->at(i).function;
        }
    }
}

void FormLogin::on_pushButton_load_clicked()
{
    QFile file(FILE_STUDENT_WORK); // открываем файл с сохраненной работой
    if(!file.open(QIODevice::ReadOnly)) // если не удалось открыть
    {
        qDebug() << "Не удалось открыть файл для чтения!";
        message("Нет сохранённого решения!");
        return;
    }
    QString text = file.readAll(); // считываем весь файл
    bool ok = false;
    md5crypter::decryptStr(text, ok);
    if(!ok)
    {
        qDebug() << "Ошибка хеша в файле решения!";
        message("Файл решения повреждён!", 2);
        return;
    }
    QStringList list = text.split(STR_SEPARATOR_1); // разбиваем по строчкам
    int size = list.size(); // получаем количество строк
    if(size < 2) // если пусто
    {
        qDebug() << "Файл пустой!";
        message("Непредвиденная ошибка!", 1);
        return;
    }
    QString name = "";
    QString group = "";
    for (int i=0; i<size; i++)
    {
        QString textLine = list.at(i); // получаем текст строки
        if(textLine.contains("name"))
        {
            QStringList dataList = textLine.split("=", Qt::SplitBehavior(Qt::SkipEmptyParts)); // разбиваем строку на до = и после
            if(dataList.size() != 2)
            {
                break;
            }
            name = dataList.at(1); // сохраняем ФИО
        }
        if(textLine.contains("group"))
        {
            QStringList dataList = textLine.split("=", Qt::SplitBehavior(Qt::SkipEmptyParts)); // разбиваем строку на до = и после
            if(dataList.size() != 2)
            {
                break;
            }
            group = dataList.at(1); // сохраняем группу
        }
    }
    if(name != "" && group != "")
    {
        for (int i=0; i<students->size(); i++)
        {
            if((students->at(i).name == name) && (students->at(i).group == group)) // если имя и группа совпадает
            {
                currentFunction = students->at(i).function; // сохраняем текущую функцию
                qDebug() << " Функция: " << students->at(i).function;
            }
        }
    }
    mw->setNoMessage(true); // отключаем показ подсказок (после загрузки данных, они включатся из функции)
    mw->show(); // запускасем окно
    mw->setFunction(currentFunction); // устанавливаем функцию
    mw->setStudentName(name); // передаём имя
    mw->setStudentGroup(group); // передаём группу
    mw->loadDataFromFile(); // загружаем данные
    this->hide();
}

void FormLogin::closeWindow()
{
    this->close();
}
