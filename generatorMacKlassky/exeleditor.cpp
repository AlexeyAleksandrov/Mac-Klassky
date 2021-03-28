#include "exeleditor.h"

ExelEditor::ExelEditor(QObject *parent) : QObject(parent)
{

}

QString **ExelEditor::readFile(QString fileDiretory, int &rowsCount, int &colsCount)
{
    emit updateDataText("Открытие Exel..");
    QAxObject* excel = new QAxObject("Excel.Application", 0);
    emit updateDataText("Получение информации о данных...");
    QAxObject* workbooks = excel->querySubObject("Workbooks");
    QAxObject* workbook = workbooks->querySubObject("Open(const QString&)", fileDiretory);
    QAxObject* sheets = workbook->querySubObject("Worksheets");
    QAxObject* sheet = sheets->querySubObject("Item(int)", 1);

    QAxObject* usedRange = sheet->querySubObject("UsedRange");
    QAxObject* rows = usedRange->querySubObject("Rows");
    QAxObject* columns = usedRange->querySubObject("Columns");
    const int countRows = rows->property("Count").toInt();
    const int countCols = columns->property("Count").toInt();


    rowsCount = countRows;
    colsCount = countCols;

    QString **text = new QString *[countRows]; // выделяем память под строки
    for(int i=0; i<countRows; i++)
    {
        text[i] = new QString[countCols]; // выделяем память под столбцы
    }

    emit updateDataText("Чтение данных...");

    for ( int row = 0; row < countRows; row++ )
    {
        for ( int column = 0; column < countCols; column++ )
        {
            QAxObject* cell = sheet->querySubObject("Cells(int,int)", row + 1, column + 1);
            QVariant value = cell->property("Value");
            text[row][column] = value.toString();;
        }
    }

    emit updateDataText("Данные прочитаны, идёт закрытие файла...");

    workbook->dynamicCall("Close()");
    excel->dynamicCall("Quit()");
    delete  excel;
    excel = NULL;
    emit updateDataText("Файл закрыт. Идёт обработка данных.");

    return text;
}

void ExelEditor::writeFile(QString fileDiretory, QString **text, int rows, int cols)
{

    QAxObject* excel = new QAxObject("Excel.Application", 0);
    QAxObject* workbooks = excel->querySubObject("Workbooks");
    QAxObject* workbook = workbooks->querySubObject("Open(const QString&)", fileDiretory);
    QAxObject* sheets = workbook->querySubObject("Worksheets");
    QAxObject* sheet = sheets->querySubObject("Item(int)", 1);

    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<cols; j++)
        {
            QAxObject *cell = sheet->querySubObject("Cells(int, int)",i+1, j+1);
            cell->dynamicCall("SetValue(const QVariant&", text[i][j]);
        }
    }

    workbook->dynamicCall("Close()");
    excel->dynamicCall("Quit()");
    delete  excel;
    excel = NULL;
}
