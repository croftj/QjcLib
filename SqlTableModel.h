#ifndef SQLTABLEMODEL
#define SQLTABLEMODEL

#include "LogBuilder.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>

namespace QcjLib
{
   class SqlTableModel : public QSqlTableModel
   {
      Q_OBJECT

   public:
      SqlTableModel(QObject *parent, QSqlDatabase db, bool readOnly = false) :
         QSqlTableModel(parent, db)
      {
         if ( readOnly ) 
         {
            m_itemFlags = Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled;
         }
         else
         {
            m_itemFlags = 0;
         }
//         m_itemFlags = Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled;
//         m_itemFlags = Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
      }

      virtual Qt::ItemFlags  flags(const QModelIndex &index) const
      {
         Qt::ItemFlags rv;
         qDebug(*log(LOG, 1)) << __FUNCTION__ << "m_itemFlags = " << m_itemFlags;
         if ( m_itemFlags == 0 ) 
         {
            QSqlRecord rec = record();
            QString fieldName = rec.fieldName(index.column());
            qDebug(*log(LOG, 1)) << __FUNCTION__ << "Fieldname = " << fieldName;
            if ( fieldName == "id" ||
                 fieldName.endsWith("_id") ||
                 fieldName.endsWith("_fk") ||
                 fieldName.startsWith("sys_")
               ) 
            {
               qDebug(*log(LOG, 1)) << __FUNCTION__ << "NO EDIT";;
               rv = Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled;
            }
            else 
            {
               qDebug(*log(LOG, 1)) << __FUNCTION__ << "EDIT";;
               rv = Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemIsEditable;  
            }
         }
         else 
         {
            qDebug(*log(LOG, 1)) << __FUNCTION__ << "Default!";;
            rv = m_itemFlags;
         }

         qDebug(*log(LOG, 1)) << __FUNCTION__ << "rv = " << rv;
         return(rv);
      }

      void setQuery(const QSqlQuery q)
      {
         QSqlTableModel::setQuery(q);
      }

   
      void setFlags(Qt::ItemFlags flags)
      {
         m_itemFlags = flags;
      }

      static const QString LOG;
      
   private:
      Qt::ItemFlags  m_itemFlags;
   };
}

#endif
