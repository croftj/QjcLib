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
#include "../QcjData/QcjDataHelpers.h"
#include "../QcjData/QcjDataStatics.h"

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
      SqlTableModel(QObject *parent, QSqlDatabase db, bool readOnly = false, const QString &xmldef = QString()) :
         QSqlTableModel(parent, db),
         m_xmldef(xmldef)
      {
         if ( readOnly ) 
         {
            m_itemFlags = Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled;
         }
         else
         {
            m_itemFlags = 0;
         }
         if ( ! m_xmldef.isEmpty())
         {
            m_fields = pFormDef->getFieldsMap(m_xmldef, nullptr);
         }
//         m_itemFlags = Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled;
//         m_itemFlags = Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
      }

      virtual Qt::ItemFlags  flags(const QModelIndex &index) const
      {
         Qt::ItemFlags rv;
//         qDebug(*log(LOG, 1)) <<  "m_itemFlags = " << m_itemFlags;
         if ( m_itemFlags == 0 ) 
         {
            QSqlRecord rec = record();
            QString fieldName = rec.fieldName(index.column());
//            qDebug(*log(LOG, 1)) <<  "Fieldname = " << fieldName;
            if ( fieldName == "id" ||
                 fieldName == "ident" ||
                 fieldName.endsWith("_id") ||
                 fieldName.endsWith("_fk") ||
                 fieldName.startsWith("sys_") ||
                 m_fields.value(fieldName).fieldType == "image"
               ) 
            {
//               qDebug(*log(LOG, 1)) <<  "NO EDIT";;
               rv = Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled;
            }
            else 
            {
//               qDebug(*log(LOG, 1)) <<  "EDIT";;
               rv = Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemIsEditable;  
            }
         }
         else 
         {
 //           qDebug(*log(LOG, 1)) <<  "Default!";;
            rv = m_itemFlags;
         }
//         qDebug(*log(LOG, 1)) <<  "rv = " << rv;
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

      QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
      {
         QcjDataFieldDef field_def;
         QSqlRecord rec = record();
         QString fieldName = rec.fieldName(index.column());
         if (m_fields.contains(fieldName))
         {
            field_def = m_fields.value(fieldName);
//            qDebug() << "Have field, role: " << role << ", name: " << fieldName << ", row: " << index.row() << ", type: " << field_def.fieldType;
            if (field_def.fieldType == "image")
            {
               QByteArray imgData = QSqlTableModel::data(index, Qt::DisplayRole).toByteArray();
               QPixmap pixmap;
//               qDebug() << "Have image, size = " << imgData.size();
               if (imgData.size() > 0)
               {
                  pixmap.loadFromData(imgData);
                  if ( field_def.height.toInt() > 0)
                  {
//                     qDebug() << "Scaling image height to :" << field_def.height.toInt();
                     pixmap = pixmap.scaledToHeight(field_def.height.toInt());
                  }
                  else if (field_def.width.toInt() > 0)
                  {
//                     qDebug() << "Scaling image width to :" << field_def.width.toInt();
                     pixmap = pixmap.scaledToWidth(field_def.width.toInt());
                  }
                  if (role == Qt::DecorationRole)
                  {
//                     qDebug() << "Returning pixmap";
                     return(QVariant(pixmap));
                  }
                  else if (role == Qt::SizeHintRole)
                  {
//                     qDebug() << "Returning size: " << pixmap.size();
                     return(QVariant(pixmap.size()));
                  }
                  else if (role == Qt::DisplayRole)
                  {
//                     qDebug() << "Returning empty string";
                     return(QVariant(QString()));
                  }
               }
            }
         }
//         qDebug() << "default processing";
         return(QSqlTableModel::data(index, role));
      }

//      QSqlRecord insertBlankRecord();

      static const QString LOG;

   private:
      QString           m_xmldef;
      QcjDataFieldMap   m_fields;
      Qt::ItemFlags     m_itemFlags;
   };
}

#endif
