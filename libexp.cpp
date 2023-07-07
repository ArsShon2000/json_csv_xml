#include "libexp.h"

LibExp::LibExp(int Extension, int table, Models models, QWidget *parent)
{
    definitionTable(&models, table);
    switch (Extension) {
    case 0:
        exportJSONFormat(modelForDefinition);
        break;

    case 1:
        exportXMLFormat(modelForDefinition);
        break;

    case 2:
        exportCSVFormat(modelForDefinition);
        break;

    default:
        break;
    }
}

void LibExp::definitionTable(Models *models, int table)
{
    switch (table) {
    case 0:
        modelForDefinition = models->carsModel;
        break;
    case 1:
        modelForDefinition = models->camerasModel;
        break;
    case 2:
        modelForDefinition = models->employeesModel;
        break;
    default:
        break;
    }
}

void LibExp::exportJSONFormat(QSqlTableModel *modelTablesForExport)
{
    columnsCount = modelTablesForExport->columnCount();                                                         // ПОЛУЧАЕМ КОЛИЧЕСТВО КОЛОНОК
    rowCount = modelTablesForExport->rowCount();                                                                // ПОЛУЧАЕМ КОЛИЧЕСТВО СТРОК
    modelTablesForExport->select();                                                                             // ОБНОВЛЯЕМ ТАБЛИЦУ
    char ch = 11;                                                                                               // ЯКОБЫ КЛЮЧ К JSON БЛОК
    for (int i = 0; i < rowCount; ++i) {                                                                        // ПРОБЕГАЕМ ПО СТРОКУ
        for (int j = 0; j < columnsCount; ++j) {                                                                // ПРОБЕГАЕМ ПО КОЛОНКЕ
            recordColumns.insert(modelTablesForExport->headerData(j, Qt::Horizontal).toString(),                // ЗАПИСЫВАЕТСЯ ВНУТРЕННИЙ БЛОК
                                 QJsonValue::fromVariant(modelTablesForExport->data(modelTablesForExport->index(i, j))));
        }
        recordRows.insert(QString::number(ch++), recordColumns);                                                // ЗАПИСЫВАЕТСЯ ВНЕШНИЙ БЛОК С КЛЮЧОМ
    }

    QJsonDocument doc(recordRows);                                                                              // СОЗДАЕТСЯ JSON ДОКУМЕНТ
    QString jsonString = doc.toJson(QJsonDocument::Indented);                                                   // ПРИВОДИМ К СТРИНГ

    QString filename = QFileDialog::getSaveFileName(this,                                                       // ДИАЛОГОВОЕ ОКНО ДЛЯ СОХРАНЕНИЯ
                                                    tr("Save json"), ".",
                                                    tr("json files (*.json)"));
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);                                                          // ОТКРЫВАЕМ
    QTextStream stream(&file);
    stream << jsonString;                                                                                       // ЗАПИСЫВАЕМ ДАННЫЕ В ФАЙЛ
    file.close();
    QMessageBox::information(NULL,"show","Данные успешно сохранены");
}

void LibExp::exportXMLFormat(QSqlTableModel *modelTablesForExport)
{
    QString filename = QFileDialog::getSaveFileName(this,                                                       // ДИАЛОГОВОЕ ОКНО ДЛЯ СОХРАНЕНИЯ
                                                    tr("Save xml"), ".",
                                                    tr("xml files (*.xml)"));
    QFile file(filename);
    if( !file.open(QIODevice::WriteOnly) )                                                                      // ЕСЛИ НЕ УДАЛОСЬ
    {
        QMessageBox::warning(this,
                             "Ошибка файла",
                             "Не удалось открыть файл",
                             QMessageBox::Ok);
    }
    else {
        QXmlStreamWriter xmlWriter(&file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartElement("List");                                                                            // ГЛАВНАЯ ЗАГОЛОВКА
        for (int i = 0; i < modelTablesForExport->rowCount(); ++i)
        {
            QString idForXMLBlock = "id_" + modelTablesForExport->                                                      // ВНУТРЕННИЕ БЛОКИ
                    data(modelTablesForExport->index(i, 0)).toString();
            xmlWriter.writeStartElement(idForXMLBlock);
            for (int j = 0; j < modelTablesForExport->columnCount(); ++j)
            {
                xmlWriter.writeStartElement(modelTablesForExport->headerData(j, Qt::Horizontal).toString());            // ДАННЫЕ ВНУТРИ БЛОКЕ
                xmlWriter.writeAttribute(modelTablesForExport->headerData(j, Qt::Horizontal).toString(),
                                         modelTablesForExport->data(modelTablesForExport->index(i, j)).toString());
                xmlWriter.writeEndElement();
            }
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeEndElement();
        xmlWriter.writeEndDocument();
        file.close();
        QMessageBox::information(NULL,"show","Данные успешно сохранены");
    }
}

void LibExp::exportCSVFormat(QSqlTableModel *modelTablesForExport)
{
    QString filename = QFileDialog::getSaveFileName(this,                                                               // ДИАЛОГОВОЕ ОКНО ДЛЯ СОХРАНЕНИЯ
                                                    tr("Save csv"), ".",
                                                    tr("csv files (*.csv)"));
    QFile file(filename);
    if (file.open(QFile::WriteOnly|QFile::Truncate))
    {
        QTextStream stream(&file);
        for(int i = 0; i <= modelTablesForExport->rowCount(); i++)                                                      // КОЛИЧЕСТВО СТРОК
        {
            for (int j = 0; j < modelTablesForExport->columnCount(); ++j) {                                             // БЕГУ ПО СТОЛБЦУ
                if(j != modelTablesForExport->columnCount() - 1)   stream
                        << modelTablesForExport->data(modelTablesForExport->index(i, j)).toString()<<","<<"\t";
                else stream <<modelTablesForExport->data(modelTablesForExport->index(i, j)).toString()<<"\n";           // ЕСЛИ КОЛОНКА ПОСЛЕДНЯЯ ДОБАВИМ \n
            }
        }
        file.close();
    }
    QMessageBox::information(NULL,"show","Данные успешно сохранены");
}
