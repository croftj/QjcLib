/******************************************************************************/
/* $Id$                              */
/*                                                                            */
/* Author: Joe Croft, joe@croftj.net 2023-2024                                     */
/*                                                                            */
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
#ifndef SQLSORTABLETABLEMDEL_H
#define SQLSORTABLETABLEMDEL_H

#include "LogBuilder.h"

#include <QString>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>

namespace QcjLib
{
   class SqlSortableTableModel : public QSqlQueryModel
   {
      Q_OBJECT

   public:
      typedef QPair<QString, QString> FieldDescr_t;

      SqlSortableTableModel(QObject *parent = NULL);

      void SetFilter(QString where);
      Qt::SortOrder SetOrder(QString field_name);
      Qt::SortOrder SetOrder(int column);
      void ClearOrder();
      void SetQuery(QString query, QSqlDatabase database = QSqlDatabase());
      void select();

      static const QString LOG;

   protected:
      QString constructQueryString();

   private:
      QString              m_queryBase;
      QString              m_queryFilter;
      QList<FieldDescr_t>  m_queryOrder;
      QSqlDatabase         m_db;
      QSqlQuery            m_query;
   };
}

#endif
