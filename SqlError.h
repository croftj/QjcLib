#ifndef SQLIB_SQL_ERROR_H
#define SQLIB_SQL_ERROR_H

#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
namespace SqlError
{
   void showError(QString operation, QWidget *parent = nullptr)
   {
      QString err("Error %1: %2");
      err = err.arg(operation).arg(pDb->lastError().text());
      qWarning() << "Database" << err;
      QMessageBox::critical(parent, "Database Error", err);
   }
}

#endif
