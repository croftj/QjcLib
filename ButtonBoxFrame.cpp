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
#include "QcjLib/ButtonBoxFrame.h"
#include "QcjLib/LinkedCheckBox.h"
#include "QcjData/QcjDataLogin.h"
#include "QcjData/QcjDataStatics.h"
#include "QcjData/QcjDataXML.h"

#include <QAction>
#include <QActionEvent>
#include <QCheckBox>
#include <QDebug>
#include <QGridLayout>
#include <QRadioButton>
#include <QSqlDriver>

using namespace QcjLib;

namespace 
{
   const int MINCOL  = 2;
   const int MINCOL1 = MINCOL + 1;
};

QcjLib::ButtonBoxFrame::ButtonBoxFrame(QWidget *parent) : QFrame(parent)
{
}   

void QcjLib::ButtonBoxFrame::setDatabase(const QStringList &items)
{
   qDebug() << "Enter: items = " << items;
   if (pDb == nullptr)
   {
      qDebug() << "Have no database";
      QcjDataLogin *login = new QcjDataLogin();
      login->initialize();
      login->show();
      if (! login->exec() )
      {
         qDebug() << "Error with login";
         fflush(stdout);
         return;
      }
      delete login;
      if ( pFormDef->database() == 0 ) 
      {
         qDebug() << "Creating pFormDef";
         pFormDef->setDatabase(pDb);
      }
   }

   std::vector<struct QcjDataFields> field_defs = pFormDef->getFields(m_xmldef, nullptr);
   qDebug() << "field_defs size:" << field_defs.size();

   /********************************************************************/
   /* Create a model for the table and fetch all of the records (there */
   /* shouldn't bee too many)                                          */
   /********************************************************************/
   QSqlDriver *drv = pDb->driver();
   QString db_table = pFormDef->getTable(m_xmldef);
   qDebug() << "table =" << db_table;
   QSqlTableModel model(nullptr, *pDb);
   model.setTable(db_table);
   if (field_defs.size() > 1)
   {
      QString fn = drv->escapeIdentifier(field_defs[1].dataName, QSqlDriver::FieldName);
      QString fv;
      foreach (QString val, items)
      {
         val.remove("'");
         if (fv.length() > 0)
         {
            fv += ", ";
         }
         fv += QString("'%1'").arg(val);
      }
      model.setFilter(QString("%1 in (%2)").arg(fn).arg(fv));
      qDebug() << "filter = " << model.filter();
   }
   model.select();
   while (model.canFetchMore())
   {
      model.fetchMore();
   }

   /**********************************************************/
   /* Now calculate the number columns of buttons to present */
   /**********************************************************/
   int rows = model.rowCount();
   qDebug() << "rows:" << rows;
   if (rows > 0)
   {
      int cols;
      if ((rows % MINCOL == 0) || (rows % MINCOL1 == 0)  || ((rows % MINCOL) >= (rows % MINCOL1)))
      {
         cols = MINCOL;
      }
      else
      {
         cols = MINCOL1;
      }

      cols = 2;
      QGridLayout *grid = new QGridLayout(this);
      int row = 0;
      int col = 0;
      QAbstractButton *btn = nullptr;
      for (int recidx = 0; recidx < rows; recidx++)
      {
         QSqlRecord rec = model.record(recidx);
         QString btn_text = rec.value(field_defs[0].dataName).toString();
         if (field_defs[0].fieldType.contains("radio"))
         {
            btn = new QRadioButton(btn_text, this);
         }
         else
         {
            qDebug() << "Creating LinkedCheckBox, text = " << btn_text;
            btn = new LinkedCheckBox(btn_text,
                                    dynamic_cast<LinkedCheckBox*>(btn), this);
            connect(btn, SIGNAL(lastButton()), this, SLOT(focusNext()));
         }
         grid->addWidget(btn, row, col);
         qDebug() << "row= " << row << ", col = " << col;

         if (++col >= cols)
         {
            row++;
            col = 0;
         }
      }
   }
   else
   {
      hide();
   }
}

void QcjLib::ButtonBoxFrame::focusNext()
{
   qDebug() << "1:Moving focus? " << focusNextChild();
   qDebug() << "2:Moving focus? " << focusNextChild();
}

QStringList QcjLib::ButtonBoxFrame::checkedButtons()
{
   QStringList rv;
   foreach (QObject *obj, children())
   {
      QAbstractButton *btn = dynamic_cast<QAbstractButton*>(obj);
      if (btn != nullptr && btn->isChecked())
      {
         rv << btn->text();
      }
   }
   return(rv);
}

QStringList QcjLib::ButtonBoxFrame::buttonList()
{
   QStringList rv;
   foreach (QObject *obj, children())
   {
      QAbstractButton *btn = dynamic_cast<QAbstractButton*>(obj);
      if (btn != nullptr)
      {
         rv << btn->text();
      }
   }
   return(rv);
}

QAbstractButton *QcjLib::ButtonBoxFrame::findButton(const QString &name)
{
   QAbstractButton *rv = nullptr;
   foreach (QObject *obj, children())
   {
      QAbstractButton *btn = dynamic_cast<QAbstractButton*>(obj);
      if (btn != nullptr)
      {
         qDebug() << "button name: " << btn->text();
      }
      if (btn != nullptr && btn->text() == name)
      {
         rv = btn;
         break;
      }
   }
   return(rv);
}

void QcjLib::ButtonBoxFrame::checkButtons(const QStringList &names)
{
   foreach (QString name, names)
   {
      qDebug() << "Testing for button: " << name;
      QAbstractButton *btn = findButton(name);
      if (btn != nullptr)
      {
         btn->setChecked(true);
      }
   }
}

