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
#include <QStandardItem>

using namespace QcjLib;

const QString GenericTableModel::LOG("QcjLib_gen_tbl_model");
static LogBuilder mylog(GenericTableModel::LOG, 1, "QcjLib Generic Table Model");

GenericTableModel::GenericTableModel(QObject *parent) :
   QStandardItemModel(parent),
   m_lock(new QMutex(QMutex::Recursive))
{};

QStringList GenericTableModel::Headers() const
{
   qDebug(*log(LOG, 1)) << "Enter";
   QMutexLocker locker(m_lock);
   QStringList rv;

   for (int x = 0; x < columnCount(); x++) 
   {
      rv << horizontalHeaderItem(x)->text();
   }
   qDebug(*log(LOG, 1)) << "Exit";
   return(rv);
}

int GenericTableModel::FindColumn(QString col_name) const
{
   qDebug(*log(LOG, 1)) << "Enter- name: " << col_name;
   QMutexLocker locker(m_lock);
   int rv = -1;
   qDebug(*log(LOG, 1))  << "columnCount = " << columnCount();
   for (rv = 0; rv < columnCount(); rv++) 
   {
      qDebug(*log(LOG, 1))  << "Testing " << col_name << " against column header " << horizontalHeaderItem(rv)->text();
      if ( horizontalHeaderItem(rv)->text().toLower() == col_name.toLower() )
      {
         qDebug(*log(LOG, 1))  << "found match- col_name: " << col_name << ", column = " << rv;
         break;
      }
   }
   qDebug(*log(LOG, 1)) << "Exit";
   return(rv);
}

int GenericTableModel::FindRow(QString col_name, QString value) const
{
   qDebug(*log(LOG, 1)) << "Enter";
   QMutexLocker locker(m_lock);
   int rv = -1;
   int col = FindColumn(col_name);
   if ( col >= 0 ) 
   {
      rv = FindRow(col, value);
   }
   qDebug(*log(LOG, 1)) << "Exit";
   return(rv);
}

int GenericTableModel::FindRow(int col, QString value) const
{
   qDebug(*log(LOG, 1)) << "Enter";
   QMutexLocker locker(m_lock);
   int rv = -1;

   for (int row = 0; rv < 0 && row < rowCount(); row++) 
   {
      if ( Value(row, col) == value ) 
      {
         rv = row;
      }
   }
   qDebug(*log(LOG, 1)) << "Exit";
   return(rv);
}

GenericTableModel::ModelRow_t GenericTableModel::GetRow(int row) const
{
   qDebug(*log(LOG, 1)) << "Enter";
   QMutexLocker locker(m_lock);
   ModelRow_t rv;

   QStringList fields = Headers();
   foreach (QString field, fields)
   {
      rv.insert(field, Value(row, field));
   }
   qDebug(*log(LOG, 1)) << "Exit";
   return(rv);
}

int GenericTableModel::AddColumn(QString col_name, QString data_name)
{
   qDebug(*log(LOG, 1)) << "Enter";
   QMutexLocker locker(m_lock);
   QRegExp re(": *$");
   col_name = col_name.replace(re, "");
   int rv = FindColumn(col_name);
   if ( rv < 0 ) 
   {
      qDebug(*log(LOG, 1))  << "New column, setting the new colCount()";
      rv = columnCount();
      setColumnCount(rv + 1);
   }
   qDebug(*log(LOG, 1))  << "Adding column named " << col_name << " to column " << rv;
   QStandardItem *hdr_item = new QStandardItem(col_name);
   hdr_item->setData(QVariant(data_name));
   setHorizontalHeaderItem(rv, hdr_item);
   qDebug(*log(LOG, 1)) << "Exit";
   return(rv);
}

int GenericTableModel::AddColumn(int row, QString col_name, QString text, QString data_name)
{
   qDebug(*log(LOG, 1)) << "Enter";
   QMutexLocker locker(m_lock);
   qDebug(*log(LOG, 1))  << "Adding column with data " << col_name;
   int rv = AddColumn(col_name, data_name);
   qDebug(*log(LOG, 1))  << "Adding to column " << rv << ", row " << row;
   SetValue(row, rv, text);
   qDebug(*log(LOG, 1)) << "Exit";
   return(rv);
}

bool GenericTableModel::RemoveColumn(QString col_name)
{
   qDebug(*log(LOG, 1)) << "Enter";
   QMutexLocker locker(m_lock);
   int idx = FindColumn(col_name);
   qDebug(*log(LOG, 1))  << "Removing column " << col_name << ", column num: " << idx;
   qDebug(*log(LOG, 1)) << "Exit";
   return(removeColumns(idx, 1));
}

void GenericTableModel::SetValue(int row, QString col_name, QString text)
{
   qDebug(*log(LOG, 1)) << "Enter";
   QMutexLocker locker(m_lock);
   int col = FindColumn(col_name);
//   qDebug(*log(LOG, 1))  << "1 Enter col_name = " << col_name << ", col = " << col << ", row = " << row;
   SetValue(row, col, text);
}

void GenericTableModel::SetValue(int row, int col, QString text)
{
   qDebug(*log(LOG, 1))  << "2 Enter row = " << row << ", col = " << col << ", text = " << text;
//   QMutexLocker locker(m_lock);
   qDebug(*log(LOG, 1))  << "have lock";
   QStandardItem *item = new QStandardItem(text);
   Qt::Alignment align = static_cast<Qt::Alignment>(data(index(row, col), Qt::TextAlignmentRole).toInt());
   qDebug(*log(LOG, 1)) << "Alignment = " << align;
   item->setTextAlignment(align);
   setItem(row, col, item);
}

QString GenericTableModel::Value(int row, QString col_name) const
{
   qDebug(*log(LOG, 1)) << "Enter";
   QMutexLocker locker(m_lock);
   int col = FindColumn(col_name);
   qDebug(*log(LOG, 1))  << "3 Enter row = " << row << ", col_name = " << col_name << ", col = " << col << ", rowCount() " << rowCount();
   qDebug(*log(LOG, 1)) << "Exit";
   return(Value(row, col));
}

QString GenericTableModel::Value(int row, int col) const
{
   qDebug(*log(LOG, 1)) << "Enter";
   QMutexLocker locker(m_lock);
   QStandardItem *val_item = item(row, col);
   if ( val_item != NULL ) 
   {
      qDebug(*log(LOG, 1)) << "Exit";
      return(item(row, col)->text());
   }
   else 
   {
      qDebug(*log(LOG, 1)) << "Exit";
      return(QString("empty"));
   }
}

QString GenericTableModel::ColumnName(int col) const
{
   QStandardItem *hdr_item = horizontalHeaderItem(col);
   return(hdr_item->text());
}

QString GenericTableModel::ColumnDataName(int col) const
{
   QStandardItem *hdr_item = horizontalHeaderItem(col);
   return(hdr_item->data().toString());
}

QString GenericTableModel::ColumnDataName(QString &col_name) const
{
   int col;
   if ((col = FindColumn(col_name)) >= 0)
   {
      QStandardItem *hdr_item = horizontalHeaderItem(col);
      return(hdr_item->data().toString());
   }
   return(QString());
}

int GenericTableModel::appendBlankRow()
{
   int rv = rowCount();
   for(int col = 0; col < columnCount(); col++)
   {
      SetValue(rv, col, QString(""));
   }
   return(rv);
}
