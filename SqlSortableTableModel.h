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
