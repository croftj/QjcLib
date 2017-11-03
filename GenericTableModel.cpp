#include "GenericTableModel.h"

using namespace QcjLib;

const QString GenericTableModel::LOG("QcjLib_gen_tbl_model");
static LogBuilder mylog(GenericTableModel::LOG, 1, "QcjLib Generic Table Model");

QStringList GenericTableModel::Headers()
{
   QStringList rv;

   for (int x = 0; x < columnCount(); x++) 
   {
      rv << horizontalHeaderItem(x)->text();
   }
   return(rv);
}

int GenericTableModel::FindColumn(QString col_name)
{
   int rv = -1;
//   qDebug(*log(LOG, 1)) << __FUNCTION__ << "columnCount = " << columnCount();
   for (rv = 0; rv < columnCount(); rv++) 
   {
//      qDebug(*log(LOG, 1)) << __FUNCTION__ << "Testing " << col_name << " against column header " << horizontalHeaderItem(rv)->text();
      if ( horizontalHeaderItem(rv)->text().toLower() == col_name.toLower() )
      {
         qDebug(*log(LOG, 1)) << __FUNCTION__ << "found match- col_name: " << col_name << ", column = " << rv;
         break;
      }
   }
   return(rv);
}

int GenericTableModel::FindRow(QString col_name, QString value)
{
   int rv = -1;
   int col = FindColumn(col_name);
   if ( col >= 0 ) 
   {
      rv = FindRow(col, value);
   }
   return(rv);
}

int GenericTableModel::FindRow(int col, QString value)
{
   int rv = -1;

   for (int row = 0; rv < 0 && row < rowCount(); row++) 
   {
      if ( Value(row, col) == value ) 
      {
         rv = row;
      }
   }
   return(rv);
}

GenericTableModel::ModelRow_t GenericTableModel::GetRow(int row)
{
   ModelRow_t rv;

   QStringList fields = Headers();
   foreach (QString field, fields)
   {
      rv.insert(field, Value(row, field));
   }
   return(rv);
}

int GenericTableModel::AddColumn(QString col_name)
{
   QRegExp re(": *$");
   col_name = col_name.replace(re, "");
   int rv = FindColumn(col_name);
   if ( rv < 0 ) 
   {
      qDebug(*log(LOG, 1)) << __FUNCTION__ << "New column, setting the new colCount()";
      rv = columnCount();
      setColumnCount(rv + 1);
   }
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "Adding column named " << col_name << " to column " << rv;
   setHorizontalHeaderItem(rv, new QStandardItem(col_name));
   return(rv);
}

int GenericTableModel::AddColumn(int row, QString col_name, QString text)
{
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "Adding column with data " << col_name;
   int rv = AddColumn(col_name);
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "Adding to column " << rv << ", row " << row;
   SetValue(row, rv, text);
   return(rv);
}

bool GenericTableModel::RemoveColumn(QString col_name)
{
   int idx = FindColumn(col_name);
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "Removing column " << col_name << ", column num: " << idx;
   return(removeColumns(idx, 1));
}

void GenericTableModel::SetValue(int row, QString col_name, QString text)
{
   int col = FindColumn(col_name);
//   qDebug(*log(LOG, 1)) << __FUNCTION__ << "1 Enter col_name = " << col_name << ", col = " << col << ", row = " << row;
   SetValue(row, col, text);
}

void GenericTableModel::SetValue(int row, int col, QString text)
{
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "2 Enter row = " << row << ", col = " << col << ", text = " << text;
   setItem(row, col, new QStandardItem(text));
}

QString GenericTableModel::Value(int row, QString col_name)
{
   int col = FindColumn(col_name);
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "3 Enter row = " << row << ", col_name = " << col_name << ", col = " << col << ", rowCount() " << rowCount();
   return(Value(row, col));
}

QString GenericTableModel::Value(int row, int col)
{
   QStandardItem *val_item = item(row, col);
   if ( val_item != NULL ) 
   {
      return(item(row, col)->text());
   }
   else 
   {
      return(QString("empty"));
   }
}

