/******************************************************************************/
/* $Id$                              */
/*                                                                            */
/* Author: Joe Croft, joe@croftj.net 2023                                     */
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
/*********************************************************************************
**
**   $Id$
**   Copyright (c) 2007 Joe Croft <joe@croftj.net>
**   
**   This file is part of QcjData Class Libraries.
**
**   QcjLib Class Libraries is free software; you can redistribute it and/or modify
**   it under the terms of the GNU General Public License as published by
**   the Free Software Foundation; either version 2 of the License, or
**   (at your option) any later version.
**
**   QcjLib Class Libraries is distributed in the hope that it will be useful,
**   but WITHOUT ANY WARRANTY; without even the implied warranty of
**   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**   GNU General Public License for more details.
**
**   You should have received a copy of the GNU General Public License
**   along with QcjLib Class Libraries; if not, write to the Free Software
**   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**
*********************************************************************************/
#include <QBuffer>
#include <QCryptographicHash>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QStringList>
#include <QWidget>
#include "DataWidgets.h"
#include "WidgetUtils.h"
#include "QcjData/QcjDataHelpers.h"
#include "QcjData/QcjDataLogin.h"
#include "QcjData/QcjDataStatics.h"
#include "QcjData/QcjPhotoSelect.h"
#include "QcjLib/CameraCaptureDialog.h"
#include "QcjLib/SqlError.h"

//#define NO_PHOTO_SELECT

using namespace QcjLib;

namespace
{
   QString DELETE_SQL = { QStringLiteral("delete from %1 where %2") };
   QString SELECT_SQL = { QStringLiteral("select %1 from %2 where %3 for update nowait") };
   QString INSERT_SQL = { QStringLiteral("insert into %1 (%2) overriding user value values (%3) returning *") };
   QString UPDATE_SQL = { QStringLiteral("update %1 set %2 where %3") };

   QString UNSAVED_CHANGES_TITLE = { QStringLiteral("Unsaved Changes") };
   QString UNSAVED_CHANGES_MSG   = { QStringLiteral("The form has unsaved changes.<br>"
                                                    "Do you want to Save them or Discard them"  
                                                   ) };

   QString parseInitString(QString init)
   {
      QString rv;

      qDebug() << "init: " << init;
      QStringList sl = init.split(':');
      if (sl.count() == 1)
      {
         rv = sl[0];
      }
      else
      {
         if (sl[0] == "config")
         {
            rv = pConfig->value(sl[1]).toString();
            qDebug() << "rv: " << rv;
         }
         else
         {
            qDebug() << sl[0] << " is not implimented yet";
         }
      }
      return(rv);
   }
}

const QString DataWidget::LINE_EDIT       = {QStringLiteral("lineedit")};
const QString DataWidget::TEXT_EDIT       = {QStringLiteral("textedit")};
const QString DataWidget::PHONE_EDIT      = {QStringLiteral("phone")};
const QString DataWidget::STRING_SELECT   = {QStringLiteral("strsel")};
const QString DataWidget::INTEGER_SPIN    = {QStringLiteral("integer")};
const QString DataWidget::DOUBLE_SPIN     = {QStringLiteral("double")};
const QString DataWidget::MONEY_EDIT      = {QStringLiteral("money")};
const QString DataWidget::IMAGE_EDIT      = {QStringLiteral("image")};
const QString DataWidget::YESNO_SELECT    = {QStringLiteral("yesno")};
const QString DataWidget::CHECKBOX        = {QStringLiteral("checkbox")};
const QString DataWidget::DATE_EDIT       = {QStringLiteral("date")};
const QString DataWidget::TIME_EDIT       = {QStringLiteral("time")};
const QString DataWidget::DATE_TIME_EDIT  = {QStringLiteral("timestamp")};

/*!
\class QcjLib::AutoDataForm

   This class is an auto populating QcjLib::DataForm that 
   creates the desired form elements based off an XML form definirion
*/
AutoDataForm::AutoDataForm(QWidget *parent) : DataForm(parent),
                                              m_inTransaction(false)
{
}

bool AutoDataForm::cancel()
{
   bool rv = DataForm::cancel();
   if (rv)
   {
      rollbackTransaction();
      clearForm();
   }
   return(rv);
}


void AutoDataForm::clearForm()
{
   DataForm::clearForm();
}

void AutoDataForm::beginTransaction()
{
   if ( ! m_inTransaction)
   {
      m_inTransaction = true;
      qDebug() << "calling transaction";
      qDebug() << objectName() << "m_db = " << m_db;
      m_db.transaction();
      qDebug() << "back";
   }
}

void AutoDataForm::commitTransaction(bool emit_updated)
{
   if (m_inTransaction)
   {
      m_inTransaction = false;
      m_db.commit();
      if (emit_updated)
      {
         emit(updated());
      }
   }
}

void AutoDataForm::rollbackTransaction()
{
   if (m_inTransaction)
   {
      m_inTransaction = false;
      m_db.rollback();
      emit(updated());
   }
}

void AutoDataForm::setDatabase()
{
   QcjDataLogin login;
   m_db = login.database(readXmldef());

   qDebug() << objectName() << "pDb = " << (unsigned long)pDb << ", m_db = " << m_db;
   QGridLayout *gl = new QGridLayout(this);
   gl->setSpacing(3);
   gl->activate();

   m_indexName = pFormDef->getIndexField(m_xmldef);
   m_tableName = pFormDef->getTable(m_xmldef);
   qDebug() << "m_tableName = " << m_tableName;
   m_model.setTable(m_tableName);
   QSizePolicy lblPolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
   QSizePolicy wdtPolicySingle(QSizePolicy::Minimum, QSizePolicy::Maximum);
   QSizePolicy wdtPolicyMulti(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
#ifndef NO_PHOTO_SELECT
   QSizePolicy wdtPolicyPhoto(QSizePolicy::Preferred, QSizePolicy::Preferred);
#endif

   QcjDataFieldStdVector field_defs = pFormDef->getFields(m_xmldef, nullptr);
   foreach(const QcjDataFieldDef &f_def, field_defs)
   {
      int col = f_def.col * 2;
      int row = f_def.row;
      if (f_def.dataName == "--ENDOFFIELDS--")
      {
         break;
      }
      qDebug() << "Calling widgetFactory for field: " << f_def.dataName;
      WidgetUtils::fieldDefToString(f_def);
      DataWidget *wdt = DataWidget::widgetFactory(f_def, m_xmldef, this);
      QLabel *lbl = new QLabel(f_def.label, this);
      lbl->setSizePolicy(lblPolicy);
      if (f_def.rowSpan <= 1)
      {
         gl->addWidget(lbl, row, col);
      }
      else
      {
         int rows = (f_def.rowSpan == 1) ? 0 : f_def.rowSpan;
         gl->addWidget(lbl, row, col, rows, 1);
      }
      if ( f_def.colSpan > 1 || f_def.rowSpan > 1) 
      {
         int rows = (f_def.rowSpan <= 1) ? 1 : f_def.rowSpan;
         int cols = (f_def.colSpan <= 1) ? 1 : f_def.colSpan;
         if (f_def.rowSpan > 1)
         {
            wdt->setSizePolicy(wdtPolicyMulti);
         }
         else
         {
            wdt->setSizePolicy(wdtPolicySingle);
         }
         gl->addWidget(wdt->widget(), row, col * 2 + 1, rows, cols > 1 ? cols * 2 : 1);
      }
      else
      {
         gl->addWidget(wdt->widget(), f_def.row, (f_def.col * 2) + 1, 1, 1);
      }
#ifndef NO_PHOTO_SELECT
      if (QString(wdt->widget()->metaObject()->className()) == QString("PhotoEntry")) 
      {
         wdt->setSizePolicy(wdtPolicyPhoto);
      }
#endif      
   }
   qDebug() << "Calling DataForm::setDatabase()";
   DataForm::setDatabase();
   qDebug() << "exit";
}

QVariant AutoDataForm::getFieldValue(const QString &name) const
{
   return(m_record.value(name));
}

void AutoDataForm::setFieldValue(const QString &name, const QVariant &value)
{
   m_record.setValue(name, value);
   m_model.setRecord(0, m_record);
}

void AutoDataForm::setReadOnly(bool flag)
{
   QList<QWidget*> widgets = findChildren<QWidget*>();
   qDebug() << "Enter, " << widgets.count() << "widgets";
   foreach (QWidget *wdt, widgets)
   {
      qDebug() << "Widget name: " << wdt->objectName();
      DataWidget *data_wdt = dynamic_cast<DataWidget*>(wdt);
      if (data_wdt != nullptr)
      {
         qDebug() << "is data widget";
         if (data_wdt->getFieldDef().ro != true)
         {
            data_wdt->setReadOnly(flag);
         }
      }
      qDebug() << "Next";
   }
   qDebug() << "exit";
}

bool AutoDataForm::validateSave()
{
   int btn = QMessageBox::question(this, UNSAVED_CHANGES_TITLE,
                        UNSAVED_CHANGES_MSG,
                        QMessageBox::Save | QMessageBox::Discard,
                        QMessageBox::Save);
   return(btn == QMessageBox::Save);
}

void AutoDataForm::deleteRecord()
{
   QString fields;
   QString filter = pFormDef->getWhereClause(m_xmldef, &m_record, &m_db);

   qDebug() << "Have filter: " << filter;
   QString sql(DELETE_SQL);
   sql = sql.arg(m_model.tableName())
            .arg(filter);
   qDebug() << "sql: " << sql;
   QSqlQuery q1;
   q1.prepare(sql);
   if ( ! q1.exec())
   {
      SqlError::showError("deleting record", this);
      rollbackTransaction();
      return;
   }
   emit(updated());
}

QString AutoDataForm::getIndexFilter()
{
   qDebug() << "enter: m_xmldef = " << m_xmldef;
   qDebug() << "enter: m_xmldef = " << m_xmldef;
   QString rv;
   QStringList sl = pFormDef->getIndexField(m_xmldef).split(",");
   foreach(QString fieldName, sl)
   {
      if ( rv.size() > 0 ) 
      {
         rv += " and ";
      }
      QSqlField f(fieldName, QVariant::String);
      f.setValue(m_widgetMap.value(fieldName)->getValue());
      rv += fieldName + " = " + m_db.driver()->formatValue(f);
   }
   qDebug() << "exit: rv = " << rv;
   return(rv);
}

QVariant AutoDataForm::getIndexValue() const
{
   QStringList sl = pFormDef->getIndexField(m_xmldef).split(",");
   QVariant val = m_record.value(sl.at(0));
   return(val);
}

QSqlRecord AutoDataForm::insertRecord()
{
   insertRecord(VariantMap());
}

QSqlRecord AutoDataForm::insertRecord(const VariantMap &predefined_fields)
{
   QString fields;
   QString bindings;
   VariantMap field_vals;

   qDebug() << "enter- xmldef = " << m_xmldef << ", predefined fields: " << field_vals;
   if (hasChanges())
   {
      if (validateSave())
      {
         updateRecord();
         commitTransaction();
      }
      else
      {
         rollbackTransaction();
      }
   }
   beginTransaction();

   QString sql(INSERT_SQL);
#if 1
   foreach (const QString &field_name, predefined_fields.keys())
   {
      field_vals.insert(field_name, predefined_fields.value(field_name));
      if (fields.length() > 0)
      {
         fields += ", ";
         bindings += ", ";
      }
      fields += field_name;
      bindings += QString(":%1").arg(field_name);
   }
   foreach (const QString &field_name, m_widgetMap.keys())
   {
      const DataWidget *data_wdt = m_widgetMap.value(field_name);
      qDebug() << "testing field: " << field_name;
      QVariant def_val = pFormDef->getFieldDefault(data_wdt->getFieldDef());
      if ( ! def_val.toString().isEmpty())
      {
         qDebug() << "adding field: " << field_name;
         field_vals.insert(field_name, def_val);
         if (fields.length() > 0)
         {
            fields += ", ";
            bindings += ", ";
         }
         fields += field_name;
         bindings += QString(":%1").arg(field_name);
      }
   }
#endif
   sql = sql.arg(m_tableName)
            .arg(fields)
            .arg(bindings);
   qDebug() << "sql: " << sql;
   QSqlQuery q1;
   q1.prepare(sql);
   foreach (const QString &field_name, field_vals.keys())
   {
      qDebug() << "Binding " << field_vals.value(field_name).toString() 
               << QString(" to :%1").arg(field_name);
      q1.bindValue(QString(":%1").arg(field_name), field_vals.value(field_name).toString());
   }
   if ( ! q1.exec())
   {
      SqlError::showError("inserting record", this);
      rollbackTransaction();
      return(QSqlRecord());
   }
   QSqlRecord rec;
   if (q1.next())
   {
      rec = q1.record();
      refresh(&rec, true);
      emit(updated());
   }
   else
   {
      qDebug() << "No new frecord returned";
   }
   return(rec);
}

QString AutoDataForm::makeFilter()
{
   return(pFormDef->getWhereClause(m_xmldef, &m_record, &m_db));
}

QSqlRecord* AutoDataForm::record()
{
   return(&m_record);
}

void AutoDataForm::refresh()
{
   recordToForm(m_record);
}

void AutoDataForm::refresh(QSqlRecord *record)
{
   refresh(record, false);
}

void AutoDataForm::refresh(QSqlRecord *record, bool no_transaction)
{
   qDebug() << objectName() << "enter...";
   if (! no_transaction && hasChanges())
   {
      if (validateSave())
      {
         updateRecord();
         commitTransaction();
      }
      else
      {
         rollbackTransaction();
      }
   }

   qDebug() << objectName() << "no_transaction = " << no_transaction;
   if (! no_transaction)
   {
      beginTransaction();
   }

   qDebug() << objectName() << "setting m_record to " << record;
   m_record = *record;

   qDebug() << objectName() << "fetching where clause";
   QString filter = pFormDef->getWhereClause(m_xmldef, &m_record, &m_db);
   qDebug() << "Have filter: " << filter;
   qDebug() << objectName() << "m_fieldLabelMap: " << m_fieldLabelMap;
   QString sql(SELECT_SQL);
   sql = sql.arg(m_fieldLabelMap.keys().join(", "))
            .arg(m_model.tableName())
            .arg(filter);
   qDebug() << "sql: " << sql;
   QSqlQuery q1;
   q1.prepare(sql);
   if ( ! q1.exec())
   {
      SqlError::showError("fetching selected record", this);
      rollbackTransaction();
      return;
   }
   if (q1.next())
   {
      recordToForm(q1.record());
   }
}

void AutoDataForm::updateRecord()
{
   QString fields;
   QString filter = pFormDef->getWhereClause(m_xmldef, &m_record, &m_db);
   QSqlRecord record = formToRecord(m_model.record());


   qDebug() << "Have filter: " << filter;

   QList<DataWidget*> field_list = modifiedFields();
   if (field_list.count() > 0)
   {
      QString sql(UPDATE_SQL);
      foreach (const DataWidget *data_wdt, modifiedFields())
      {
         QString field_name = data_wdt->getFieldName();
         if (fields.length() > 0)
         {
            fields += ", ";
         }
         fields += field_name + " = :" + field_name;
      }
      sql = sql.arg(m_model.tableName())
               .arg(fields)
               .arg(filter);
      qDebug() << "sql: " << sql;
      QSqlQuery q1;
      q1.prepare(sql);
      foreach (const DataWidget *data_wdt, modifiedFields())
      {
         QString field_name = data_wdt->getFieldName();
         QVariant field_value = data_wdt->getValue();
   //      qDebug() << "Binding " << data_wdt->getValue().toString() << QString(" to :%1").arg(field_name);
         q1.bindValue(QString(":%1").arg(field_name), data_wdt->getValue());
      }
      if ( ! q1.exec())
      {
         SqlError::showError("updating record", this);
         rollbackTransaction();
         return;
      }
      emit(updated());
   }
   qDebug() << "Exit";
}

/*!
   \class QcjLib::DataForm

   This class is a QFrame that used to hold various types
   of DataWidget and has functionality to Readily access
   the data in the widgetes as QSqlRecord objects.
*/
DataForm::DataForm(QWidget *parent) : CancelableFrame(parent)
{
   qDebug() << "Enter";
}

bool DataForm::cancel()
{
   if (hasChanges())
   {
      QMessageBox::StandardButton btn;
      btn = QMessageBox::question(this, "Warning, this screen has change",
         "This screen has changes! If you continue,those changes will be lost " 
         "To keep the changes, press Cancel, to abandon them press Ok",
         QMessageBox::StandardButton::Ok | QMessageBox::StandardButton::Cancel);
      if (btn == QMessageBox::StandardButton::Ok)
      {
         return(true);
      }
      else
      {
         return(false);
      }
   }
}

bool DataForm::hasChanges() const
{
   QList<DataWidget*> rv;
   QList<QWidget*> widgets = findChildren<QWidget*>();
   return(false);

   foreach (QWidget *wdt, widgets)
   {
      DataWidget *data_wdt = dynamic_cast<DataWidget*>(wdt);
      if (data_wdt != nullptr)
      {
         qDebug() << "Testing widget: " << data_wdt->getFieldName();
         if (data_wdt->hasChanges())
         {
            qDebug() << objectName() << ": field" << data_wdt->getFieldName() << " changed";
            data_wdt->setFocus(Qt::OtherFocusReason);
            return(true);
         }
      }
   }
   qDebug() << "No fields changed";
   return(false);
}

QList<DataWidget*> DataForm::modifiedFields()
{
   QList<DataWidget*> rv;
   QList<QWidget*> widgets = findChildren<QWidget*>();
   foreach (QWidget *wdt, widgets)
   {
      DataWidget *data_wdt = dynamic_cast<DataWidget*>(wdt);
      if (data_wdt != nullptr)
      {
         qDebug() << "Testing widget: " << data_wdt->getFieldName();
         if (data_wdt->hasChanges())
         {
            qDebug() << "Adding " << data_wdt->getFieldDef().dataName 
                     << " to modified widget list";
            rv << data_wdt;
         }
      }
   }
   return(rv);
}

void DataForm::setDatabase()
{
   qDebug() << "Enter";
   QList<QWidget*> widgets = findChildren<QWidget*>();
   foreach (QWidget *wdt, widgets)
   {
      qDebug() << "widget: " << wdt->objectName();
      DataWidget *data_wdt = dynamic_cast<DataWidget*>(wdt);
      if (data_wdt != nullptr && ! data_wdt->getFieldName().isEmpty())
      {
         qDebug() << "Initializing field: " << data_wdt->getFieldName();
         data_wdt->initialize(m_xmldef);
         QcjDataFieldDef field_def = data_wdt->getFieldDef();
         qDebug() << "dataName: " << field_def.dataName
                  << ", label: " << field_def.label;
         m_fieldLabelMap.insert(field_def.dataName, field_def.label);
         m_labelFieldMap.insert(field_def.label, field_def.dataName);
         m_widgetMap.insert(field_def.dataName, data_wdt);
      }
   }
   qDebug() << objectName() << "m_fieldLabelMap: " << m_fieldLabelMap;
}

QSqlRecord DataForm::formToRecord(const QSqlRecord &record) const
{
   QSqlRecord rec = record;
   QList<QWidget*> widgets = findChildren<QWidget*>();
   foreach (QWidget *wdt, widgets)
   {
      DataWidget *data_wdt = dynamic_cast<DataWidget*>(wdt);
      if (data_wdt != nullptr)
      {
         QString rec_name = m_labelFieldMap.value(data_wdt->getFieldName());
         qDebug() << "searching for label: " << data_wdt->getFieldName() 
                  << ", field: " << rec_name;
         if (rec.contains(rec_name))
         {
            rec.setValue(rec_name, data_wdt->getValue());
            qDebug() << "rec field" << rec_name
                     << "set to: " << data_wdt->getText();
         }
      }
   }
   return(rec);
}

void DataForm::clearForm()
{
   QList<QWidget*> widgets = findChildren<QWidget*>();
   foreach (QWidget *wdt, widgets)
   {
      DataWidget *data_wdt = dynamic_cast<DataWidget*>(wdt);
      if (data_wdt != nullptr)
      {
         data_wdt->setDefault();
      }
   }
}

void DataForm::recordToForm(const QSqlRecord &rec)
{
   qDebug() << "rec: " << rec;
   QList<QWidget*> widgets = findChildren<QWidget*>();
   foreach (QWidget *wdt, widgets)
   {
      DataWidget *data_wdt = dynamic_cast<DataWidget*>(wdt);
      if (data_wdt != nullptr)
      {
         QString rec_name = m_labelFieldMap.value(data_wdt->getFieldName());
         if (rec_name == QString())
         {
            rec_name = data_wdt->getFieldName();
         }
         qDebug() << "fieldName = " << data_wdt->getFieldName() << "rec_name = " << rec_name;
         if (rec.contains(rec_name))
         {
            if (rec.isNull(rec_name))
            {
               QcjDataFieldDef field_def = data_wdt->getFieldDef();
               qDebug() << "Setting form field " << data_wdt->getFieldName() << " to default: " << field_def.defvalue;
               data_wdt->setValue(data_wdt->defaultValue());
            }
            else
            {
               qDebug() << "Setting form field " << data_wdt->getFieldName() << " to " << rec.value(rec_name);
               data_wdt->setValue(rec.value(rec_name));
               qDebug() << "value set";
            }
            qDebug() << "widget " << data_wdt->getFieldName()
                     << "set to: " << data_wdt->getText();
         }
      }
   }
}

/********************************************************************/
/* Base widget for all of the various DataWidget that can populate  */
/* a DataForm                                                       */
/********************************************************************/

/***********************************************************************/
/* This function serves as a datawidget factory. It uses the fieldType */
/* element in the field definition to create the appropriate widget    */
/* for the form.                                                       */
/***********************************************************************/
DataWidget *DataWidget::widgetFactory(const QcjDataFieldDef &field_def, const QString &xmldef, QWidget *parent)
{
#if 1
   DataWidget *rv;
   qDebug() << "Enter- xmldef = " << xmldef;
   if (field_def.fieldType == DataWidget::LINE_EDIT)
   {
      rv = new TextLineEdit(parent);
   }
   else if  (field_def.fieldType == DataWidget::TEXT_EDIT)
   {
      rv = new TextBlockEdit(parent);
   }
   else if  (field_def.fieldType == DataWidget::PHONE_EDIT)
   {
      rv = new PhoneEdit(parent);
   }
   else if  (field_def.fieldType == DataWidget::STRING_SELECT)
   {
      rv = new StringSelect(parent);
   }
   else if  (field_def.fieldType == DataWidget::INTEGER_SPIN)
   {
      rv = new SpinBox(parent);
   }
   else if  (field_def.fieldType == DataWidget::DOUBLE_SPIN)
   {
      rv = new DoubleSpinBox(parent);
   }
   else if  (field_def.fieldType == DataWidget::MONEY_EDIT)
   {
      rv = new MoneyEdit(parent);
   }
   else if  (field_def.fieldType == DataWidget::IMAGE_EDIT)
   {
      rv = new PhotoEntry(parent);
   }
   else if  (field_def.fieldType == DataWidget::YESNO_SELECT)
   {
      rv = new YesNoSelect(parent);
   }
   else if  (field_def.fieldType == DataWidget::CHECKBOX)
   {
      rv = new CheckBox(parent);
   }
   else if  (field_def.fieldType == DataWidget::DATE_EDIT)
   {
      rv = new DateEntry(parent);
   }
   else if  (field_def.fieldType == DataWidget::TIME_EDIT)
   {
      rv = new TimeEntry(parent);
   }
   else if  (field_def.fieldType == DataWidget::DATE_TIME_EDIT)
   {
      rv = new TimestampEntry(parent);
   }
   else
   {
      rv = new TextLineEdit(parent);
      qWarning() << "Invalid field type defined for field " << field_def.dataName
                  << " of form definition " << xmldef;
   }
   rv->setFieldName(field_def.dataName);
   rv->setFieldDefinition(field_def);
   return(rv);
#endif
}

QVariant DataWidget::defaultValue() const
{
   return(QVariant(m_fieldDef.defvalue));
}

bool DataWidget::hasChanges() const
{
   qDebug() << "Original value: " << m_setValue;
   qDebug() << "Current value: " << getValue();
   return(m_setValue.isValid() && getValue() != m_setValue);
}

void DataWidget::setText(const QString &)
{
   qDebug() << "Function not implimented";
};

void DataWidget::setFieldDefinition(const QcjDataFieldDef &field_def)
{
   m_fieldDef = field_def;
   qDebug() << "dataName: " << m_fieldDef.dataName
            << ", label: " << m_fieldDef.label;
}

void DataWidget::setFieldDefinition(const QString &xmldef)
{
   if (m_xmldef.isEmpty())
   {
      m_xmldef = xmldef;
   }
   qDebug() << "Get field def";
   if (m_fieldDef.dataName.isEmpty())
   {
      m_fieldDef = pFormDef->getFieldDef(xmldef, m_fieldName);
   }
}

/***************************************************************/
/***************************************************************/
/***                  CheckBox Widget                        ***/
/***************************************************************/
/***************************************************************/
QVariant CheckBox::defaultValue() const
{
   QString def_value = m_fieldDef.defvalue.trimmed().toUpper().at(0);
   if (def_value == "C" || def_value == "Y")
   {
      return(QVariant("Y"));
   }
   else
   {
      return(QVariant("N"));
   }
}

bool CheckBox::hasChanges() const
{
   qDebug() << "Original value: " << m_setValue;
   qDebug() << "Current value: " << getValue();
   return(m_setValue != QString() && getValue() != m_setValue);
}

void CheckBox::initialize(const QString &xmldef)
{
   qDebug() << "Initialize, xmldef = " << xmldef
            << ", text = " << text();
   setFieldDefinition(xmldef);

   QString def_value = m_fieldDef.defvalue.trimmed().toUpper().at(0);
   if (def_value == "C" || def_value == "Y")
   {
      setChecked(true);
      m_setValue = def_value;
   }
   else
   {
      setChecked(false);
   }
   if (m_fieldDef.ro)
   {
      setFocusPolicy(Qt::NoFocus);
   }
   if (m_fieldDef.ro)
   {
      setFocusPolicy(Qt::NoFocus);
   }
}

void CheckBox::setReadOnly(bool flag)
{
   QCheckBox::setEnabled(flag);
}

QVariant CheckBox::getValue() const
{
   return(QVariant(getText()));
}

QString CheckBox::getText() const
{
   QString rv;
#ifndef QT4_DESIGNER_PLUGIN
   rv = isChecked() ? "Y" : "N";
#endif
   return(rv);
}

void CheckBox::setText(const QString &str)
{
   if (str == QString())
   {
      setText("No");
   }
   else
   {
      QCheckBox::setText(str);
   }
}


void CheckBox::setValue(const QVariant &value)
{
   m_setValue = value;
   QString str_value = value.toString().trimmed().toUpper().at(0);
   qDebug() << "str_value = " << str_value 
            << "checked: " << (str_value == "C" || str_value == "Y");
   setChecked(str_value == "C" || str_value == "Y");
}

void CheckBox::setDefault()
{
   qDebug() << getFieldName() << "default value = " << m_fieldDef.defvalue;
   setValue(QVariant(m_fieldDef.defvalue));
}
   
/***************************************************************/
/***************************************************************/
/***                 DateEntry Widget                        ***/
/***************************************************************/
/***************************************************************/
void DateEntry::initialize(const QString &xmldef)
{
   qDebug() << "Initialize, xmldef = " << xmldef;
   setFieldDefinition(xmldef);
   if (m_fieldDef.ro)
   {
      setFocusPolicy(Qt::NoFocus);
   }
   if (m_fieldDef.ro)
   {
      setFocusPolicy(Qt::NoFocus);
   }
}

void DateEntry::setReadOnly(bool flag)
{
   QDateEdit::setEnabled(flag);
}

QVariant DateEntry::getValue() const
{
   return(QVariant(date()));
}

QString DateEntry::getText() const
{
   return(QString());
}

void DateEntry::setValue(const QVariant &value)
{
   m_setValue = value;
   if (value.type() == QVariant::Date ||
       value.type() == QVariant::DateTime)
   {
      setDate(value.toDate());
   }
   else
   {
      QString date_fmt = pConfig->value("DateFormat").toString();
      setDate(QDate::fromString(value.toString(), date_fmt));
   }
}

void DateEntry::setText(const QString &t)
{
#ifndef QT4_DESIGNER_PLUGIN
   QString date_fmt = pConfig->value("DateFormat").toString();
   setDate(QDate::fromString(t, date_fmt));
#endif
}

void DateEntry::setDefault()
{
   setValue(QVariant(m_fieldDef.defvalue));
}
   
/***************************************************************/
/***************************************************************/
/***                   DoubleSpinBox Widget                  ***/
/***************************************************************/
/***************************************************************/
void DoubleSpinBox::initialize(const QString &xmldef)
{
   qDebug() << "Initialize, xmldef = " << xmldef;
   setFieldDefinition(xmldef);

   if (m_fieldDef.defvalue != QString())
   {
      setValue(m_fieldDef.defvalue.toDouble());
   }
   if (m_fieldDef.minValue != QString())
   {
      setMinimum(m_fieldDef.minValue.toDouble());
   }
   if (m_fieldDef.maxValue != QString())
   {
      qDebug() << "maxValue set to: " << m_fieldDef.maxValue.toDouble();
      setMaximum(m_fieldDef.maxValue.toDouble());
   }
   if (m_fieldDef.decimals != QString())
   {
      setDecimals(m_fieldDef.decimals.toInt());
   }
   if (m_fieldDef.stepValue != QString())
   {
      setSingleStep(m_fieldDef.stepValue.toDouble());
   }
   if (m_fieldDef.suffix != QString())
   {
      setSuffix(m_fieldDef.suffix);
   }
   if (m_fieldDef.ro)
   {
      setFocusPolicy(Qt::NoFocus);
   }
   if (m_fieldDef.ro)
   {
      setFocusPolicy(Qt::NoFocus);
   }
}

void DoubleSpinBox::setReadOnly(bool flag)
{
   QDoubleSpinBox::setEnabled(flag);
}

QVariant DoubleSpinBox::getValue() const
{
   return(QVariant(value()));
}

QString DoubleSpinBox::getText() const
{
   return(text());
}

QString DoubleSpinBox::text() const
{
   QString s = cleanText();
#ifndef QT4_DESIGNER_PLUGIN
#endif
   return(s);
}

void DoubleSpinBox::setValue(const QVariant &value)
{
   qDebug() << "value = " << value;
   m_setValue = value;
   QDoubleSpinBox::setValue(value.toDouble());
}

void DoubleSpinBox::setText(const QString &s)
{
#ifndef QT4_DESIGNER_PLUGIN
   setValue(s.toInt());
#endif
}

void DoubleSpinBox::setDefault()
{
   setValue(QVariant(m_fieldDef.defvalue));
}
   
/***************************************************************/
/***************************************************************/
/***                   MoneyEdit Widget                      ***/
/***************************************************************/
/***************************************************************/
void MoneyEdit::initialize(const QString &xmldef)
{
   qDebug() << "Initialize, xmldef = " << xmldef;
   setFieldDefinition(xmldef);
   qDebug() << "creating validator";
   QRegExpValidator *v = new QRegExpValidator(QRegExp("\\$?[0-9,]*\\.?[0-9]{0,2}"), this);
   setValidator(v);
   setAlignment(Qt::AlignRight);
   qDebug() << "Connecting editingFinished";
   connect(this, SIGNAL(editingFinished()), this, SLOT(formatText()));
   fflush(stdout); 
   if (m_fieldDef.ro)
   {
      setFocusPolicy(Qt::NoFocus);
   }
}

void MoneyEdit::formatText()
{
   QString txt = text();
   qDebug() << "Enter: text =" << txt;
   QLineEdit::setText(formatCurrency(txt));
   qDebug() << "exit";
}

void MoneyEdit::setReadOnly(bool flag)
{
   QLineEdit::setReadOnly(flag);
}

QVariant MoneyEdit::getValue() const
{
   return(QVariant(text()));
}

QString MoneyEdit::getText() const
{
   return(text());
}

bool MoneyEdit::hasChanges() const
{
   qDebug() << "Original value: " << m_setValue;
   qDebug() << "Current value: " << getValue();
   return(m_setValue.isValid() && getValue().toDouble() != m_setValue);
}

QString MoneyEdit::text() const
{
   QString s = QLineEdit::text();
#ifndef QT4_DESIGNER_PLUGIN
   QString exp;
   QString cur = pConfig->value("Currency", "$").toString();
   if ( cur == "$" ) 
      cur = "\\$";
   exp = "[" + QString(pLocale->groupSeparator()) + cur + "]";
   s.replace(QRegExp(exp), "");

//   s.replace(QRegExp("[\\$,]"), "");
#endif
   return(s);
}

void MoneyEdit::setValue(const QVariant &value)
{
   m_setValue = value;
   setText(value.toString());
}

void MoneyEdit::setText(const QString &f) 
{
   QString str = f;

   QString exp;
   QString cur = pConfig->value("Currency", "$").toString();
   if ( cur == "$" ) 
      cur = "\\$";
   exp = "[" + QString(pLocale->groupSeparator()) + cur + "]";
   str.replace(QRegExp(exp), "");

//   str.replace(QRegExp("[\\$,]"), "");
   QLineEdit::setText(formatCurrency(str));
}

QString MoneyEdit::formatCurrency(const QString &money)
{
   QString s, s1, s2;
   QString str = money;
   str.remove("$");
   str.remove(",");
   double f = str.toDouble();

#ifndef QT4_DESIGNER_PLUGIN
   s = pLocale->toString(f, 'f', 2);
   s = pConfig->value("Currency", "$").toString().replace("&nbsp;", " ") + s;

#endif
   return(s);
}

void MoneyEdit::setDefault()
{
   setValue(QVariant(m_fieldDef.defvalue));
}

/***************************************************************/
/***************************************************************/
/***                   PhoneEdit Widget                      ***/
/***************************************************************/
/***************************************************************/
void PhoneEdit::initialize(const QString &xmldef)
{ 
   qDebug() << "Initialize, xmldef = " << xmldef;
   setFieldDefinition(xmldef);
   
   if (m_phoneRE.pattern() == QString())
   {
      m_phoneRE.setPattern(pConfig->value("PhoneParse", "(\\d{3})(\\d{3})(\\d{4})").toString());
   }
   m_phoneFormat = pConfig->value("PhoneFormat", "(%1) %2-%3" ).toString();
   if (m_fieldDef.ro)
   {
      setFocusPolicy(Qt::NoFocus);
   }
}  

void PhoneEdit::setReadOnly(bool flag)
{
   QLineEdit::setReadOnly(flag);
}

QVariant PhoneEdit::getValue() const
{
   return(QVariant(getText()));
}

QString PhoneEdit::getText() const
{
   return(text());
}

QString PhoneEdit::text() const
{
   QString s = QLineEdit::text();
   QString rv;

#ifndef QT4_DESIGNER_PLUGIN
   s.replace(QRegExp("[() -.]"), "");
   QRegularExpressionMatch match = m_phoneRE.match(s);
   for (int x = 1; x < match.lastCapturedIndex() + 1; x++)
   {
      if (false && rv.length() > 0)
      {
         rv += ' ';
      }
      rv += match.captured(x);
   }
   if (rv.length() == 0)
   {
      rv = s;
   }
//   s.replace(QRegExp("[\\$,]"), "");
#endif
   return(rv);
}

void PhoneEdit::setValue(const QVariant &value)
{
   m_setValue = value;
   setText(value.toString());
}

void PhoneEdit::setText(const QString &f) 
{
   QString str = f;
   str.replace(QRegExp("[() -.]"), "");

//   str.replace(QRegExp("[\\$,]"), "");
   QLineEdit::setText(formatPhoneNumber(str));
}

QString PhoneEdit::formatPhoneNumber(const QString &phone) const
{
   QString rv(m_phoneFormat);
   QString str(phone);
#ifndef QT4_DESIGNER_PLUGIN
   str.replace(QRegExp("[() -.]"), "");
   QRegularExpressionMatch match = m_phoneRE.match(str);
   if (match.lastCapturedIndex() > 0)
   {
      for (int x = 1; x < match.lastCapturedIndex() + 1; x++)
      {
         rv = rv.arg(match.captured(x));
      }
   }
   else
   {
      rv = str;
   }
   fflush(stdout);
#endif
   return(rv);
}

void PhoneEdit::setDefault()
{
   setValue(QVariant(m_fieldDef.defvalue));
}

#ifndef NO_PHOTO_SELECT
/***************************************************************/
/***************************************************************/
/***                 PhotoEntry Widget                       ***/
/***************************************************************/
/***************************************************************/
void PhotoEntry::initialize(const QString &xmldef)
{
   qDebug() << "Initialize, xmldef = " << xmldef;
   setFieldDefinition(xmldef);
   m_width = m_fieldDef.width.toInt();
   m_height = m_fieldDef.height.toInt();
   if (m_fieldDef.ro)
   {
      setFocusPolicy(Qt::NoFocus);
   }
}

QString PhotoEntry::getText() const
{
   return(text());
}

QString PhotoEntry::text() const
{
   QString sum;
   sum = QString(QCryptographicHash::hash(m_ba, QCryptographicHash::Md5).toHex().constData());
   return(sum);
}

void PhotoEntry::setText(const QString &str)
{
   QLabel::setText(str);
}

bool PhotoEntry::match(QByteArray ba) const
{
   QString sum1;
   QString sum2;
   sum1 = QString(QCryptographicHash::hash(m_ba, QCryptographicHash::Md5).constData());
   sum2 = QString(QCryptographicHash::hash(ba, QCryptographicHash::Md5).constData());
   return(sum1 == sum2);
}

QVariant PhotoEntry::getValue()  const
{
   return(QVariant(m_ba));
}

void PhotoEntry::setReadOnly(bool flag)
{
}

void PhotoEntry::setValue(const QVariant &value)
{
   m_setValue = value;
   m_ba = value.toByteArray();
   if ( m_ba.size() == 0 ) 
   {
      QLabel::setText("<html>Double click</html>");
   }
   else 
   {
      showImage();
   }
   qDebug() << "md5sum: " << text();
}
 
void PhotoEntry::showImage()
{
   QPixmap pm;
   if ( ! pm.loadFromData(m_ba))
   {
      qDebug() << "Error loading data";
   }
   pm = pm.scaledToWidth(width() - 5);

   qDebug() << "showing image, m_width" << m_width << ", size: " << pm.size();
   setPixmap(pm);
}

void PhotoEntry::mouseDoubleClickEvent(QMouseEvent*) 
{
   qDebug() << "starting dialog";
   QcjLib::CameraCaptureDialog photoDlg(this);
   bool good = photoDlg.exec();
   if (good && photoDlg.hasImage())
   {
      m_ba = photoDlg.getImage("PNG", 5);
      qDebug() << "image size: " << m_ba.size() << ", md5sum: " << text();
      showImage();
   }
   qDebug() << "dialog exited, status: " << good;
}

void PhotoEntry::setDefault()
{
   setValue(QVariant(m_fieldDef.defvalue));
}

#endif

/***************************************************************/
/***************************************************************/
/***                      SpinBox Widget                     ***/
/***************************************************************/
/***************************************************************/
void SpinBox::initialize(const QString &xmldef)
{
   qDebug() << "Initialize, xmldef = " << xmldef;
   setFieldDefinition(xmldef);
   if (m_fieldDef.defvalue != QString())
   {
      m_setValue = m_fieldDef.defvalue;
      setValue(m_fieldDef.defvalue.toInt());
   }
   if (m_fieldDef.minValue != QString())
   {
      setMinimum(m_fieldDef.minValue.toInt());
   }
   if (m_fieldDef.maxValue != QString())
   {
      setMaximum(m_fieldDef.maxValue.toInt());
   }
   if (m_fieldDef.stepValue != QString())
   {
      setSingleStep(m_fieldDef.stepValue.toInt());
   }
   if (m_fieldDef.suffix != QString())
   {
      setSuffix(m_fieldDef.suffix);
   }
   if (m_fieldDef.ro)
   {
      setFocusPolicy(Qt::NoFocus);
   }
}

void SpinBox::setReadOnly(bool flag)
{
   QSpinBox::setEnabled(flag);
}

QVariant SpinBox::getValue() const
{
   return(QVariant(value()));
}

QString SpinBox::getText() const
{
   return(text());
}

QString SpinBox::text() const
{
   QString s = cleanText();
#ifndef QT4_DESIGNER_PLUGIN
#endif
   return(s);
}

void SpinBox::setValue(const QVariant &value)
{
   m_setValue = value;
   QSpinBox::setValue(value.toInt());
}

void SpinBox::setText(const QString &s)
{
#ifndef QT4_DESIGNER_PLUGIN
   setValue(s.toInt());
#endif
}

void SpinBox::setDefault()
{
   setValue(QVariant(m_fieldDef.defvalue));
}

/***************************************************************/
/***************************************************************/
/***               StringSelect Widget                       ***/
/***************************************************************/
/***************************************************************/
QString StringSelect::text() const
{
   QString s = currentText();
#ifndef QT4_DESIGNER_PLUGIN
   int i = currentIndex();
   QVariant v = itemData(i);
   if (  v != QVariant::Invalid) 
   {
      s = v.toString();
   }
   if ( s == "<NULL>") 
   {
      s = "";
   }
#endif
   return(s);
}

void StringSelect::initialize(const QString &xmldef)
{
   qDebug() << "Initialize, xmldef = " << xmldef;
   setFieldDefinition(xmldef);
   QStringList init_list = parseInitString(m_fieldDef.init).split(",");
   qDebug() << "init_list: " << init_list;
   foreach (QString item, init_list)
   {
      QComboBox::addItem(item);
   }
   if (m_fieldDef.defvalue != QString())
   {
      setCurrentText(m_fieldDef.defvalue);
   }
   else
   {
      setCurrentIndex(0);
   }
   if (m_fieldDef.ro)
   {
      setFocusPolicy(Qt::NoFocus);
   }
}

void StringSelect::setReadOnly(bool flag)
{
   QComboBox::setEnabled(flag);
}

QVariant StringSelect::getValue() const
{
   return(QVariant(currentText()));
}

void StringSelect::setValue(const QVariant &value)
{
   m_setValue = value;
   setText(value.toString());
}

void StringSelect::setText(const QString &s)
{
   int i = 0;
   QString str = s;
#ifndef QT4_DESIGNER_PLUGIN
   if ( str == "" ) 
   {
      str = "<NULL>";
   }
   if ((i = findData(QVariant(str))) >= 0)
   {
      setCurrentIndex(i);
   }
   else if ((i = findText(s)) >= 0) 
   {
//      setSelected(i, true);
      setCurrentIndex(i);
      setItemText(i, str);
   }
   else 
   {
      insertItem(0, str);
      setCurrentIndex(0);
      setItemText(0, str);
   }
#endif
}

QString StringSelect::getText() const
{
   return(text());
}

void StringSelect::setDefault()
{
   setValue(QVariant(m_fieldDef.defvalue));
}

/***************************************************************/
/***************************************************************/
/***                   TextBlockEdit Widget                  ***/
/***************************************************************/
/***************************************************************/
void TextBlockEdit::initialize(const QString &xmldef)
{
   qDebug() << "Initialize, xmldef = " << xmldef;
   setFieldDefinition(xmldef);
   qDebug() << "default value = " << m_fieldDef.defvalue;
   if (m_fieldDef.defvalue != QString())
   {
      setText(m_fieldDef.defvalue);
   }
   if (m_fieldDef.ro)
   {
      setFocusPolicy(Qt::NoFocus);
   }
   qDebug() << "exit";
}

void TextBlockEdit::setReadOnly(bool flag)
{
   QTextEdit::setReadOnly(flag);
}

QVariant TextBlockEdit::getValue() const
{
   return(QVariant(text()));
}

QString TextBlockEdit::getText() const
{
   return(text());
}

QString TextBlockEdit::text() const
{
   QString s = toPlainText();
#ifndef QT4_DESIGNER_PLUGIN
#endif
   return(s);
}

void TextBlockEdit::setValue(const QVariant &value)
{
   m_setValue = value;
   setText(value.toString());
}

void TextBlockEdit::setText(const QString &s)
{
#ifndef QT4_DESIGNER_PLUGIN
   setPlainText(s);
#endif
}

void TextBlockEdit::setDefault()
{
   setValue(QVariant(m_fieldDef.defvalue));
}

/***************************************************************/
/***************************************************************/
/***                    TextLineEdit Widget                  ***/
/***************************************************************/
/***************************************************************/
void TextLineEdit::initialize(const QString &xmldef)
{
   qDebug() << "Initialize, xmldef = " << xmldef;
   setFieldDefinition(xmldef);
   if (m_fieldDef.defvalue != QString())
   {
      setText(m_fieldDef.defvalue);
   }
   if (m_fieldDef.ro)
   {
      setFocusPolicy(Qt::NoFocus);
   }
}

void TextLineEdit::setReadOnly(bool flag)
{
   QLineEdit::setReadOnly(flag);
}

QVariant TextLineEdit::getValue() const
{
   return(QVariant(text()));
}

QString TextLineEdit::getText() const
{
   return(text());
}

QString TextLineEdit::text() const
{
   QString s = QLineEdit::text();
#ifndef QT4_DESIGNER_PLUGIN
#endif
   return(s);
}

void TextLineEdit::setValue(const QVariant &value)
{
   m_setValue = value;
   setText(value.toString());
}

void TextLineEdit::setText(const QString &s)
{
#ifndef QT4_DESIGNER_PLUGIN
   QLineEdit::setText(s);
#endif
}

void TextLineEdit::setDefault()
{
   setValue(QVariant(m_fieldDef.defvalue));
}

/***************************************************************/
/***************************************************************/
/***                    TimeEntry Widget                     ***/
/***************************************************************/
/***************************************************************/
void TimeEntry::initialize(const QString &xmldef)
{
   qDebug() << "Initialize, xmldef = " << xmldef;
   setFieldDefinition(xmldef);
   if (m_fieldDef.ro)
   {
      setFocusPolicy(Qt::NoFocus);
   }
}

void TimeEntry::setReadOnly(bool flag)
{
   QTimeEdit::setEnabled(flag);
}

QVariant TimeEntry::getValue() const
{
   return(time());
}

void TimeEntry::setValue(const QVariant &value)
{
   m_setValue = value;
   setTime(value.toTime());
}

QString TimeEntry::getText() const
{
   return(QString());
}

void TimeEntry::setDefault()
{
   setValue(QVariant(m_fieldDef.defvalue));
}

/***************************************************************/
/***************************************************************/
/***                TimestampEntry Widget                    ***/
/***************************************************************/
/***************************************************************/
void TimestampEntry::initialize(const QString &xmldef)
{
   qDebug() << "Initialize, xmldef = " << xmldef;
   setFieldDefinition(xmldef);
   if (m_fieldDef.ro)
   {
      setFocusPolicy(Qt::NoFocus);
   }
}

void TimestampEntry::setReadOnly(bool flag)
{
   QDateTimeEdit::setEnabled(flag);
}

QVariant TimestampEntry::getValue() const
{
   return(QVariant(dateTime()));
}

QString TimestampEntry::getText() const
{
   return(text());
}

QString TimestampEntry::text() const
{
   QString rv;
#ifndef QT4_DESIGNER_PLUGIN
   QString date_fmt = pConfig->value("DateFormat").toString();
   date_fmt += " hh:mm:ss";
   rv = dateTime().toString(date_fmt);
#endif
   return(rv);
}

void TimestampEntry::setValue(const QVariant &value)
{
   m_setValue = value;
   if (value.type() == QVariant::Date ||
       value.type() == QVariant::DateTime)
   {
      setDateTime(value.toDateTime());
   }
   else
   {
      setText(value.toString());
   }
}

void TimestampEntry::setText(const QString &t)
{
#ifndef QT4_DESIGNER_PLUGIN
   QString date_fmt = pConfig->value("DateFormat").toString();
   date_fmt += " hh:mm:ss";
   setDate(QDate::fromString(t, date_fmt));
#endif
}

void TimestampEntry::setDefault()
{
   setValue(QVariant(m_fieldDef.defvalue));
}

/***************************************************************/
/***************************************************************/
/***                  TrueFalseSelect Widget                 ***/
/***************************************************************/
/***************************************************************/
void TrueFalseSelect::initialize(const QString &xmldef)
{
   qDebug() << "Initialize, xmldef = " << xmldef;
   setFieldDefinition(xmldef);

   addItem("False");
   addItem("True");
   if (m_fieldDef.defvalue.trimmed().toUpper().startsWith("T"))
   {
      setCurrentText("True");
   }
   if (m_fieldDef.ro)
   {
      setFocusPolicy(Qt::NoFocus);
   }
}

void TrueFalseSelect::setReadOnly(bool flag)
{
   QComboBox::setEnabled(flag);
}

QVariant TrueFalseSelect::getValue() const
{
   return(QVariant(getText() == "Y"));
}

QString TrueFalseSelect::getText() const
{
   return(currentText());
}

void TrueFalseSelect::setValue(const QVariant &value)
{
   QString val_string;
   m_setValue = value;
   if (value.type() == QVariant::Bool)
   {
      if (value.toBool())
      {
         val_string = "True";
      }
      else
      {
         val_string = "False";
      }
   }
   else
   {
      val_string = value.toString();
   }
   setCurrentText(val_string);
}

void TrueFalseSelect::setText(const QString &t)
{
   QString s;
#ifndef QT4_DESIGNER_PLUGIN
   if ( t.toUpper() == "Y" ) 
      s = "Yes";
   else
      s = "No";
   setCurrentText(s);
#endif
}

void TrueFalseSelect::setDefault()
{
   setValue(QVariant(m_fieldDef.defvalue));
}

/***************************************************************/
/***************************************************************/
/***                YesNoSelect Widget                       ***/
/***************************************************************/
/***************************************************************/
QVariant YesNoSelect::defaultValue() const
{
   QString def_value = m_fieldDef.defvalue.trimmed().toUpper().at(0);
   if (def_value == "Y")
   {
      return(QVariant("Y"));
   }
   else
   {
      return(QVariant("N"));
   }
}

void YesNoSelect::initialize(const QString &xmldef)
{
   qDebug() << "Initialize, xmldef = " << xmldef;
   setFieldDefinition(xmldef);

   addItem("No");
   addItem("Yes");
   if (m_fieldDef.defvalue.trimmed().toUpper().startsWith("Y"))
   {
      m_setValue = "Y";
      setText(m_setValue.toString());
   }
   if (m_fieldDef.ro)
   {
      setFocusPolicy(Qt::NoFocus);
   }
}

void YesNoSelect::setReadOnly(bool flag)
{
   QComboBox::setEnabled(flag);
}

QVariant YesNoSelect::getValue() const
{
   qDebug() << "value: " << text().at(0);
   return(QVariant(text().at(0)));
}

QString YesNoSelect::getText() const
{
   return(text());
}

QString YesNoSelect::text() const
{
   QString rv;
#ifndef QT4_DESIGNER_PLUGIN
   rv = currentText();
#endif
   return(rv);
}

void YesNoSelect::setValue(const QVariant &value)
{
   QString val_string;
   m_setValue = value;
   if (value.type() == QVariant::Bool)
   {
      if (value.toBool())
      {
         val_string = "Yes";
      }
      else
      {
         val_string = "No";
      }
   }
   else
   {
      val_string = value.toString();
   }
   setText(val_string);
}

void YesNoSelect::setText(const QString &t)
{
   QString s;
#ifndef QT4_DESIGNER_PLUGIN
   if ( t.toUpper().startsWith("Y") ) 
      s = "Yes";
   else
      s = "No";
   setCurrentText(s);
#endif
}

void YesNoSelect::setDefault()
{
   setValue(QVariant(m_fieldDef.defvalue));
}

