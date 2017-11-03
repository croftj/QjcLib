# include "SqlDbForm.h"

# include <QComboBox>
# include <QDebug>
# include <QDoubleSpinBox>
# include <QSqlRecord>
# include <QSqlQuery>
# include <QLineEdit>
# include <QWidget>
# include <QTextEdit>

# include "SqlDbFormDelegate.h"

using namespace QcjLib;

const QString SqlDbForm::LOG("QcjLib_db_form");
static LogBuilder mylog(SqlDbForm::LOG, 3, "QcjLib Database Form");

SqlDbForm::SqlDbForm(QWidget *parent) : QFrame(parent),
                                        m_model(NULL)
{
   qDebug(*log(LOG, 3)) << __FUNCTION__ << "Enter: this = " << (unsigned long)this;
   qDebug(*log(LOG, 3)) << __FUNCTION__ << "Exit";
}

SqlDbForm::~SqlDbForm()
{
}

void SqlDbForm::SetDatabase(DbInterface *dbLoader)
{
   QMap<int, QString>      forder;

   qDebug(*log(LOG, 1)) << __FUNCTION__ << "Enter: " << (unsigned long)this;
   m_dbInterface = dbLoader;

   m_formMapper = new QDataWidgetMapper(this);
   qDebug(*log(LOG, 2)) << __FUNCTION__ << "QDataWidgetMapper submit policy: " << m_formMapper->submitPolicy();
   m_fields.clear();
   m_rawFieldNames.clear();

   QString table = property("sql_table_name").toString();
   qDebug(*log(LOG, 1)) << __FUNCTION__ << ": Table: " << table;
   m_rawTableName = table;
   m_table = m_dbInterface->GetTableName(table);
   qDebug(*log(LOG, 1)) << __FUNCTION__ << ": xlated table name: " << m_table;
   QString sql = "select * from " + m_table;
   qDebug(*log(LOG, 1)) << __FUNCTION__ << ": sql = " << sql;
   QSqlQuery q1(m_dbInterface->Database());
   q1.prepare(sql);
   q1.exec();
   q1.next();
   qDebug(*log(LOG, 1)) << __FUNCTION__ << ": Executed query: " << q1.executedQuery();
   qDebug(*log(LOG, 2)) << __FUNCTION__ << ": Error reading query: " <<
               q1.lastError().databaseText() <<
               " - " <<
               q1.lastError().driverText();
   QSqlRecord tableRecord = q1.record();
   qDebug(*log(LOG, 1)) << __FUNCTION__ << ": have " << tableRecord.count() << " record fields";
   if ( tableRecord.count() > 0 ) 
   {
      qDebug(*log(LOG, 1)) << __FUNCTION__ << ": have " << tableRecord.count() << " record fields";
      QObjectList childList = children();
      qDebug(*log(LOG, 1)) << __FUNCTION__ << ": found " << childList.count() << " child widgets";
      
      int column = 0;
      foreach (QObject *child, childList)
      {
         QWidget *cw = dynamic_cast<QWidget*>(child);
         if ( cw != 0 ) 
         {
            QVariant field          = cw->property("sql_field_name");
            QVariant primary        = cw->property("primary_field");
            QVariant sort_order     = cw->property("sort_order");
            QVariant force_enabled  = cw->property("force_enabled");
            qDebug(*log(LOG, 1)) << __FUNCTION__ << ": found widget name: " << field << ", index = " << tableRecord.indexOf(field.toString());

            QString fname = field.toString();
            QStringList sl = fname.split("@");
            QString rawName = sl[0];
            fname = m_dbInterface->GetIdName(rawName);
            if ( field.isValid() && tableRecord.indexOf(fname) >= 0 ) 
            {
               qDebug(*log(LOG, 1)) << __FUNCTION__ << ": fname: " << fname;
               
               if ( sl.count() > 1 ) 
               {
                  qDebug(*log(LOG, 1)) << __FUNCTION__ << ": adding relation: " << sl[1];
                  m_relations.insert(fname, sl[1]);
               }

               if ( primary.isValid() && primary.toBool() ) 
               {
                  qDebug(*log(LOG, 1)) << __FUNCTION__ << ": Prepending field: " << fname;
                  m_fields.prepend(fname);
               }
               else 
               {
                  qDebug(*log(LOG, 1)) << __FUNCTION__ << ": Appending field: " << fname;
                  m_fields.append(fname);
               }
               m_formWidgets.insert(fname, cw);
               qDebug(*log(LOG, 1)) << __FUNCTION__ << ": Appending raw field name: " << rawName;
               m_rawFieldNames.append(rawName);

               if ( sort_order.isValid() ) 
               {
                  forder.insert(sort_order.toInt(), fname);
               }
            }
            else if ( ! force_enabled.toBool() )
            {
               cw->setEnabled(false);
            }
         }
         column++;
      }
      if ( m_fields.count() > 0 ) 
      {
         m_select = "select " + m_fields.join(", ") + " from " + m_table + " ";
         qDebug(*log(LOG, 1)) << __FUNCTION__ << ": m_select = " << m_select;
         m_order.clear();
         foreach (int priority, forder.keys())
         {
            if ( m_order.size() > 0 )
            {
               m_order += ", ";
            }
            m_order += forder[priority];
         }
         if ( m_order.size() > 0 ) 
         {
            m_order.prepend(" order by ");
         }
      }
   }
   m_model = new QSqlQueryModel(this);
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "Exit";
}

/***********************************************************************************************************/
/*   This  member  function  builds the select statement and saves it in the model. If whereClause is not  */
/*   empty, it is added to the select statement. If the form widgets have not been mapped to the model by  */
/*   a previous call to this function, they will be mapped.                                                */
/***********************************************************************************************************/
void SqlDbForm::setQueryStatement(QString whereClause)
{
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "Enter: " << (unsigned long)this;
   qDebug(*log(LOG, 1)) << __FUNCTION__ << ": m_select = " << m_select;
   QString sql = m_select;
   if ( whereClause.size() > 0 ) 
   {
      sql += " " + whereClause;
   }                         
   if ( m_order.size() > 0 ) 
   {
      sql += " " + m_order;
   }
   sql = m_dbInterface->AdjustSqlNames(sql);

   m_model->setQuery(sql, m_dbInterface->Database());

   /***********************************************/
   /*   If we have not already mapped the widgets */
   /*   to the model, then do it now              */
   /***********************************************/
   m_formMapper->setModel(m_model);
   QMap<QString, QWidget*>::const_iterator it =  m_formWidgets.constBegin();
   if ( it != m_formWidgets.constEnd() && 
        m_formMapper->mappedSection(it.value()) == -1) 
   {
      qDebug(*log(LOG, 1)) << __FUNCTION__ << "Mapping widgets!";
      SqlDbFormDelegate *delegate = new SqlDbFormDelegate(this);
      m_formMapper->setItemDelegate(delegate);

      QSqlRecord rec = m_model->record();
      while ( it != m_formWidgets.constEnd() )
      {
         qDebug(*log(LOG, 1)) << __FUNCTION__ << ": Index of field " << it.key() << ": " << rec.indexOf(it.key());
         if ( m_relations.contains(it.key()) && it.value()->inherits("QComboBox") ) 
         {
            qDebug(*log(LOG, 1)) << __FUNCTION__ << "Have relational combobox";
            QComboBox *wdt = dynamic_cast<QComboBox*>(it.value());
            wdt->clear();
            QString sql = m_relations.value(it.key());
            sql = m_dbInterface->AdjustSqlNames(sql);
            qDebug(*log(LOG, 1)) << __FUNCTION__ << "sql = " << sql;
            QSqlQuery q1(m_dbInterface->Database());
            q1.prepare(sql);
            q1.exec();
            while (q1.next())
            {
               qDebug(*log(LOG, 1)) << __FUNCTION__ << "text = " << q1.value(1) << ", value = " << q1.value(0);
               wdt->addItem(q1.value(1).toString(), q1.value(0).toInt());
            }
         }
         qDebug(*log(LOG, 1)) << __FUNCTION__ << "mapping Object named" << it.value()->objectName() << " to field " << it.key();
         m_formMapper->addMapping(it.value(), rec.indexOf(it.key()));
         ++it;
      }         
   }
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "Exit";
}

QSqlQueryModel *SqlDbForm::model()
{
   qDebug(*log(LOG, 2)) << __FUNCTION__ << "Enter";
   qDebug(*log(LOG, 2)) << __FUNCTION__ << "Exit";
   return(m_model);
}

void SqlDbForm::setCurrentModelIndex(QModelIndex index)
{
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "Enter- form = " << this;
   if ( index.isValid() ) 
   {
      qDebug(*log(LOG, 1)) << __FUNCTION__ << "Index is valid: " << index;
      setEnabled(true);
      m_formMapper->setCurrentModelIndex(index);
      foreach(QWidget *wdt, m_formWidgets)
      {
         if ( wdt->inherits("QLineEdit")  ) 
         {
            if ( ! static_cast<QLineEdit*>(wdt)->isReadOnly() )
            {
               static_cast<QLineEdit*>(wdt)->setReadOnly(! m_dbInterface->IsAdvancedUser());
            }
         }
         else if ( wdt->inherits("QTextEdit")  ) 
         {
            if ( ! static_cast<QTextEdit*>(wdt)->isReadOnly() )
            {
               static_cast<QTextEdit*>(wdt)->setReadOnly(! m_dbInterface->IsAdvancedUser());
            }
         }
         else if ( wdt->inherits("QDoubleSpinBox")  ) 
         {
            if ( ! static_cast<QDoubleSpinBox*>(wdt)->isReadOnly() )
            {
               static_cast<QDoubleSpinBox*>(wdt)->setReadOnly(! m_dbInterface->IsAdvancedUser());
            }
         }
         else if ( wdt->inherits("QSpinBox")  ) 
         {
            if ( ! static_cast<QSpinBox*>(wdt)->isReadOnly() )
            {
               static_cast<QSpinBox*>(wdt)->setReadOnly(! m_dbInterface->IsAdvancedUser());
            }
         }
         else if ( wdt->inherits("QComboBox")  ) 
         {
            if ( static_cast<QComboBox*>(wdt)->isEnabled() )
            {
               qDebug(*log(LOG, 1)) << __FUNCTION__ << "Disabling combobox";
               static_cast<QComboBox*>(wdt)->setEnabled(m_dbInterface->IsAdvancedUser());
            }
         }
      }
   }
   else 
   {
      qDebug(*log(LOG, 1)) << __FUNCTION__ << "Index is INVALID!!";
      setEnabled(false);
   }
}

QString SqlDbForm::GetFormValue(QString field_name)
{
   QString rv;
   QWidget *wdt;

   if ( m_formWidgets.contains(field_name) ) 
   {
      wdt = m_formWidgets.value(field_name);
      if ( wdt->inherits("QLineEdit")  ) 
      {
         qDebug(*log(LOG, 1)) << __FUNCTION__ << "field " << field_name << " is a QLineEdit";
         rv = static_cast<QLineEdit*>(wdt)->text();         
      }
      else if ( wdt->inherits("QTextEdit")  ) 
      {
         qDebug(*log(LOG, 1)) << __FUNCTION__ << "field " << field_name << " is a QTextEdit";
         rv = static_cast<QTextEdit*>(wdt)->toPlainText();         
      }
      else if ( wdt->inherits("QDoubleSpinBox")  ) 
      {
         qDebug(*log(LOG, 1)) << __FUNCTION__ << "field " << field_name << " is a QDoubleSpinBox";
         rv = static_cast<QDoubleSpinBox*>(wdt)->cleanText();         
      }
      else if ( wdt->inherits("QSpinBox")  ) 
      {
         qDebug(*log(LOG, 1)) << __FUNCTION__ << "field " << field_name << " is a QSpinBox";
         rv = static_cast<QSpinBox*>(wdt)->cleanText();         
      }
      else if ( wdt->inherits("QComboBox")  ) 
      {
         qDebug(*log(LOG, 1)) << __FUNCTION__ << "field " << field_name << " is a QComboBox";
         rv = static_cast<QComboBox*>(wdt)->itemData(static_cast<QComboBox*>(wdt)->currentIndex()).toString();         
      }
   }
   return(rv);
}

QSqlQuery SqlDbForm::BuildUpdateQuery(QString where)
{
   QSqlQuery rv(m_dbInterface->Database());
   
   QString bind_name;
   QString field_list;

   QString sql = "update " + m_table + " set ";

   foreach(QString field, m_rawFieldNames)
   {
      if ( ! field.endsWith("_id") && ! field.startsWith("sys_") ) 
      {
         qDebug(*log(LOG, 1)) << __FUNCTION__ << "adding field: " << field;
         if ( ! field_list.isEmpty() ) 
         {
            field_list += ", ";
         }
         field_list += field + " = :" + field;
      }
   }
   sql += field_list + " " + where;
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "sql = " << sql;
   sql = m_dbInterface->AdjustSqlNames(sql);
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "sql = " << sql;
   rv.prepare(sql);

   foreach(QString field, m_rawFieldNames)
   {
      if ( ! field.startsWith("sys_") ) 
      {
         QString value;
         value = GetFormValue(m_dbInterface->GetIdName(field));
         bind_name = field;
         if ( field.endsWith("_id") ) 
         {
            bind_name = "id";
         }
         bind_name = ":" + bind_name;
         qDebug(*log(LOG, 1)) << __FUNCTION__ << "binding value: " << value << " to " << bind_name;
         rv.bindValue(bind_name, value);
      }
   }
   return(rv);
}

QSqlQuery SqlDbForm::BuildInsertQuery()
{
   QSqlQuery rv(m_dbInterface->Database());
   
   QString bind_name;
   QString field_list;
   QString value_list;

   QString sql = "insert into " + m_table + " (";

   foreach(QString field, m_rawFieldNames)
   {
      if ( ! field.endsWith("_id") ) 
      {
         qDebug(*log(LOG, 1)) << __FUNCTION__ << "adding field: " << field;
         if ( ! field_list.isEmpty() ) 
         {
            field_list += ", ";
         }
         field_list += field;

         if ( ! value_list.isEmpty() ) 
         {
            value_list += ", ";
         }
         value_list += ":" + field;
      }
   }
   foreach(QString field, m_addedFields.keys())
   {
      if ( ! field.endsWith("_id") ) 
      {
         qDebug(*log(LOG, 1)) << __FUNCTION__ << "adding field: " << field;
         if ( ! field_list.isEmpty() ) 
         {
            field_list += ", ";
         }
         field_list += field;

         if ( ! value_list.isEmpty() ) 
         {
            value_list += ", ";
         }
         value_list += ":" + field;
      }
   }
   
   sql += field_list + ") values (" + value_list + ")";
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "sql = " << sql;
   sql = m_dbInterface->AdjustSqlNames(sql);
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "sql = " << sql;
   rv.prepare(sql);

   foreach(QString field, m_rawFieldNames)
   {
      QString value;
      value = GetFormValue(m_dbInterface->GetIdName(field));
      bind_name = field;
      if ( field.endsWith("_id") ) 
      {
         bind_name = "id";
      }
      bind_name = ":" + bind_name;
      qDebug(*log(LOG, 1)) << __FUNCTION__ << "binding value: " << value << " to " << bind_name;
      rv.bindValue(bind_name, value);
   }
   foreach(QString field, m_addedFields.keys())
   {
      QVariant value;
      value = m_addedFields.value(field);
      bind_name = ":" + field;
      qDebug(*log(LOG, 1)) << __FUNCTION__ << "binding value: " << value << " to " << bind_name;
      rv.bindValue(bind_name, value);
   }
   return(rv);
}


bool SqlDbForm::InsertNew(bool throwErrors)
{
   bool rv;

   QSqlQuery q1 = BuildInsertQuery();
   if ( ! (rv = q1.exec()) ) 
   {
      m_lastError.Message = "Error inserting record into " + m_table;
      m_lastError.Error = q1.lastError();
      m_lastError.Statement = q1.executedQuery();
      if ( throwErrors ) 
      {
         throw(m_lastError);
      }
   }
   return(rv);
}
