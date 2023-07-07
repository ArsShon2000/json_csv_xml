#ifndef LIBEXP_H
#define LIBEXP_H

#include "libExp_global.h"
#include "qwidget.h"
#include <QObject>
#include <QSqlTableModel>

#include <QFile>
#include <QFileDialog>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QXmlStreamWriter>
#include <QMessageBox>

//#include <filesystem>

struct Models
{
    QSqlTableModel *carsModel;
    QSqlTableModel *camerasModel;
    QSqlTableModel *employeesModel;
};

class LIBEXP_EXPORT LibExp : public QWidget
{
    Q_OBJECT
public:
    LibExp(int Extension, int table, Models models, QWidget *parent = nullptr);

private:
    QJsonObject recordColumns;                                                                                          // ДЛЯ ВНУТРЕННЕГО БЛОКА JSON
    QJsonObject recordRows;                                                                                             // ДЛЯ ВНЕШНЕГО БЛОКА JSON
    int columnsCount;                                                                                                   // ДЛЯ ПРОБЕГА ПО ГОРИЗОНТАЛИ
    int rowCount;

    void definitionTable(Models *models, int table);
    void exportJSONFormat(QSqlTableModel *modelTablesForExport);
    void exportXMLFormat(QSqlTableModel *modelTablesForExport);
    void exportCSVFormat(QSqlTableModel *modelTablesForExport);

    QSqlTableModel *modelForDefinition;
};

#endif // LIBEXP_H
