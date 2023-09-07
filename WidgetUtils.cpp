#include "WidgetUtils.h"
#include "../QcjData/QcjDataHelpers.h"
#include <QDebug>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QLineEdit>
#include <QSpinBox>
#include <QLineEdit>

namespace WidgetUtils
{
   /*!
         \fn bool isA(QObject *obj, const char *type)

         Tests  the  passed  object  to  be  of  the  class name
         specified by <em>type</em>.
   */ 
   bool isA(QObject *obj, const char *type)
   {
      qDebug() << "enter";
      if ( obj != 0 ) 
      {
         const QMetaObject *mobj = obj->metaObject();
         while (mobj != NULL) 
         {
            qDebug() << "have meta object";
            qDebug() << "className: " << QString(mobj->className());
            if ( QString(mobj->className()) == QString(type) ) 
            {
               qDebug() << "Found it!";
                  return(true);
            }
            qDebug() << "moving to superClass";
            mobj = mobj->superClass();
         }
      }
      return(false);
   }

   /*!
         \fn bool isAObject(QObject *obj, const char *type)

         Tests the passed object to have the object name specified by
         <em>type</em>.
   */ 
   static bool isAObject(QObject *obj, const char *type)
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

   /***********************************************************************/
   /* This function will search through the child widgets of the widget   */
   /* parameter form and search for one with the property field_name that */
   /* matches the parameter field_name. If a widget is not found a NULL   */
   /* is returned                                                         */
   /***********************************************************************/
   QWidget *findChildByFieldName(QWidget *form, QString field_name)
   {
      QWidget *rv = NULL;

      // Get a list of al of the children widgets
      QList<QWidget*> widgets = form->findChildren<QWidget*>();
      foreach (QWidget *wdt, widgets)
      {
         QVariant fn = wdt->property("field_name");
         qDebug() << "child object: " << wdt->objectName()
                  << ", fieldName: " << fn.toString();
         if (fn.isValid())
         {
            if (fn.toString() == field_name)
            {
               rv = wdt;
               break;
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
         qDebug() << "TextEdit";
         static_cast<QcjTextBlockEdit*>(wdt)->setText(val.toString());
      }
      else if ( isA(wdt, "QLineEdit") ) 
      {
         qDebug() << "LineEdit";
         static_cast<QLineEdit*>(wdt)->setText(val.toString());
      }
      else if ( isA(wdt, "QSpinBox") )
      {
         qDebug() << "spin";
         static_cast<QSpinBox*>(wdt)->setValue(val.toInt());
      }
      else if (isA(wdt, "QDoubleSpinBox"))
      {
         qDebug() << "double spin";
         static_cast<QDoubleSpinBox*>(wdt)->setValue(val.toDouble());
      }
      else if ( isA(wdt, "QcjPhotoEntry") ) 
      {
         qDebug() << "image";
         static_cast<QcjPhotoEntry*>(wdt)->set(val.toByteArray());
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
      if ( isA(wdt, "QcjTextBlockEdit") ) 
      {
         rv = QVariant::String;
      }
      if ( isA(wdt, "QcjMoneyEdit") ) 
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
      else if ( isA(wdt, "QLineEdit") ) 
      {
         rv = QVariant(static_cast<QLineEdit*>(wdt)->text());
      }
      else if ( isA(wdt, "QSpinBox") )
      {
         rv = QVariant(static_cast<QSpinBox*>(wdt)->value());
      }
      else if (isA(wdt, "QDoubleSpinBox"))
      {
         rv = QVariant(static_cast<QDoubleSpinBox*>(wdt)->value());
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
};
