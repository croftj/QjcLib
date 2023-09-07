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
#include "TableView.h"

#include "../QcjData/QcjDataStatics.h"
#include "../QcjData/QcjDataHelpers.h"
#include "GenericItemDelegates.h"
#include "GenericTableModel.h"

using namespace QcjLib;

const QString TableView::LOG("QcjLib_table_view");
static LogBuilder mylog(TableView::LOG, 1, "QcjLib Table View");

void TableView::setFields(int row)
{
   std::vector<struct QcjDataFields> fieldDefs;
   qDebug() << __FUNCTION__ << "(): calling getFields for entry" << m_xmldef;
   fieldDefs = pFormDef->getFields(m_xmldef, this);
   qDebug() << __FUNCTION__ << "(): Have" << fieldDefs.size() << "fields defined";

   GenericTableModel *model_ptr = dynamic_cast<GenericTableModel*>(model());
   /*******************************************************************/
   /* Iterate through each column. If the column has a type the has a */
   /* defined delegate, create the delegate and set it.               */
   /*******************************************************************/
   int column = 0;
   foreach (struct QcjDataFields field, fieldDefs)
   {
      /*******************************************************************/
      /* First, hide the widget is it was created. It should've been. It */
      /* will be used as the template for the delegates edit widget.     */
      /*******************************************************************/
      if ( ! m_haveDelegates)
      {
       qDebug(*log(LOG, 1)) << "Have field: " << field.label;
         if (field.widget != nullptr)
         {
            field.widget->hide();
         }

         QString fieldType = field.fieldType;
         if (fieldType == "integer")
         {
            GenericIntDelegate *delegate = new GenericIntDelegate(field, this);
            setItemDelegateForColumn(column, delegate);
         }
         else if (fieldType == "double")
         {
            GenericDoubleDelegate *delegate = new GenericDoubleDelegate(field, this);
            setItemDelegateForColumn(column, delegate);
         }
         else if (fieldType == "money")
         {
            GenericMoneyDelegate *delegate = new GenericMoneyDelegate(field, this);
            setItemDelegateForColumn(column, delegate);
            model_ptr->setData(model_ptr->index(row, column), 
                               QVariant(Qt::AlignRight), Qt::TextAlignmentRole);
         }
         else if (fieldType == "strsel")
         {
            GenericStringSelectDelegate *delegate = new GenericStringSelectDelegate(field, this);
            setItemDelegateForColumn(column, delegate);
         }
         else if (fieldType == "yesno")
         {
            GenericYesNoDelegate *delegate = new GenericYesNoDelegate(field, this);
            setItemDelegateForColumn(column, delegate);
         }
         else if (fieldType == "phone")
         {
            GenericPhoneDelegate *delegate = new GenericPhoneDelegate(field, this);
            setItemDelegateForColumn(column, delegate);
         }
      }
      if (model_ptr != nullptr && field.dataName != "--ENDOFFIELDS--")
      {
         qDebug(*log(LOG, 1)) << "Adding column label: " << field.label
                              << ", dataName: " << field.dataName;
         if (row == 0)
         {
            model_ptr->AddColumn(field.label, field.dataName);
         }
         
         QString defaultValue(field.defvalue);
         if (defaultValue.startsWith("config:"))
         {
            QStringList sl = defaultValue.split(":");
            qDebug(*log(LOG, 1)) << "Using setting " << sl[1];
            defaultValue = pConfig->value(sl[1], QVariant("")).toString();
         }
         qDebug(*log(LOG, 1)) << "Setting default value for " << field.label << ": " << defaultValue;
         model_ptr->SetValue(row, field.label, defaultValue);

      }
      column++;
   }
   setCurrentIndex(model_ptr->index(0, 0));
   m_haveDelegates = true;
   QEvent evt = QEvent(QEvent::Enter);
   TableView::event(&evt);
}

bool TableView::focusInEvent(QEvent *evt)
{
   if (true && evt->type() != QEvent::Timer)
   {
      qDebug() << __FUNCTION__ << "Have event: " << evt->type();
   }
   return(false);
}

bool TableView::event(QEvent *evt)
{
   if (true && evt->type() == QEvent::FocusIn)
   {
      qDebug() << __FUNCTION__ << "Have event: " << evt->type();
      if ( currentIndex().isValid())
      {
         edit(currentIndex());
      }
      else
      {
         qDebug() << "model ptr = " << (long int)model();
         if (model() != nullptr)
            edit(model()->index(0, 0));
      }

   }
   else if (true && evt->type() != QEvent::Timer)
   {
      qDebug() << __FUNCTION__ << "Have event: " << evt->type();
   }
   return(false);
}

void TableView::keyPressEvent(QKeyEvent *evt)
{
   qDebug() << __FUNCTION__ << "Have key: " << evt->key();
   if (evt->key() != Qt::Key_Enter && evt->key() != Qt::Key_Return)
   {
      QTableView::keyPressEvent(evt);
   }
}

