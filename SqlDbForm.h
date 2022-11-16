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
#ifndef SQLDBFORM_H
#define SQLDBFORM_H

# include <QAbstractTableModel>
# include <QSqlQueryModel>
# include <QFrame>
# include <QDataWidgetMapper>

# include "DbInterface.h"
# include "LogBuilder.h"
# include "YesNoComboBox.h"


namespace QcjLib
{
   class SqlDbForm : public QFrame
   {
      Q_OBJECT

   public:
      SqlDbForm(QWidget *parent);
      virtual ~SqlDbForm();

      void SetDatabase(QcjLib::DbInterface *dbLoader);
      void setQueryStatement(QString whereClause);
      QSqlQueryModel *model();
      QSqlQuery BuildUpdateQuery(QString where);
      QSqlQuery BuildInsertQuery();
      QString GetFormValue(QString field_name);
      void setCurrentModelIndex(QModelIndex index);
      bool InsertNew(bool throwErrors = true);
      QVariant GetLastInsertId()
      {
         return(m_dbInterface->GetLastInsertId(m_rawTableName));
      }

      QcjLib::DbInterface::SqlError lastError()
      {
         return m_lastError;
      }

      void setModel(QSqlQueryModel* model)
      {
         if ( m_model != NULL ) 
         {
            delete(m_model);
         }
         m_model = model;
      }

      QStringList GetFieldList()
      {
         return(m_fields);
      }

      void AddFieldValue(QString fieldName, QVariant value)
      {
         m_addedFields.insert(fieldName, value);
      }

      static const QString LOG;

   private:
      QcjLib::DbInterface*           m_dbInterface;
      QcjLib::DbInterface::SqlError  m_lastError;
      QDataWidgetMapper*                     m_formMapper;
      QMap<QString, QWidget*>                m_formWidgets;
      QMap<QString, QVariant>                m_addedFields;
      QSqlQueryModel*                        m_model;

      QStringList                            m_fields;
      QStringList                            m_rawFieldNames;
      QString                                m_order;
      QString                                m_select;
      QString                                m_table;
      QString                                m_rawTableName;
      QMap<QString, QString>                 m_relations;
   };
};

#endif
