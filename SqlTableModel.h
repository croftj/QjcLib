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
