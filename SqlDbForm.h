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
