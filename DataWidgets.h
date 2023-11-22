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


#ifndef DATA_WIDGETS_H
#define DATA_WIDGETS_H

#include "QcjData/QcjDataHelpers.h"

#include <qcombobox.h>
//#include <q3datetimeedit.h>
//#include <q3frame.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qsettings.h>
//#include <q3sqlcursor.h>
#include <qsqldatabase.h>
//#include <q3sqleditorfactory.h>
//#include <q3sqlform.h>
#include <qsqlrecord.h>
#include <qstring.h>
#include <qtextedit.h>
#include <QHash>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include <QFrame>
#include <QCheckBox>
#include <QDateEdit>
#include <QSpinBox>
#include <QTextEdit>
#include <QDoubleSpinBox>
#include <stdlib.h>
#include <stdio.h>

namespace QcjLib
{
   class DataForm : public QFrame
   {
      Q_OBJECT
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ getXmldef );

   public:
      DataForm(QWidget *parent = nullptr);
      void setXmldef(const QString &name)
      {
         m_xmldef = name;
      };

      QString getXmldef()
      {
         return(m_xmldef);
      };

      void setDatabase();
      QSqlRecord formToRecord(const QSqlRecord &record) const;
      void recordToForm(const QSqlRecord &record);

   protected:
      QString                 m_xmldef;

   private:
      QHash<QString, QString> m_fieldLabelMap;
      QHash<QString, QString> m_labelFieldMap;
   };

   class AutoDataForm : public DataForm
   {
      Q_OBJECT
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ getXmldef );

   public:
      AutoDataForm(QWidget *parent = nullptr);
      void setDatabase();
   };

   class DataWidget
   {
   public:
      virtual void      initialize(const QString &xmldef) = 0;
      virtual QVariant  getValue() = 0;
      virtual void      setValue(const QVariant &value) = 0;
      virtual QString   getText() = 0;
      virtual void      setText(const QString &str);
      virtual void      setSizePolicy(QSizePolicy policy) = 0;
      virtual QWidget*  widget() = 0;      

      void setFieldName(const QString &name)
      {
         m_fieldName = name;
      };

      QString getFieldName()
      {
         return(m_fieldName);
      };

      void setXmldef(const QString &name)
      {
         m_xmldef = name;
      };

      QString getXmldef()
      {
         return(m_xmldef);
      };

      const QcjDataFieldDef getFieldDef()
      {
         return(m_fieldDef);
      }

      static const QString LINE_EDIT;
      static const QString TEXT_EDIT;
      static const QString PHONE_EDIT;
      static const QString STRING_SELECT;
      static const QString INTEGER_SPIN;
      static const QString DOUBLE_SPIN;
      static const QString MONEY_EDIT;
      static const QString IMAGE_EDIT;
      static const QString YESNO_SELECT;
      static const QString CHECKBOX;
      static const QString DATE_EDIT;
      static const QString TIME_EDIT;
      static const QString DATE_TIME_EDIT;

   protected:
      void setFieldDefinition(const QString &xmldef);

      QcjDataFieldDef   m_fieldDef;
      QString           m_fieldName;
      QString           m_xmldef;
   private:
   };

   class StringSelect : public QComboBox, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ getXmldef );

   public:
      StringSelect(QWidget * parent = 0) : QComboBox(parent), DataWidget() { };

      QVariant getValue();
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText();
      QString text() const;
      void    setText(const QString &);
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      QWidget*  widget() { return(this); }
   };

   class YesNoSelect : public QComboBox, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ getXmldef );

   public:
      YesNoSelect(QWidget * parent = 0) : QComboBox(parent), DataWidget() { };

      QVariant getValue();
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText();
      QString text() const;
      void setText(const QString &);
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      QWidget*  widget() { return(this); }
   };

   class TrueFalseSelect : public QComboBox, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ getXmldef );

   public:
      TrueFalseSelect(QWidget * parent = 0) : QComboBox(parent), DataWidget() { };

      QVariant getValue();
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText();
      QString text() const;
      void setText(const QString &str);
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      QWidget*  widget() { return(this); }
   };

   class CheckBox : public QCheckBox, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ getXmldef );

   public:
      CheckBox(QWidget * parent = 0) : QCheckBox(parent), DataWidget() { };
      CheckBox(const QString &text, QWidget * parent = 0) : QCheckBox(text, parent), DataWidget() { };

      QVariant getValue();
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);
      QString getText();
      void setText(const QString &str);
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      QWidget*  widget() { return(this); }
   };

   class PhoneEdit : public QLineEdit, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ getXmldef );

   public:
      PhoneEdit(QWidget * parent = 0) : QLineEdit(parent), DataWidget() { };

      QVariant getValue();
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText();
      QString text() const;
      void setText(const QString &str);
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      QWidget*  widget() { return(this); }

   protected:
      QString formatPhoneNumber(const QString &str) const;

   private:
      QRegularExpression   m_phoneRE;
      QString              m_phoneFormat;
   };

   class MoneyEdit : public QLineEdit, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ getXmldef );

   public:
      MoneyEdit(QWidget * parent = 0) : QLineEdit(parent), DataWidget() { };

      QVariant getValue();
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText();
      QString text() const;
      void setText(const QString &str);
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      QWidget*  widget() { return(this); }

   protected:
      void formatText();
      QString formatCurrency(const QString &str);
   };

   class TextBlockEdit : public QTextEdit, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ getXmldef );

   public:
      TextBlockEdit(QWidget * parent = 0) : QTextEdit(parent), DataWidget() { };

      QVariant getValue();
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText();
      QString text() const;
      void setText(const QString &str);
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      QWidget*  widget() { return(this); }
   };

   class TextLineEdit : public QLineEdit, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ getXmldef );

   public:
      TextLineEdit(QWidget * parent = 0) : QLineEdit(parent), DataWidget() { };

      QVariant getValue();
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText();
      QString text() const;
      void setText(const QString &str);
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      QWidget*  widget() { return(this); }
   };

   class DateEntry : public QDateEdit, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ getXmldef );

   public:
      DateEntry(QWidget * parent = 0) : QDateEdit(parent), DataWidget() { };

      QVariant getValue();
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText();
      QString text() const;
      void setText(const QString &str);
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      QWidget*  widget() { return(this); }
   };

   class TimeEntry : public QTimeEdit, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ getXmldef );

   public:
      TimeEntry(QWidget * parent = 0) : QTimeEdit(parent), DataWidget() { };

      QVariant getValue();
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText();
      QString text() const;
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      QWidget*  widget() { return(this); }
   };

   class TimestampEntry : public QDateTimeEdit, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ getXmldef );

   public:
      TimestampEntry(QWidget * parent = 0) : QDateTimeEdit(parent), DataWidget() { };

      QVariant getValue();
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText();
      QString text() const;
      void setText(const QString &str);
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      QWidget*  widget() { return(this); }
   };

   class SpinBox : public QSpinBox, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ getXmldef );

   public:
      SpinBox(QWidget * parent = 0) : QSpinBox(parent), DataWidget() { };

      QVariant getValue();
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText();
      QString text() const;
      void setText(const QString &str);
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      QWidget*  widget() { return(this); }
   };

   class DoubleSpinBox : public QDoubleSpinBox, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ getXmldef );

   public:
      DoubleSpinBox(QWidget * parent = 0) : QDoubleSpinBox(parent), DataWidget() { };

      QVariant getValue();
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText();
      QString text() const;
      void setText(const QString &str);
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      QWidget*  widget() { return(this); }
   };

#ifndef NO_PHOTO_SELECT
   class PhotoEntry : public QLabel, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ getXmldef );

   public:
      PhotoEntry(QWidget * parent = 0) : QLabel(parent), DataWidget() { };

      QVariant getValue();
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText();
      void setText(const QString &str);
      QString text() const;
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      QWidget*  widget() { return(this); }

   signals:
      void filename(QString file_name);
      void editingFinished();

   protected:
      void mouseDoubleClickEvent(QMouseEvent*);
      bool match(QByteArray ba) const;
      void showImage();

   private:
      QByteArray  m_ba;
      int         m_height;
      int         m_width;
   };
#endif
}

#endif
