#include "SqlSortableTableModel.h"

#include <QDebug>
#include <QSqlRecord>

#define  ASCENDING   "asc"
#define  DESCENDING  "desc"

using namespace QcjLib;

const QString SqlSortableTableModel::LOG("QcjLib_sortable_table_model");
static LogBuilder mylog(SqlSortableTableModel::LOG, 1, "QcjLib Sortable Table Model");

SqlSortableTableModel::SqlSortableTableModel(QObject *parent) :
   QSqlQueryModel(parent)
{
}

void SqlSortableTableModel::SetFilter(QString where)
{
   m_queryFilter = where;
}

Qt::SortOrder SqlSortableTableModel::SetOrder(QString field_name)
{
   FieldDescr_t fd;
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "setting sort order for column named: " << field_name << ", sorted column count = " << m_queryOrder.count();
   if ( m_queryOrder.count() > 0 ) 
   {
      if ( m_queryOrder.first().first == field_name ) 
      {
         qDebug(*log(LOG, 1)) << __FUNCTION__ << "Flipping sort order";
         fd = m_queryOrder.takeFirst();
         if ( fd.second == ASCENDING ) 
         {
            fd.second = DESCENDING;
         }
         else
         {
            fd.second = ASCENDING;
         }
      }
      else
      {
         qDebug(*log(LOG, 1)) << __FUNCTION__ << "Adding additional sort column";
         for (int x = 0; x < m_queryOrder.count(); x++) 
         {
            if ( m_queryOrder[x].first == field_name ) 
            {
               qDebug(*log(LOG, 1)) << __FUNCTION__ << "Found column already in the list, removing it";
               fd = m_queryOrder.takeAt(x);
               fd.second = ASCENDING;
               break; 
            }
         }
      }
   }
   if ( fd.first.isEmpty() ) 
   {
      fd.first = field_name;
      fd.second = ASCENDING;
   }
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "Placing column up front";
   m_queryOrder.push_front(fd);
   return((fd.second == ASCENDING) ? Qt::AscendingOrder : Qt::DescendingOrder);
}


Qt::SortOrder SqlSortableTableModel::SetOrder(int column)
{
   Qt::SortOrder rv = Qt::AscendingOrder;
   QSqlRecord rec = record();
   QString field_name = rec.fieldName(column);
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "found field " << field_name << " in column " << column;
   if ( ! field_name.isEmpty() ) 
   {
      rv = SetOrder(field_name);
   }
   return(rv);
}

void SqlSortableTableModel::ClearOrder()
{
   m_queryOrder.clear();
}

void SqlSortableTableModel::SetQuery(QString query, QSqlDatabase database)
{
   m_db = database;
   m_queryBase = query;
   m_queryFilter.clear();
   m_queryOrder.clear();
   select();
}
   
void SqlSortableTableModel::select()
{
   QString query = constructQueryString();
   setQuery(query, m_db);
   m_query = QSqlQuery(m_db);
   m_query.prepare(query);
   m_query.exec();
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "record = " << record();
}

QString SqlSortableTableModel::constructQueryString()
{ 
   QString rv = m_queryBase;
   if ( ! m_queryFilter.isEmpty() ) 
   {
      rv += " where " + m_queryFilter;
   }

   if ( m_queryOrder.count() > 0) 
   {
      rv += " order by ";
      for (int x = 0; x < m_queryOrder.count(); x++) 
      {
         if ( x > 0 ) 
         {
            rv += ", ";
         }
         rv += "\"" + m_queryOrder[x].first + "\" " + m_queryOrder[x].second;
      }
   }
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "rv = " << rv;
   return(rv);
}

