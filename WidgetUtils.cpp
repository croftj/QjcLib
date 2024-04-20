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
#include "WidgetUtils.h"
#include "../QcjData/QcjDataXML.h"
#include "../QcjData/QcjDataHelpers.h"
#include <QDebug>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QLineEdit>
#include <QSpinBox>
#include <QLineEdit>

namespace WidgetUtils
{
   /***********************************************************************/
   /* This function will search through the child widgets of the widget   */
   /* parameter form and search for one with the property field_name that */
   /* matches the parameter field_name. If a widget is not found a NULL   */
   /* is returned                                                         */
   /***********************************************************************/
   QWidget *findChildByFieldName(QWidget *form, QString field_name)
   {
      QWidget *rv = NULL;

      qDebug() << "search for field_name: " << field_name;
      // Get a list of al of the children widgets
      QList<QWidget*> widgets = form->findChildren<QWidget*>();
      foreach (QWidget *wdt, widgets)
      {
         QVariant fn = wdt->property("field_name");
         if (fn.toString().length() > 0)
         {
            qDebug() << "child object: " << wdt->objectName()
                     << ", fieldName: " << fn.toString();
            if (fn.isValid())
            {
               if (fn.toString() == field_name)
               {
                  qDebug() << "found it!";
                  rv = wdt;
                  break;
               }
            }
         }
      }
      return(rv);
   }

   /************************************************************************/
   /* This function sets the value of the passed in form element specified */
   /* by widget.                                                           */
   /*                                                                      */
   /* It is assumed that the widget will be of a limited type typically    */
   /* used in a form.                                                      */
   /*                                                                      */
   /* If the the widget is not one of those types, false will be returned, */
   /* otherwise true will be returned.                                     */
   /************************************************************************/
   bool setWidgetValue(QWidget* wdt, QVariant val)
   {
      bool rv = true;
      if ( isA(wdt, "QTextEdit") || isA(wdt, "QcjTextBlockEdit") ) 
      {
         qDebug() << "TextEdit: " << val.toString();
         static_cast<QcjTextBlockEdit*>(wdt)->setText(val.toString());
      }
      else if ( isA(wdt, "QLineEdit") ) 
      {
         qDebug() << "LineEdit: " << val.toString();
         static_cast<QLineEdit*>(wdt)->setText(val.toString());
      }
      else if ( isA(wdt, "QSpinBox") )
      {
         qDebug() << "spin: " << val.toInt();
         static_cast<QSpinBox*>(wdt)->setValue(val.toInt());
      }
      else if (isA(wdt, "QDoubleSpinBox"))
      {
         qDebug() << "double spin: " << val.toDouble();
         static_cast<QDoubleSpinBox*>(wdt)->setValue(val.toDouble());
      }
      else if ( isA(wdt, "QcjPhotoEntry") ) 
      {
         qDebug() << "image";
         static_cast<QcjPhotoEntry*>(wdt)->set(val.toByteArray());
      }
      else if (isA(wdt, "QCheckBox"))
      {
         static_cast<QCheckBox*>(wdt)->setChecked(val.toBool());
      }
      else if (isA(wdt, "QComboBox"))
      {
         static_cast<QComboBox*>(wdt)->setCurrentText(val.toString());
      }
      else
      {
         rv = false;
      }
      return(rv);
   }

   QVariant::Type widgetType(QWidget *wdt)
   {
      QVariant::Type rv;
      if ( isA(wdt, "QcjCheckBox") ) 
      {
         rv = QVariant::Bool;
      }
      else if ( isA(wdt, "QcjTextBlockEdit") ) 
      {
         rv = QVariant::String;
      }
      else if ( isA(wdt, "QcjMoneyEdit") ) 
      {
         rv = QVariant::Double;
      }
      else if ( isA(wdt, "QLineEdit") ) 
      {
         rv = QVariant::String;
      }
      else if ( isA(wdt, "QSpinBox") )
      {
         rv = QVariant::Int;
      }
      else if (isA(wdt, "QDoubleSpinBox"))
      {
         rv = QVariant::Double;
      }
      else if (isA(wdt, "QDateEdit"))
      {
         rv = QVariant::Date;
      }
      else if (isA(wdt, "QDateTimeEdit"))
      {
         rv = QVariant::DateTime;
      }
      return(rv);
   }

   QVariant getWidgetValue(QWidget *wdt)
   {
      QVariant rv;
      if ( isA(wdt, "QcjTextBlockEdit") ) 
      {
         rv = QVariant(static_cast<QcjTextBlockEdit*>(wdt)->text());
      }
      else if ( isA(wdt, "QcjPhotoEntry") ) 
      {
         rv = QVariant(static_cast<QcjPhotoEntry*>(wdt)->get());
      }
      else if ( isA(wdt, "QCheckBox") ) 
      {
         rv = QVariant(static_cast<QCheckBox*>(wdt)->isChecked());
      }
      else if ( isA(wdt, "QTextEdit") ) 
      {
         rv = QVariant(static_cast<QLineEdit*>(wdt)->text());
      }
      else if ( isA(wdt, "QcjPhotoEntry") ) 
      {
         rv = QVariant(static_cast<QcjPhotoEntry*>(wdt)->get());
      }
      else if ( isA(wdt, "QSpinBox") )
      {
         rv = QVariant(static_cast<QSpinBox*>(wdt)->value());
      }
      else if (isA(wdt, "QDoubleSpinBox"))
      {
         rv = QVariant(static_cast<QDoubleSpinBox*>(wdt)->value());
      }
      else if (isA(wdt, "QCheckBox"))
      {
         rv = QVariant(static_cast<QCheckBox*>(wdt)->isChecked());
      }
      return(rv);
   }

   QSqlField createField(QWidget *wdt, QString field_name)
   {
      QVariant::Type type = widgetType(wdt);
      QSqlField rv(field_name, type);

      rv.setValue(getWidgetValue(wdt));
      if (isA(wdt, "QDoubleSpinBox"))
      {
         rv.setPrecision(static_cast<QDoubleSpinBox*>(wdt)->decimals());
      }
      else if (isA(wdt, "QcjMoneyEdit"))
      {
         rv.setPrecision(2);
      }
      return(rv);
   }

   bool isA(QObject *obj, const char *type)
   {
      qDebug() << "Enter, type = " << type;
      fflush(stdout);
      if ( obj != 0 ) 
      {
         const QMetaObject *mobj = obj->metaObject();
         while (mobj != NULL) 
         {
            printf("isA(): className = |%s|\n", mobj->className());
            fflush(stdout);
            if ( QString(mobj->className()) == QString(type) ) 
            {
                  printf("isA(): Exit(true)\n");
                  fflush(stdout);
                  return(true);
            }
            mobj = mobj->superClass();
         }
      }
      else
         printf("isA(): Object is a Null pointer\n");
      printf("isA(): Exit(false)\n");
      fflush(stdout);
      return(false);
   }

   /*!
         \fn bool isAObject(QObject *obj, const char *type)

         Tests  the  passed  object  to  be  of  the  type  specified by
         <em>type</em>.
   */ 
   bool isAObject(QObject *obj, const char *type)
   {
      printf("isA(): Enter\n");
      fflush(stdout);
      if (obj != NULL && obj->objectName() == QString(type))
      {
         printf("isAObject(): objectName = |%s|\n", qPrintable(obj->objectName()));
         fflush(stdout);
         printf("isAObject(): Exit(true)\n");
         fflush(stdout);
         return(true);
      }
      else
         printf("isAObject(): Object is a Null pointer\n");
      printf("isAObject(): Exit(false)\n");
      fflush(stdout);
      return(false);
   }

   QString objectType(QObject *obj)
   {
      QString rv = QStringLiteral("nullptr");
      if ( obj != 0 ) 
      {
         const QMetaObject *mobj = obj->metaObject();
         rv = mobj->className();
         while (mobj != NULL) 
         {
            mobj = mobj->superClass();
         }
      }
      return(rv);
   }

   void fieldDefToString(const QcjDataFieldDef &field_def)
   {
      qDebug().nospace() << "field_def = " 
               << "dataname: "      << field_def.dataName << "\n"
               << "label: "         << field_def.label << "\n"
               << "key: "           << field_def.key << "\n"
               << "valuename: "     << field_def.valueName << "\n"
               << "row: "           << field_def.row << "\n"
               << "col: "           << field_def.col << "\n"
               << "colspan: "       << field_def.colSpan << "\n"
               << "rowspan: "       << field_def.rowSpan << "\n"
               << "minwidth: "      << field_def.minWidth << "\n"
               << "maxwidth: "      << field_def.maxWidth << "\n"
               << "ro: "            << field_def.ro << "\n"
               << "init: "          << field_def.init << "\n"
               << "defvalue: "      << field_def.defvalue << "\n"
               << "fieldtype: "     << field_def.fieldType << "\n"
               << "propname: "      << field_def.propName << "\n"
               << "options: "       << field_def.options << "\n"
               << "format: "        << field_def.format << "\n"
               << "align: "         << field_def.align << "\n"
               << "color: "         << field_def.color << "\n"
               << "bgcolor: "       << field_def.bgcolor << "\n"
               << "width: "         << field_def.width << "\n"
               << "height: "        << field_def.height << "\n"
               << "search: "        << field_def.search << "\n"
               << "minvalue: "      << field_def.minValue << "\n"
               << "maxvalue: "      << field_def.maxValue << "\n"
               << "stepvalue: "     << field_def.stepValue << "\n"
               << "decimals: "      << field_def.decimals << "\n"
               << "suffix: "        << field_def.suffix << "\n"
               << "visible: "        << field_def.visible << "\n"
               << "focuswidget: "   << field_def.focusWidget << "\n";
   }
};
