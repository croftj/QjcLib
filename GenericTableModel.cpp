/******************************************************************************/
/* This is free and unencumbered software released into the public domain.    */
/*                                                                            */
/* Anyone is free to copy, modify, publish, use, compile, sell, or distribute */
/* this software, either in source code form or as a compiled binary,         */
/* for any purpose, commercial or non-commercial, and by any means.           */
/*                                                                            */
/* In jurisdictions that recognize copyright laws, the author or authors of   */
/* this software dedicate any and all copyright interest in the software to   */
/* the public domain. We make this dedication for the benefit of the public   */
/* at large and to the detriment of our heirs and successors. We intend       */
/* this dedication to be an overt act of relinquishment in perpetuity of      */
/* all present and future rights to this software under copyright law.        */
/*                                                                            */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL   */
/* THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER   */
/* IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    */
/* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
/*                                                                            */
/* For more information, please refer to <http://unlicense.org/>              */
/******************************************************************************/
#include "GenericTableModel.h"
#include <QMutexLocker>

using namespace QcjLib;

const QString GenericTableModel::LOG("QcjLib_gen_tbl_model");
static LogBuilder mylog(GenericTableModel::LOG, 1, "QcjLib Generic Table Model");

QStringList GenericTableModel::Headers()
{
   QMutexLocker locker(&m_lock);
   QStringList rv;

   for (int x = 0; x < columnCount(); x++) 
   {
      rv << horizontalHeaderItem(x)->text();
   }
   return(rv);
}

int GenericTableModel::FindColumn(QString col_name)
{
   QMutexLocker locker(&m_lock);
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
   QMutexLocker locker(&m_lock);
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
   QMutexLocker locker(&m_lock);
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
   QMutexLocker locker(&m_lock);
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
   QMutexLocker locker(&m_lock);
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
   QMutexLocker locker(&m_lock);
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "Adding column with data " << col_name;
   int rv = AddColumn(col_name);
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "Adding to column " << rv << ", row " << row;
   SetValue(row, rv, text);
   return(rv);
}

bool GenericTableModel::RemoveColumn(QString col_name)
{
   QMutexLocker locker(&m_lock);
   int idx = FindColumn(col_name);
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "Removing column " << col_name << ", column num: " << idx;
   return(removeColumns(idx, 1));
}

void GenericTableModel::SetValue(int row, QString col_name, QString text)
{
   QMutexLocker locker(&m_lock);
   int col = FindColumn(col_name);
//   qDebug(*log(LOG, 1)) << __FUNCTION__ << "1 Enter col_name = " << col_name << ", col = " << col << ", row = " << row;
   SetValue(row, col, text);
}

void GenericTableModel::SetValue(int row, int col, QString text)
{
   QMutexLocker locker(&m_lock);
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "2 Enter row = " << row << ", col = " << col << ", text = " << text;
   setItem(row, col, new QStandardItem(text));
}

QString GenericTableModel::Value(int row, QString col_name)
{
   QMutexLocker locker(&m_lock);
   int col = FindColumn(col_name);
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "3 Enter row = " << row << ", col_name = " << col_name << ", col = " << col << ", rowCount() " << rowCount();
   return(Value(row, col));
}

QString GenericTableModel::Value(int row, int col)
{
   QMutexLocker locker(&m_lock);
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

