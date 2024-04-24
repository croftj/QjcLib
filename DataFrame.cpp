/******************************************************************************/
/* $Id$                              */
/*                                                                            */
/* Author: Joe Croft, joe@croftj.net 2023-2024                                     */
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
# include <QAbstractButton>
# include <QDebug>
# include <QMessageBox>

# include "DataFrame.h"
# include "QcjData/QcjDataStatics.h"
# include "QcjLib/WidgetUtils.h"

using namespace QcjLib;

DataFrame::DataFrame(QWidget *pParent) :
   CancelableFrame(pParent)
{
#ifndef QT4_DESIGNER_PLUGIN
   printf("QcjLib::DataFrame::DataFrame(): Enter\n");
   fflush(stdout);
   m_activatedAction = new QAction(this);
   m_activatedAction->setShortcut(Qt::Key_Return);
   m_activatedAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
   connect(m_activatedAction, SIGNAL(triggered(bool)), this, SLOT(haveActivatedAction(bool)), Qt::UniqueConnection);
   addAction(m_activatedAction);

   m_cancelAction = new QAction(this);
   m_cancelAction->setShortcut(Qt::CTRL + Qt::Key_Escape);
   m_cancelAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
   connect(m_cancelAction, SIGNAL(triggered(bool)), this, SLOT(haveCancelAction(bool)), Qt::UniqueConnection);
   addAction(m_cancelAction);

   m_clearAction = new QAction(this);
   m_clearAction->setShortcut(Qt::Key_F10);
   m_clearAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
   connect(m_clearAction, SIGNAL(triggered(bool)), this, SLOT(haveClearAction(bool)), Qt::UniqueConnection);
   addAction(m_clearAction);

   m_delAction = new QAction(this);
   m_delAction->setShortcut(QKeySequence::DeleteEndOfWord);
   m_delAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
   connect(m_delAction, SIGNAL(triggered(bool)), this, SLOT(haveDelAction(bool)), Qt::UniqueConnection);
   addAction(m_delAction);

   m_downAction = new QAction(this);
   m_downAction->setShortcut(Qt::ALT + Qt::Key_Down);
   m_downAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
   connect(m_downAction, SIGNAL(triggered(bool)), this, SLOT(haveDownAction(bool)), Qt::UniqueConnection);
   addAction(m_downAction);

   m_editAction = new QAction(this);
   m_editAction->setShortcut(QKeySequence::Open);
   m_editAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
   connect(m_editAction, SIGNAL(triggered(bool)), this, SLOT(haveEditAction(bool)), Qt::UniqueConnection);
   addAction(m_editAction);

   m_newAction = new QAction(this);
   m_newAction->setShortcut(QKeySequence::New);
   m_newAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
   connect(m_newAction, SIGNAL(triggered(bool)), this, SLOT(haveNewAction(bool)), Qt::UniqueConnection);
   addAction(m_newAction);

   m_saveAction = new QAction(this);
   m_saveAction->setShortcut(QKeySequence::Save);
   m_saveAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
   connect(m_saveAction, SIGNAL(triggered(bool)), this, SLOT(haveSaveAction(bool)), Qt::UniqueConnection);
   addAction(m_saveAction);

   m_searchAction = new QAction(this);
   m_searchAction->setShortcut(QKeySequence::FindNext);
   m_searchAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
   connect(m_searchAction, SIGNAL(triggered(bool)), this, SLOT(haveSearchAction(bool)), Qt::UniqueConnection);
   addAction(m_searchAction);

   m_upAction = new QAction(this);
   m_upAction->setShortcut(Qt::ALT + Qt::Key_Up);
   m_upAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
   connect(m_upAction, SIGNAL(triggered(bool)), this, SLOT(haveUpAction(bool)), Qt::UniqueConnection);
   addAction(m_upAction);

   for (int x = 0; x < Qcj::Actions; x++) 
   {
      m_enabled[x] = true;
      m_buttons[x] = 0;
   }
   m_form = 0;
   m_table = 0;

#else
   setFrameShape(QFrame::Box);
#endif
}

bool DataFrame::cancel()
{
   if (m_form != nullptr)
   {
      return(m_form->cancel());
   }
}

void DataFrame::setDatabase()
{
   if (m_form == nullptr)
   {
      qDebug() << "Searching for existing forms and tables";
      QList<QcjLib::AutoDataForm*> data_forms = findChildren<QcjLib::AutoDataForm*>();
      qDebug() << WidgetUtils::objectType(this) << ", found " << data_forms.count() << " autodata forms";
      if (data_forms.count() > 0)
      {
         m_form = data_forms.at(0);
      }
   }

   if (m_table == nullptr)
   {
      QList<QcjDataTable*> data_tables = findChildren<QcjDataTable*>();
      qDebug() << "found " << data_tables.count() << " data tables";
      if (data_tables.count() > 0)
      {
         m_table = data_tables.at(0);
      }
   }

   if (m_table == nullptr || m_form == nullptr)
   {
      QString msg(QStringLiteral("Data frame %1 is missing table or form")
               .arg(parent()->objectName()));
      QMessageBox::warning(NULL, "Warning", msg); 
      qDebug() << WidgetUtils::objectType(this) << " *** Error:" << msg;
   }
   else
   {
      m_form->setDatabase();
      m_table->setDatabase();
      connect(m_table, SIGNAL(rowSelected(QSqlRecord*)), m_form, SLOT(refresh(QSqlRecord*)), Qt::UniqueConnection);
   }
}

void DataFrame::connectButton(Qcj::Action act, QAbstractButton *btn)
{
#ifndef QT4_DESIGNER_PLUGIN
   qDebug() << QString("Connecting button %1, action %2").arg(btn->text())
                                                         .arg(act);
   switch (act) 
   {
      case Qcj::ActivatedAction:
         qDebug() << QString("Connecting Activated Action");
         connect(btn, SIGNAL(clicked()), this, SLOT(haveActivatedAction()), Qt::UniqueConnection);
         m_buttons[act] = btn;
         break;

      case Qcj::CancelAction:
         qDebug() << QString("Connecting Cancel Action");
         connect(btn, SIGNAL(clicked()), this, SLOT(haveCancelAction()), Qt::UniqueConnection);
         m_buttons[act] = btn;
         break;

      case Qcj::ClearAction:
         qDebug() << QString("Connecting Clear Action");
         connect(btn, SIGNAL(clicked()), this, SLOT(haveClearAction()), Qt::UniqueConnection);
         m_buttons[act] = btn;
         break;

      case Qcj::DelAction:
         qDebug() << QString("Connecting Del Action");
         connect(btn, SIGNAL(clicked()), this, SLOT(haveDelAction()), Qt::UniqueConnection);
         m_buttons[act] = btn;
         break;
      
      case Qcj::DownAction:
         qDebug() << QString("Connecting Down Action");
         connect(btn, SIGNAL(clicked()), this, SLOT(haveDownAction()), Qt::UniqueConnection);
         m_buttons[act] = btn;
         break;
      
      case Qcj::EditAction:
         qDebug() << QString("Connecting Edit Action");
         connect(btn, SIGNAL(clicked()), this, SLOT(haveEditAction()), Qt::UniqueConnection);
         m_buttons[act] = btn;
         break;
      
      case Qcj::NewAction:
         qDebug() << QString("Connecting New Action");
         connect(btn, SIGNAL(clicked()), this, SLOT(haveNewAction()), Qt::UniqueConnection);
         m_buttons[act] = btn;
         break;
      
      case Qcj::SaveAction:
         qDebug() << QString("Connecting Save Action");
         connect(btn, SIGNAL(clicked()), this, SLOT(haveSaveAction()), Qt::UniqueConnection);
         m_buttons[act] = btn;
         break;
      
      case Qcj::SearchAction:
         qDebug() << QString("Connecting Search Action");
         connect(btn, SIGNAL(clicked()), this, SLOT(haveSearchAction()), Qt::UniqueConnection);
         m_buttons[act] = btn;
         break;

      case Qcj::UpAction:
         qDebug() << QString("Connecting Up Action");
         connect(btn, SIGNAL(clicked()), this, SLOT(haveUpAction()), Qt::UniqueConnection);
         m_buttons[act] = btn;
         break;
      
      default:
         break;
   }

   printf("QcjLib::DataFrame::connectButton(): Exit\n");
   fflush(stdout);
#endif
}

void QcjLib::DataFrame::haveActivatedAction(bool)
{
#ifndef QT4_DESIGNER_PLUGIN
   qDebug() << "Enter";
   if ( m_table != 0 ) 
   {
      m_table->activateCurrentRecord();
      qDebug() << "Emmitting ActivatedAction";
      emit actionActivated(Qcj::ActivatedAction);
   }
#endif
}

void QcjLib::DataFrame::haveCancelAction(bool)
{
#ifndef QT4_DESIGNER_PLUGIN
   printf("QcjLib::DataFrame::haveClearAction(): Enter\n");
   fflush(stdout);
   printf("QcjLib::DataFrame::haveClearAction(): Exit\n");
   fflush(stdout);
#endif
   emit actionActivated(Qcj::CancelAction);
}

void QcjLib::DataFrame::haveClearAction(bool)
{
#ifndef QT4_DESIGNER_PLUGIN
   printf("QcjLib::DataFrame::haveClearAction(): Enter\n");
   fflush(stdout);
   if ( m_enabled[Qcj::ClearAction] ) 
   {
      printf("QcjLib::DataFrame::haveClearAction(): Action Enabled\n");
      if ( m_form != 0 ) 
      {
         if ( m_table != 0 ) 
         {
            m_table->setFilter("");
            m_table->selectRow(0);
         }
         m_form->clearForm();
      }
      setState(Qcj::Search);
      emit clearForm();
   }
   printf("QcjLib::DataFrame::haveClearAction(): Exit\n");
   fflush(stdout);
#endif
   emit actionActivated(Qcj::ClearAction);
}

void QcjLib::DataFrame::haveDelAction(bool)
{
#ifndef QT4_DESIGNER_PLUGIN
   if ( m_state != Qcj::Search ) 
   {
      if ( m_enabled[Qcj::DelAction] ) 
      {
         if (validate())
         {
            emit deleteRecord();
         }
      }
   }
   emit actionActivated(Qcj::DelAction);
#endif
}

void DataFrame::haveDownAction(bool)
{
#ifndef QT4_DESIGNER_PLUGIN
   if ( m_state != Qcj::Insert ) 
   {
      printf("QcjLib::DataFrame::haveDownAction(): Enter\n");
      fflush(stdout);
      if ( m_enabled[Qcj::DownAction] ) 
      {
         printf("QcjLib::DataFrame::haveDownAction(): Action Enabled\n");
         if ( m_table != 0 ) 
         {
            m_table->nextRow();
         }
      }
      emit nextRow();
      printf("QcjLib::DataFrame::haveDownAction(): Exit\n");
      fflush(stdout);
   }
#endif
}

/********************************************************************/
/* This slot does not do anything accept signal that the action was */
/* activated.                                                       */
/********************************************************************/
void DataFrame::haveEditAction(bool)
{
#ifndef QT4_DESIGNER_PLUGIN
   emit actionActivated(Qcj::EditAction);
#endif
}

void DataFrame::haveNewAction(bool)
{
#ifndef QT4_DESIGNER_PLUGIN
   printf("QcjLib::DataFrame::haveNewAction(): Enter\n");
   fflush(stdout);
   if ( m_enabled[Qcj::NewAction] ) 
   {
      printf("QcjLib::DataFrame::haveNewAction(): Action enabled\n");
      fflush(stdout);
      if ( m_form != 0 )
      {
         printf("QcjLib::DataFrame::haveNewAction(): Form present\n");
         fflush(stdout);
         if ( validate() ) 
         {
            QSqlRecord customer_rec = m_form->insertRecord();
            m_form->refresh(&customer_rec);
            printf("QcjLib::DataFrame::haveNewAction(): Emitting insertRecord signal\n");
            emit insertRecord();
         }
      }
      setState(Qcj::Insert);
   }
   else if ( validate() ) 
      emit insertRecord();
   printf("QcjLib::DataFrame::haveNewAction(): Emitting NewAction signal\n");
   emit actionActivated(Qcj::NewAction);
   printf("QcjLib::DataFrame::haveNewAction(): Exit\n");
   fflush(stdout);
#endif
}

void DataFrame::haveSaveAction(bool)
{
#ifndef QT4_DESIGNER_PLUGIN
   if ( m_state != Qcj::Search ) 
   {
      printf("QcjLib::DataFrame::haveSaveAction(): Enter\n");
      fflush(stdout);
      if ( m_enabled[Qcj::SaveAction] ) 
      {
         if ( m_form != 0 ) 
         {
            qDebug() << "Validating form data";
            if ( validate() ) 
            {
               qDebug() << "Updating record";
               m_form->updateRecord();
               if ( m_table != 0 ) 
                  m_table->findRow(m_form->m_indexMap);
               emit updateRecord();
            }
         }
         setState(Qcj::Updated);
      }
      else if ( validate() ) 
         emit updateRecord();
      printf("QcjLib::DataFrame::haveSaveAction(): Exit\n");
      fflush(stdout);
   }
   emit actionActivated(Qcj::SaveAction);
#endif
}

void DataFrame::haveSearchAction(bool)
{
#ifndef QT4_DESIGNER_PLUGIN
   if ( m_state != Qcj::Insert ) 
   {
      printf("QcjLib::DataFrame::haveSearchAction(): Enter\n");
      fflush(stdout);
      if ( m_table && m_form ) 
      {
         printf("QcjLib::DataFrame::haveSearchAction(): Action Enabled\n");
         fflush(stdout);
         QString filter = m_form->makeFilter();
         m_table->setFilter(filter);
         m_table->selectRow(0);
         setState(Qcj::Updated);
      }
      emit findRows();
      printf("QcjLib::DataFrame::haveSearchAction(): Exit\n");
      fflush(stdout);
   }
   emit actionActivated(Qcj::SearchAction);
#endif
}

void DataFrame::haveUpAction(bool)
{
#ifndef QT4_DESIGNER_PLUGIN
   if ( m_state != Qcj::Insert ) 
   {
      printf("QcjLib::DataFrame::haveUpAction(): Enter\n");
      fflush(stdout);
      if ( m_enabled[Qcj::UpAction] ) 
      {
         printf("QcjLib::DataFrame::haveUpAction(): Action Enabled\n");
         fflush(stdout);
         if ( m_table != 0 ) 
         {
            m_table->previousRow();
         }
      }
      emit prevRow();
      printf("QcjLib::DataFrame::haveUpAction(): Exit\n");
      fflush(stdout);
      emit actionActivated(Qcj::UpAction);
   }
#endif
}

void DataFrame::setState(Qcj::FrameState newState)
{
#ifndef QT4_DESIGNER_PLUGIN
   printf("QcjLib::DataFrame::setState(): Enter, newState: %d\n", newState);
   fflush(stdout);
   QAbstractButton *btn;
   m_state = newState;

   if ( (btn = m_buttons[Qcj::ClearAction]) != 0 ) 
   {
      printf("QcjLib::DataFrame::setState(): %s clear button %d\n", ( m_state != Qcj::Insert ) ? "Enabling" : "Disabling", Qcj::ClearAction);
      fflush(stdout);
      if ( m_state != Qcj::Insert ) 
         btn->setEnabled(true);
      else
         btn->setEnabled(false);
   }

   if ( (btn = m_buttons[Qcj::DelAction]) != 0 ) 
   {
      printf("QcjLib::DataFrame::setState(): %s del button %d\n", ( m_state != Qcj::Search ) ? "Enabling" : "Disabling", Qcj::DelAction);
      fflush(stdout);
      if ( m_state != Qcj::Search ) 
         btn->setEnabled(true);
      else
         btn->setEnabled(false);
   }

   if ( (btn = m_buttons[Qcj::DownAction]) != 0 ) 
   {
      printf("QcjLib::DataFrame::setState(): %s down button %d\n", ( m_state != Qcj::Insert ) ? "Enabling" : "Disabling", Qcj::DownAction);
      fflush(stdout);
      if ( m_state != Qcj::Insert ) 
         btn->setEnabled(true);
      else
         btn->setEnabled(false);
   }

   if ( (btn = m_buttons[Qcj::NewAction]) != 0 ) 
   {
      printf("QcjLib::DataFrame::setState(): %s new button %d\n", ( m_state != Qcj::Search ) ? "Enabling" : "Disabling", Qcj::NewAction);
      fflush(stdout);
      if ( m_state != Qcj::Search ) 
         btn->setEnabled(true);
      else
         btn->setEnabled(false);
   }

   if ( (btn = m_buttons[Qcj::SaveAction]) != 0 ) 
   {
      printf("QcjLib::DataFrame::setState(): %s save button %d\n", ( m_state != Qcj::Search ) ? "Enabling" : "Disabling", Qcj::SaveAction);
      fflush(stdout);
      if ( m_state != Qcj::Search ) 
         btn->setEnabled(true);
      else
         btn->setEnabled(false);
   }

   if ( (btn = m_buttons[Qcj::SearchAction]) != 0 ) 
   {
      printf("QcjLib::DataFrame::setState(): %s search button %d\n", ( m_state != Qcj::Insert ) ? "Enabling" : "Disabling", Qcj::SearchAction);
      fflush(stdout);
      if ( m_state != Qcj::Insert ) 
         btn->setEnabled(true);
      else
         btn->setEnabled(false);
   }

   if ( (btn = m_buttons[Qcj::UpAction]) != 0 ) 
   {
      printf("QcjLib::DataFrame::setState(): %s up button %d\n", ( m_state != Qcj::Insert ) ? "Enabling" : "Disabling", Qcj::UpAction);
      fflush(stdout);
      if ( m_state != Qcj::Insert ) 
         btn->setEnabled(true);
      else
         btn->setEnabled(false);
   }

   emit frameState(m_state);
#endif
}

void DataFrame::haveRowSelected(QSqlRecord *rec)
{
#ifndef QT4_DESIGNER_PLUGIN
   printf("QcjLib::DataFrame::haveRowSelected(): Enter, m_form = %ld\n", (unsigned long)m_form);
   fflush(stdout);
   if ( m_form != 0 ) 
   {
      printf("QcjLib::DataFrame::haveRowSelected(): Refreshing form\n");
      m_form->refresh(rec);
   }
   setState(Qcj::Updated);
   printf("QcjLib::DataFrame::haveRowSelected(): Exit\n");
   fflush(stdout);
#endif
}

void DataFrame::haveRowActivated(QSqlRecord *rec)
{
#ifndef QT4_DESIGNER_PLUGIN
   printf("QcjLib::DataFrame::haveRowActivated(): Enter, m_form = %ld\n", (unsigned long)m_form);
   fflush(stdout);
   if ( m_form != 0 ) 
   {
      printf("QcjLib::DataFrame::haveRowActivated(): Refreshing form\n");
      m_form->refresh(rec);
   }
   setState(Qcj::Updated);
   emit actionActivated(Qcj::ActivatedAction);
   printf("QcjLib::DataFrame::haveRowActivated(): Exit\n");
   fflush(stdout);
#endif
}

void DataFrame::haveUpdated()
{
#ifndef QT4_DESIGNER_PLUGIN
   printf("QcjLib::DataFrame::haveUpdated(): Enter\n");
   fflush(stdout);

   if ( m_table != 0 ) 
      m_table->refresh(true);

   setState(Qcj::Updated);
   printf("QcjLib::DataFrame::haveUpdated(): Exit\n");
   fflush(stdout);
#endif
}

void DataFrame::setFormVisible(bool visible)
{
   qDebug() << "setting form visible? " << visible;
   m_form->setVisible(visible);
}

bool DataFrame::validate()
{
   bool rv = false;

   printf("QcjLib::DataFrame::validate(): Enter, m_validString length: %d\n", m_validString.size());
   fflush(stdout);
   if ( m_validString.size() > 0 && QMessageBox::warning(NULL, "Warning", m_validString + "\n", 
                              QMessageBox::Yes | QMessageBox::Default, 
                              QMessageBox::No | QMessageBox::Escape) == QMessageBox::No )
   {
      printf("QcjLib::DataFrame::validate(): Form not validated, returning false\n");
      fflush(stdout);
      rv = false;
   }
   else
      rv = true;
   printf("QcjLib::DataFrame::validate(): Exit, rv = %s\n", rv == true ? "true" : "false");
   fflush(stdout);
   return(rv);
}

void DataFrame::setTableFilter(QString filter)
{
   qDebug() << "m_table = " << (unsigned long long)m_table;
   if (m_table != nullptr)
   {
      m_table->setFilter(filter);
      m_table->refresh();
      m_table->seek(0);
   }
}

int DataFrame::rowCount() const
{
   return(m_table->rowCount());
}

