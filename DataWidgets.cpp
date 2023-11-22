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
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QStringList>
#include <QWidget>
#include "DataWidgets.h"
#include "QcjData/QcjDataHelpers.h"
#include "QcjData/QcjDataStatics.h"
#include "QcjData/QcjPhotoSelect.h"
#include "QcjLib/CameraCaptureDialog.h"

//#define NO_PHOTO_SELECT

using namespace QcjLib;

namespace
{
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

   /***********************************************************************/
   /* This function serves as a datawidget factory of sorts. It uses the  */
   /* fieldType element in the field definition to create the appropriate */
   /* widget for the form.                                                */
   /***********************************************************************/
   DataWidget *buildWidget(const QcjDataFieldDef &field_def, const QString &xmldef, QWidget *parent)
   {
#if 1
      DataWidget *rv;
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
      return(rv);
#endif
   }
}

const QString DataWidget::LINE_EDIT       = {QStringLiteral("lineedit")};
const QString DataWidget::TEXT_EDIT       = {QStringLiteral("textedit")};
const QString DataWidget::PHONE_EDIT      = {QStringLiteral("phone")};
const QString DataWidget::STRING_SELECT   = {QStringLiteral("strsel")};
const QString DataWidget::INTEGER_SPIN    = {QStringLiteral("integer")};
const QString DataWidget::DOUBLE_SPIN     = {QStringLiteral("double")};
const QString DataWidget::MONEY_EDIT      = {QStringLiteral("monet")};
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
AutoDataForm::AutoDataForm(QWidget *parent) : DataForm(parent)
{
}

void AutoDataForm::setDatabase()
{
   QGridLayout *gl = new QGridLayout(this);
   gl->setSpacing(3);
   gl->activate();

   QSizePolicy lblPolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
   QSizePolicy wdtPolicySingle(QSizePolicy::Minimum, QSizePolicy::Maximum);
   QSizePolicy wdtPolicyMulti(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
#ifndef NO_PHOTO_SELECT
   QSizePolicy wdtPolicyPhoto(QSizePolicy::Preferred, QSizePolicy::Preferred);
#endif

   QcjDataFieldStdVector field_defs = pFormDef->getFields(m_xmldef, this);
   foreach(const QcjDataFieldDef &f_def, field_defs)
   {
      int col = f_def.col * 2;
      int row = f_def.row;
      DataWidget *wdt = buildWidget(f_def, m_xmldef, this);
      QLabel *lbl = new QLabel(f_def.label, this);
      lbl->setSizePolicy(lblPolicy);
      if (f_def.rowSpan <= 0)
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
         gl->addWidget(wdt->widget(), row, col, rows, cols > 1 ? ((cols - 1) * 2) + 1 : cols);
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
   DataForm::setDatabase();
}

/*!
   \class QcjLib::DataForm

   This class is a QFrame that used to hold various types
   of DataWidgets and has functionality to Readily access
   the data in the widgetes as QSqlRecord objects.
*/
DataForm::DataForm(QWidget *parent) : QFrame(parent)
{
}

void DataForm::setDatabase()
{
   QList<QWidget*> widgets = findChildren<QWidget*>();
   foreach (QWidget *wdt, widgets)
   {
      DataWidget *data_wdt = dynamic_cast<DataWidget*>(wdt);
      if (data_wdt != nullptr && ! data_wdt->getFieldName().isEmpty())
      {
         qDebug() << "Initialializing field: " << data_wdt->getFieldName();
         data_wdt->initialize(m_xmldef);
         QcjDataFieldDef field_def = data_wdt->getFieldDef();
         m_fieldLabelMap.insert(field_def.dataName, field_def.label);
         m_labelFieldMap.insert(field_def.label, field_def.dataName);
      }
   }
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

void DataForm::recordToForm(const QSqlRecord &record)
{
   QSqlRecord rec = record;
   QList<QWidget*> widgets = findChildren<QWidget*>();
   foreach (QWidget *wdt, widgets)
   {
      DataWidget *data_wdt = dynamic_cast<DataWidget*>(wdt);
      if (data_wdt != nullptr)
      {
         QString rec_name = m_labelFieldMap.value(data_wdt->getFieldName());
         qDebug() << "fieldName = " << data_wdt->getFieldName() << "rec_name = " << rec_name;
         if (rec.contains(rec_name))
         {
            if (rec.isNull(rec_name))
            {
               QcjDataFieldDef field_def = data_wdt->getFieldDef();
               qDebug() << "Setting form field " << data_wdt->getFieldName() << " to default: " << field_def.defvalue;
               data_wdt->setValue(QVariant(field_def.defvalue));
            }
            else
            {
               qDebug() << "Setting form field " << data_wdt->getFieldName() << " to " << rec.value(rec_name);
               data_wdt->setValue(rec.value(rec_name));
            }
            qDebug() << "widget " << data_wdt->getFieldName()
                     << "set to: " << data_wdt->getText();
         }
      }
   }
}

/********************************************************************/
/* Base widget for all of the various DataWidgets that can populate */
/* a DataForm                                                       */
/********************************************************************/
void DataWidget::setText(const QString &)
{
   qDebug() << "Function not implimented";
};

void DataWidget::setFieldDefinition(const QString &xmldef)
{
   if (m_xmldef.isEmpty())
   {
      m_xmldef = xmldef;
   }
   qDebug() << "Get field def";
   m_fieldDef = pFormDef->getFieldDef(xmldef, m_fieldName);
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
   printf("PhoneEdit(): pattern: %s\n", qPrintable(m_phoneRE.pattern()));
   printf("PhoneEdit(): format: %s\n", qPrintable(m_phoneFormat));
}  

QVariant PhoneEdit::getValue()
{
   return(QVariant(getText()));
}

QString PhoneEdit::getText()
{
   return(text());
}

QString PhoneEdit::text() const
{
   QString s = QLineEdit::text();
   QString rv;

#ifndef QT4_DESIGNER_PLUGIN
   printf("PhoneEdit::text(): started with |%s|\n", (const char*)s.toLocal8Bit());
   fflush(stdout);

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
   printf("PhoneEdit::text(): returned |%s|\n", (const char*)s.toLocal8Bit());
   fflush(stdout);
#endif
   return(rv);
}

void PhoneEdit::setValue(const QVariant &value)
{
   setText(value.toString());
}

void PhoneEdit::setText(const QString &f) 
{
   QString str = f;
   printf("PhoneEdit::SetText(): enter\n");
   fflush(stdout);
   str.replace(QRegExp("[() -.]"), "");

//   str.replace(QRegExp("[\\$,]"), "");
   QLineEdit::setText(formatPhoneNumber(str));
   printf("PhoneEdit::SetText(): set to |%s|\n", qPrintable(QLineEdit::text()));
   fflush(stdout);
}

QString PhoneEdit::formatPhoneNumber(const QString &phone) const
{
   QString rv(m_phoneFormat);
   QString str(phone);
#ifndef QT4_DESIGNER_PLUGIN
   printf("PhoneEdit::formatPhoneNumber(): str = %s\n", qPrintable(str));
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
   printf("PhoneEdit::formatCurrency(): exit(%s)\n", qPrintable(rv));
   fflush(stdout);
#endif
   return(rv);
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
}

void MoneyEdit::formatText()
{
   QString txt = text();
   qDebug() << "Enter: text =" << txt;
   QLineEdit::setText(formatCurrency(txt));
   qDebug() << "exit";
}

QVariant MoneyEdit::getValue()
{
   return(QVariant(text()));
}

QString MoneyEdit::getText()
{
   return(text());
}

QString MoneyEdit::text() const
{
   QString s = QLineEdit::text();
#ifndef QT4_DESIGNER_PLUGIN
   printf("MoneyEdit::text(): started with |%s|\n", (const char*)s.toLocal8Bit());
   fflush(stdout);

   QString exp;
   QString cur = pConfig->value("Currency", "$").toString();
   if ( cur == "$" ) 
      cur = "\\$";
   exp = "[" + QString(pLocale->groupSeparator()) + cur + "]";
   printf("MoneyEdit::SetText(): exp = |%s|\n", qPrintable(exp));
   s.replace(QRegExp(exp), "");

//   s.replace(QRegExp("[\\$,]"), "");
   printf("MoneyEdit::text(): returned |%s|\n", (const char*)s.toLocal8Bit());
   fflush(stdout);
#endif
   return(s);
}

void MoneyEdit::setValue(const QVariant &value)
{
   setText(value.toString());
}

void MoneyEdit::setText(const QString &f) 
{
   QString str = f;
   printf("MoneyEdit::SetText(): enter\n");
   fflush(stdout);

   QString exp;
   QString cur = pConfig->value("Currency", "$").toString();
   if ( cur == "$" ) 
      cur = "\\$";
   exp = "[" + QString(pLocale->groupSeparator()) + cur + "]";
   printf("MoneyEdit::SetText(): exp = |%s|\n", qPrintable(exp));
   str.replace(QRegExp(exp), "");

//   str.replace(QRegExp("[\\$,]"), "");
   QLineEdit::setText(formatCurrency(str));
   printf("MoneyEdit::SetText(): started with |%s|\n", (const char*)formatCurrency(f).toLocal8Bit());
   fflush(stdout);
}

QString MoneyEdit::formatCurrency(const QString &money)
{
   QString s, s1, s2;
   QString str = money;
   printf("MoneyEdit::formatCurrentcy(): Enter, str = |%s|\n", qPrintable(str));
   fflush(stdout);
   str.remove("$");
   str.remove(",");
   printf("MoneyEdit::formatCurrentcy(): converting |%s| to double\n", qPrintable(str));
   double f = str.toDouble();

#ifndef QT4_DESIGNER_PLUGIN
   printf("MoneyEdit::formatCurrentcy(): f = %15.2f\n", f);
   fflush(stdout);
   s = pLocale->toString(f, 'f', 2);
   printf("MoneyEdit::formatCurrentcy(): s = %s\n", qPrintable(s));
   s = pConfig->value("Currency", "$").toString().replace("&nbsp;", " ") + s;

   printf("MoneyEdit::formatCurrency(): exit(%s)\n", qPrintable(s));
   fflush(stdout);
#endif
   return(s);
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
}

QVariant StringSelect::getValue()
{
   return(QVariant(currentText()));
}

void StringSelect::setValue(const QVariant &value)
{
   setText(value.toString());
}

void StringSelect::setText(const QString &s)
{
   int i = 0;
   QString str = s;
#ifndef QT4_DESIGNER_PLUGIN
   printf("StringSelect::setText(): Enter, test = |%s|\n", qPrintable(s));
   if ( str == "" ) 
   {
      str = "<NULL>";
   }
   if ((i = findData(QVariant(str))) >= 0)
   {
      printf("StringSelect::setText(): Found data, index = %d\n", i);
      setCurrentIndex(i);
   }
   else if ((i = findText(s)) >= 0) 
   {
//      setSelected(i, true);
      printf("StringSelect::setText(): Found text, index = %d\n", i);
      setCurrentIndex(i);
      setItemText(i, str);
   }
   else 
   {
      printf("StringSelect::setText(): Adding new item\n");
      insertItem(0, str);
      setCurrentIndex(0);
      setItemText(0, str);
   }
   printf("StringSelect::setText(): Exit()\n");
#endif
}

QString StringSelect::getText()
{
   return(text());
}

/***************************************************************/
/***************************************************************/
/***                YesNoSelect Widget                       ***/
/***************************************************************/
/***************************************************************/
void YesNoSelect::initialize(const QString &xmldef)
{
   qDebug() << "Initialize, xmldef = " << xmldef;
   setFieldDefinition(xmldef);

   addItem("No");
   addItem("Yes");
   if (m_fieldDef.defvalue.trimmed().toUpper().startsWith("Y"))
   {
      setCurrentText("Yes");
   }
}

QVariant YesNoSelect::getValue()
{
   qDebug() << "value: " << text().at(0);
   return(QVariant(text().at(0)));
}

QString YesNoSelect::getText()
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
}

QVariant TrueFalseSelect::getValue()
{
   return(QVariant(getText() == "Y"));
}

QString TrueFalseSelect::getText()
{
   return(currentText());
}

void TrueFalseSelect::setValue(const QVariant &value)
{
   QString val_string;
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

/***************************************************************/
/***************************************************************/
/***                CheckBox Widget                       ***/
/***************************************************************/
/***************************************************************/
void CheckBox::initialize(const QString &xmldef)
{
   qDebug() << "Initialize, xmldef = " << xmldef
            << ", text = " << text();
   setFieldDefinition(xmldef);

   QString def_value = m_fieldDef.defvalue.trimmed().toUpper().at(0);
   if (def_value == "C" || def_value == "Y")
   {
      setChecked(true);
   }
   else
   {
      setChecked(false);
   }
}

QVariant CheckBox::getValue()
{
   return(QVariant(getText()));
}

QString CheckBox::getText()
{
   QString rv;
#ifndef QT4_DESIGNER_PLUGIN
   rv = isChecked() ? "Y" : "N";
#endif
   return(rv);
}

void CheckBox::setText(const QString &str)
{
   QCheckBox::setText(str);
}


void CheckBox::setValue(const QVariant &value)
{
   QString str_value = value.toString().trimmed().toUpper().at(0);
   qDebug() << "str_value = " << str_value 
            << "checked: " << (str_value == "C" || str_value == "Y");
   setChecked(str_value == "C" || str_value == "Y");
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
}

QVariant TimeEntry::getValue()
{
   return(time());
}

void TimeEntry::setValue(const QVariant &value)
{
   setTime(value.toTime());
}

QString TimeEntry::getText()
{
   return(QString());
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
}

QVariant DateEntry::getValue()
{
   return(QVariant(date()));
}

QString DateEntry::getText()
{
   return(QString());
}

void DateEntry::setValue(const QVariant &value)
{
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
   printf("DateEntry::set(): Entry  |%s|\n", (const char*)t.toLocal8Bit());
#ifndef QT4_DESIGNER_PLUGIN
   QString date_fmt = pConfig->value("DateFormat").toString();
   setDate(QDate::fromString(t, date_fmt));
#endif
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
}

QVariant TimestampEntry::getValue()
{
   return(QVariant(dateTime()));
}

QString TimestampEntry::getText()
{
   return(text());
}

QString TimestampEntry::text() const
{
   QString rv;
#ifndef QT4_DESIGNER_PLUGIN
   printf("TimestampEntry::get(): Enter\n");
   QString date_fmt = pConfig->value("DateFormat").toString();
   date_fmt += " hh:mm:ss";
   rv = dateTime().toString(date_fmt);
   printf("TimestampEntry::get(): Exit |%s|\n", (const char*)rv.toLocal8Bit());
#endif
   return(rv);
}

void TimestampEntry::setValue(const QVariant &value)
{
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
   printf("TimestampEntry::set(): Entry  |%s|\n", (const char*)t.toLocal8Bit());
#ifndef QT4_DESIGNER_PLUGIN
   QString date_fmt = pConfig->value("DateFormat").toString();
   date_fmt += " hh:mm:ss";
   setDate(QDate::fromString(t, date_fmt));
   printf("TimestampEntry::setText(): Exit\n");
#endif
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
   qDebug() << "exit";
}

QVariant TextBlockEdit::getValue()
{
   return(QVariant(text()));
}

QString TextBlockEdit::getText()
{
   return(text());
}

QString TextBlockEdit::text() const
{
   printf("TextBlockEdit::text(): Enter\n");
   QString s = toPlainText();
#ifndef QT4_DESIGNER_PLUGIN
#endif
   printf("TextBlockEdit::text(): exit with |%s|\n", (const char*)s.toLocal8Bit());
   return(s);
}

void TextBlockEdit::setValue(const QVariant &value)
{
   setText(value.toString());
}

void TextBlockEdit::setText(const QString &s)
{
#ifndef QT4_DESIGNER_PLUGIN
   printf("TextBlockEdit::text(): Enter- setting text |%s|\n", (const char*)s.toLocal8Bit());
   setPlainText(s);
   printf("TextBlockEdit::text(): exit\n");
#endif
}


/***************************************************************/
/***************************************************************/
/***                   TextLineEdit Widget                  ***/
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
}

QVariant TextLineEdit::getValue()
{
   return(QVariant(text()));
}

QString TextLineEdit::getText()
{
   return(text());
}

QString TextLineEdit::text() const
{
   printf("TextLineEdit::text(): Enter\n");
   QString s = QLineEdit::text();
#ifndef QT4_DESIGNER_PLUGIN
#endif
   printf("TextLineEdit::text(): exit with |%s|\n", (const char*)s.toLocal8Bit());
   return(s);
}

void TextLineEdit::setValue(const QVariant &value)
{
   setText(value.toString());
}

void TextLineEdit::setText(const QString &s)
{
#ifndef QT4_DESIGNER_PLUGIN
   printf("TextLineEdit::text(): Enter- setting text |%s|\n", (const char*)s.toLocal8Bit());
   QLineEdit::setText(s);
   printf("TextLineEdit::text(): exit\n");
#endif
}

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
}

QVariant SpinBox::getValue()
{
   return(QVariant(value()));
}

QString SpinBox::getText()
{
   return(text());
}

QString SpinBox::text() const
{
   printf("SpinBox::text(): Enter\n");
   QString s = cleanText();
#ifndef QT4_DESIGNER_PLUGIN
   printf("SpinBox::text(): exit with |%s|\n", (const char*)s.toLocal8Bit());
#endif
   return(s);
}

void SpinBox::setValue(const QVariant &value)
{
   setValue(value.toInt());
}

void SpinBox::setText(const QString &s)
{
#ifndef QT4_DESIGNER_PLUGIN
   printf("SpinBox::text(): Enter- setting text |%s|\n", (const char*)s.toLocal8Bit());
   setValue(s.toInt());
   printf("SpinBox::text(): exit\n");
#endif
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
      setMinimum(m_fieldDef.minValue.toInt());
   }
   if (m_fieldDef.maxValue != QString())
   {
      setMaximum(m_fieldDef.maxValue.toInt());
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
}

QVariant DoubleSpinBox::getValue()
{
   return(QVariant(value()));
}

QString DoubleSpinBox::getText()
{
   return(text());
}

QString DoubleSpinBox::text() const
{
   printf("DoubleSpinBox::text(): Enter\n");
   QString s = cleanText();
#ifndef QT4_DESIGNER_PLUGIN
   printf("DoubleSpinBox::text(): exit with |%s|\n", (const char*)s.toLocal8Bit());
#endif
   return(s);
}

void DoubleSpinBox::setValue(const QVariant &value)
{
   setValue(value.toInt());
}

void DoubleSpinBox::setText(const QString &s)
{
#ifndef QT4_DESIGNER_PLUGIN
   printf("DoubleSpinBox::text(): Enter- setting text |%s|\n", (const char*)s.toLocal8Bit());
   setValue(s.toInt());
   printf("DoubleSpinBox::text(): exit\n");
#endif
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
}

QString PhotoEntry::getText()
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

QVariant PhotoEntry::getValue() 
{
   return(QVariant(m_ba));
}

void PhotoEntry::setValue(const QVariant &value)
{
   printf("PhotoEntry::set(): Enter, pic size = %d\n", m_ba.size());
   fflush(stdout);
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
   printf("PhotoEntry::set(): Exit\n");
   fflush(stdout);
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
#endif
