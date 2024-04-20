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
#ifndef QCJLIB_DATA_WIDGETS_H
#define QCJLIB_DATA_WIDGETS_H

#include "QcjData/QcjDataHelpers.h"
#include "QcjLib/CancelableFrame.h"
#include "QcjLib/Types.h"

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
#include <QCheckBox>
#include <QDateEdit>
#include <QDebug>
#include <QDoubleSpinBox>
#include <QHash>
#include <QLabel>
#include <QLineEdit>
#include <QPair>
#include <QSpinBox>
#include <QSqlTableModel>
#include <QString>
#include <QTextEdit>
#include <QVariant>
#include <QWidget>

#include <stdlib.h>
#include <stdio.h>

namespace QcjLib
{
   typedef QPair<QString, VariantMap> SqlStatement;

   class AutoDataForm;
   class DataForm;
   class DataWidget;
   class DataFrame;

   class CheckBox;
   class DateEntry;
   class DoubleSpinBox;
   class MoneyEdit;
   class PhoneEdit;
   class PhotoEntry;
   class SpinBox;
   class StringSelect;
   class TextBlockEdit;
   class TextLineEdit;
   class TimeEntry;
   class TimestampEntry;
   class TrueFalseSelect;
   class YesNoSelect;

   class DataForm : public CancelableFrame
   {
      Q_OBJECT
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ readXmldef );

   public:
      DataForm(QWidget *parent = nullptr);
      bool cancel();
      void setXmldef(const QString &name)
      {
         qDebug() << "m_xmldef: " << m_xmldef;
         m_xmldef = name;
      };

      QString readXmldef()
      {
         qDebug() << "m_xmldef: " << m_xmldef;
         return(m_xmldef);
      };

      void setDatabase();
      void clearForm();
      QSqlRecord formToRecord(const QSqlRecord &record) const;
      void recordToForm(const QSqlRecord &record);
      bool hasField(const QString &data_name) const;
      bool hasChanges() const;
      DataWidget *fieldWidget(const QString &data_name);
      QList<DataWidget*> modifiedFields();

   protected:
      QString                       m_xmldef;
      QHash<QString, QString>       m_fieldLabelMap;
      QHash<QString, QString>       m_labelFieldMap;
      QHash<QString, DataWidget*>   m_widgetMap;
   };

   class AutoDataForm : public DataForm
   {
      Q_OBJECT
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ readXmldef );

   signals:
      void updated();
      
   public:
      AutoDataForm(QWidget *parent = nullptr);
      bool cancel();
      void setReadOnly(bool flag);
      void clearForm();
      void deleteRecord();
      QVariant getFieldValue(const QString &name) const;
      void setFieldValue(const QString &name, const QVariant &value);
      QString getIndexFilter();
      QVariant getIndexValue() const;
      QSqlRecord insertRecord();
      QSqlRecord insertRecord(const QcjLib::VariantMap &predefined_fields);
      QString makeFilter();
      void setDatabase();
      QSqlRecord* record();
      void updateRecord();

   public slots:
      void refresh();
      void refresh(QSqlRecord *record);
      void refresh(QSqlRecord *record, bool no_transaction);
      void commitTransaction(bool emit_updated = true);
      void rollbackTransaction();
      
   protected:
      void beginTransaction();
      bool validateSave();

      QString           m_indexName;
      QSqlTableModel    m_model;
      QSqlRecord        m_record;
      QString           m_tableName;
      QSqlDatabase      m_db;

   private:
      friend                  DataFrame;
      bool                    m_inTransaction;
      QMap<QString, QString>  m_indexMap;
   };

   class DataWidget
   {
   public:
      DataWidget()
      {
//         qDebug() << "enter";
      }
      virtual void      initialize(const QString &xmldef) = 0;
      virtual QVariant  getValue() const = 0;
      virtual void      setValue(const QVariant &value) = 0;
      virtual QString   getText() const = 0;
      virtual bool      hasChanges() const;
      virtual void      setText(const QString &str);
      virtual void      setReadOnly(bool flag) = 0;
      virtual void      setSizePolicy(QSizePolicy policy) = 0;
      virtual void      setDefault() = 0;
      virtual QWidget*  widget() = 0;      
      virtual void      setFocus(Qt::FocusReason reason) = 0;

      QVariant          defaultValue() const;

      static DataWidget *widgetFactory(const QcjDataFieldDef &field_def,
                                       const QString &xmldef,
                                       QWidget *parent);

      void setFieldName(const QString &name)
      {
         m_fieldName = name;
         qDebug() << "m_fieldName = " << m_fieldName;
      };

      QString getFieldName() const
      {
         return(m_fieldName);
      };

      void setXmldef(const QString &name)
      {
         m_xmldef = name;
      };

      QString readXmldef() const
      {
         return(m_xmldef);
      };

      const QcjDataFieldDef getFieldDef() const
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
      void setFieldDefinition(const QcjDataFieldDef &field_def);
      void setFieldDefinition(const QString &xmldef);

      QcjDataFieldDef   m_fieldDef;
      QString           m_fieldName;
      QVariant          m_setValue;
      QString           m_xmldef;

   private:
   };

   /***************************************************************/
   /***************************************************************/
   /***                  CheckBox Widget                        ***/
   /***************************************************************/
   /***************************************************************/
   class CheckBox : public QCheckBox, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ readXmldef );

   public:
      CheckBox(QWidget * parent = 0) : QCheckBox(parent), DataWidget()
      {
         m_setValue = "N";
         qDebug().noquote() << objectName(); // << "::" << getFieldName();
      }
      CheckBox(const QString &text, QWidget * parent = 0) : QCheckBox(text, parent), DataWidget() { };

      QVariant defaultValue() const;
      QVariant getValue() const;
      bool hasChanges() const;
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);
      QString getText() const;
      void setText(const QString &str);
      void setReadOnly(bool flag);
      void setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      void setDefault();
      QWidget*  widget() { return(this); }
      void setFocus(Qt::FocusReason reason) {QWidget::setFocus(reason); }
   };

   /***************************************************************/
   /***************************************************************/
   /***                 DateEntry Widget                        ***/
   /***************************************************************/
   /***************************************************************/
   class DateEntry : public QDateEdit, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ readXmldef );

   public:
      DateEntry(QWidget * parent = 0) : QDateEdit(parent), DataWidget()
      {
         qDebug().noquote() << objectName(); // << "::" << getFieldName();
      };

      QVariant getValue() const;
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText() const;
      QString text() const;
      void setText(const QString &str);
      void setReadOnly(bool flag);
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      void setDefault();
      QWidget*  widget() { return(this); }
      void setFocus(Qt::FocusReason reason) {QWidget::setFocus(reason); }
   };

   /***************************************************************/
   /***************************************************************/
   /***                   DoubleSpinBox Widget                  ***/
   /***************************************************************/
   /***************************************************************/
   class DoubleSpinBox : public QDoubleSpinBox, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ readXmldef );

   public:
      DoubleSpinBox(QWidget * parent = 0) : QDoubleSpinBox(parent), DataWidget()
      {
         qDebug().noquote() << objectName(); // << "::" << getFieldName();
      };

      QVariant getValue() const;
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText() const;
      QString text() const;
      void setText(const QString &str);
      void setReadOnly(bool flag);
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      void setDefault();
      QWidget*  widget() { return(this); }
      void setFocus(Qt::FocusReason reason) {QWidget::setFocus(reason); }
   };

   /***************************************************************/
   /***************************************************************/
   /***                   MoneyEdit Widget                      ***/
   /***************************************************************/
   /***************************************************************/
   class MoneyEdit : public QLineEdit, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ readXmldef );

   public:
      MoneyEdit(QWidget * parent = 0) : QLineEdit(parent), DataWidget()
      {
         qDebug().noquote() << objectName(); // << "::" << getFieldName();
      };


      QVariant getValue() const;
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText() const;
      bool hasChanges() const;
      QString text() const;
      void setText(const QString &str);
      void setReadOnly(bool flag);
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      void setDefault();
      QWidget*  widget() { return(this); }

   protected:
      void formatText();
      QString formatCurrency(const QString &str);
      void setFocus(Qt::FocusReason reason) {QWidget::setFocus(reason); }
   };

   /***************************************************************/
   /***************************************************************/
   /***                   PhoneEdit Widget                      ***/
   /***************************************************************/
   /***************************************************************/
   class PhoneEdit : public QLineEdit, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ readXmldef );

   public:
      PhoneEdit(QWidget * parent = 0) : QLineEdit(parent), DataWidget()
      {
         qDebug().noquote() << objectName(); // << "::" << getFieldName();
      };


      QVariant getValue() const;
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText() const;
      QString text() const;
      void setText(const QString &str);
      void setReadOnly(bool flag);
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      void setDefault();
      QWidget*  widget() { return(this); }

   protected:
      QString formatPhoneNumber(const QString &str) const;

   private:
      QRegularExpression   m_phoneRE;
      QString              m_phoneFormat;
      void setFocus(Qt::FocusReason reason) {QWidget::setFocus(reason); }
   };

   /***************************************************************/
   /***************************************************************/
   /***                 PhotoEntry Widget                       ***/
   /***************************************************************/
   /***************************************************************/
#ifndef NO_PHOTO_SELECT
   class PhotoEntry : public QLabel, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QString thumb_name WRITE setThumbName READ getThumbName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ readXmldef );

   public:
      PhotoEntry(QWidget * parent = 0) : QLabel(parent), DataWidget()
      {
         qDebug().noquote() << objectName(); // << "::" << getFieldName();
      };

      void setThumbName(const QString &name)
      {
         m_thumbName = name;
         qDebug() << "m_thumbName = " << m_thumbName;
      };

      QString getThumbName() const
      {
         return(m_thumbName);
      };

      QVariant getScaledValue() const;
      QVariant getValue() const;
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText() const;
      void setText(const QString &str);
      void setReadOnly(bool flag);
      QString text() const;
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      void setDefault();
      void setWidth(int width);
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
      QByteArray  m_scaledImage;
      int         m_height;
      int         m_width;
      QString     m_thumbName;
      void setFocus(Qt::FocusReason reason) {QWidget::setFocus(reason); }
   };
#endif

   /***************************************************************/
   /***************************************************************/
   /***                      SpinBox Widget                     ***/
   /***************************************************************/
   /***************************************************************/
   class SpinBox : public QSpinBox, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ readXmldef );

   public:
      SpinBox(QWidget * parent = 0) : QSpinBox(parent), DataWidget()
      {
         qDebug().noquote() << objectName(); // << "::" << getFieldName();
      };


      QVariant getValue() const;
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText() const;
      QString text() const;
      void setText(const QString &str);
      void setReadOnly(bool flag);
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      void setDefault();
      QWidget*  widget() { return(this); }
      void setFocus(Qt::FocusReason reason) {QWidget::setFocus(reason); }
   };

   /***************************************************************/
   /***************************************************************/
   /***               StringSelect Widget                       ***/
   /***************************************************************/
   /***************************************************************/
   class StringSelect : public QComboBox, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ readXmldef );

   public:
      StringSelect(QWidget * parent = 0) : QComboBox(parent), DataWidget()
      {
         qDebug().noquote() << objectName(); // << "::" << getFieldName();
      };


      QVariant getValue() const;
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText() const;
      QString text() const;
      void    setText(const QString &);
      void setReadOnly(bool flag);
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      void setDefault();
      QWidget*  widget() { return(this); }
      void setFocus(Qt::FocusReason reason) {QWidget::setFocus(reason); }
   };

   /***************************************************************/
   /***************************************************************/
   /***                   TextBlockEdit Widget                  ***/
   /***************************************************************/
   /***************************************************************/
   class TextBlockEdit : public QTextEdit, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ readXmldef );

   public:
      TextBlockEdit(QWidget * parent = 0) : QTextEdit(parent), DataWidget()
      {
         qDebug().noquote() << objectName(); // << "::" << getFieldName();
      };


      QVariant getValue() const;
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText() const;
      QString text() const;
      void setText(const QString &str);
      void setReadOnly(bool flag);
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      void setDefault();
      QWidget*  widget() { return(this); }
      void setFocus(Qt::FocusReason reason) {QWidget::setFocus(reason); }
   };

   /***************************************************************/
   /***************************************************************/
   /***                    TextLineEdit Widget                  ***/
   /***************************************************************/
   /***************************************************************/
   class TextLineEdit : public QLineEdit, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ readXmldef );

   public:
      TextLineEdit(QWidget * parent = 0) : QLineEdit(parent), DataWidget()
      {
         qDebug().noquote() << objectName(); // << "::" << getFieldName();
      };


      QVariant getValue() const;
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText() const;
      QString text() const;
      void setText(const QString &str);
      void setReadOnly(bool flag);
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      void setDefault();
      QWidget*  widget() { return(this); }
      void setFocus(Qt::FocusReason reason) {QWidget::setFocus(reason); }
   };

   /***************************************************************/
   /***************************************************************/
   /***                    TimeEntry Widget                     ***/
   /***************************************************************/
   /***************************************************************/
   class TimeEntry : public QTimeEdit, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ readXmldef );

   public:
      TimeEntry(QWidget * parent = 0) : QTimeEdit(parent), DataWidget()
      {
         qDebug().noquote() << objectName(); // << "::" << getFieldName();
      };


      QVariant getValue() const;
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      void setReadOnly(bool flag);
      QString getText() const;
      QString text() const;
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      void setDefault();
      QWidget*  widget() { return(this); }
      void setFocus(Qt::FocusReason reason) {QWidget::setFocus(reason); }
   };

   /***************************************************************/
   /***************************************************************/
   /***                TimestampEntry Widget                    ***/
   /***************************************************************/
   /***************************************************************/
   class TimestampEntry : public QDateTimeEdit, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ readXmldef );

   public:
      TimestampEntry(QWidget * parent = 0) : QDateTimeEdit(parent), DataWidget()
      {
         qDebug().noquote() << objectName(); // << "::" << getFieldName();
      };


      QVariant getValue() const;
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText() const;
      QString text() const;
      void setText(const QString &str);
      void setReadOnly(bool flag);
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      void setDefault();
      QWidget*  widget() { return(this); }
      void setFocus(Qt::FocusReason reason) {QWidget::setFocus(reason); }
   };

   /***************************************************************/
   /***************************************************************/
   /***                  TrueFalseSelect Widget                 ***/
   /***************************************************************/
   /***************************************************************/
   class TrueFalseSelect : public QComboBox, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ readXmldef );

   public:
      TrueFalseSelect(QWidget * parent = 0) : QComboBox(parent), DataWidget()
      {
         qDebug().noquote() << objectName(); // << "::" << getFieldName();
      };


      QVariant getValue() const;
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText() const;
      QString text() const;
      void setText(const QString &str);
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      void setReadOnly(bool flag);
      void setDefault();
      QWidget*  widget() { return(this); }
      void setFocus(Qt::FocusReason reason) {QWidget::setFocus(reason); }
   };

   /***************************************************************/
   /***************************************************************/
   /***                YesNoSelect Widget                       ***/
   /***************************************************************/
   /***************************************************************/
   class YesNoSelect : public QComboBox, public DataWidget
   {
      Q_OBJECT
      Q_PROPERTY( QString field_name WRITE setFieldName READ getFieldName );
      Q_PROPERTY( QVariant value WRITE setValue READ getValue );
      Q_PROPERTY( QString xml_definition WRITE setXmldef READ readXmldef );

   public:
      YesNoSelect(QWidget * parent = 0) : QComboBox(parent)
                                        , DataWidget()
      {
         m_setValue = "N";
         qDebug() << objectName(); // << "::" << getFieldName();
      }

      QVariant defaultValue() const;
      QVariant getValue() const;
      void setValue(const QVariant &value);
      void initialize(const QString &xmldef);

      QString getText() const;
      QString text() const;
      void setText(const QString &);
      void setReadOnly(bool flag);
      void    setSizePolicy(QSizePolicy policy) { widget()->setSizePolicy(policy); };
      void setDefault();
      QWidget*  widget() { return(this); }
      void setFocus(Qt::FocusReason reason) {QWidget::setFocus(reason); }
   };
}

#endif
