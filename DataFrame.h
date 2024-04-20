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
#ifndef QCJLIB_DATAFRAME_H
#define QCJLIB_DATAFRAME_H

# include <QAction>
# include <QFrame>
# include "CancelableFrame.h"
# include "DataWidgets.h"
# include "../QcjData/Qcj.h"
# include "../QcjData/QcjDataTable.h"
#include <stdlib.h>
#include <stdio.h>

namespace QcjLib
{
   class DataFrame : public CancelableFrame
   {
      Q_OBJECT

   public:
      DataFrame(QWidget *pParent = 0);

      bool cancel();

      void setDataForm(AutoDataForm *form) 
      {
         printf("DataFrame::setDataForm(): Enter, Setting form pointer to %ld\n", (unsigned long)form);
         m_form = form;
         if ( m_form != 0 ) 
         {
            connect(m_form, SIGNAL(updated()), this, SLOT(haveUpdated()), Qt::UniqueConnection);
         }
         printf("DataFrame::setDataForm(): Exit\n");
      };

      void setDataTable(QcjDataTable *table)
      {
         printf("DataFrame::setDataTable(): Enter, setting table point to %ld\n", (unsigned long)table);
         m_table = table;
         if ( m_table != 0 ) 
         {
            m_table->setFocusPolicy(Qt::NoFocus);
            connect(m_table, SIGNAL(rowSelected(QSqlRecord*)), this, SLOT(haveRowSelected(QSqlRecord*)), Qt::UniqueConnection);
            connect(m_table, SIGNAL(rowActivated(QSqlRecord*)), this, SLOT(haveRowActivated(QSqlRecord*)), Qt::UniqueConnection);
            if ( m_form != 0 ) 
               connect(m_table, SIGNAL(emptyTable()), m_form, SLOT(clearForm()), Qt::UniqueConnection);
         }
         printf("DataFrame::setDataTable(): Exit\n");
      };

      void enableAction(Qcj::Action act, bool f)
      {
         m_enabled[act] = f;
      };

      void connectButton(Qcj::Action act, QAbstractButton *btn);

      void setValidationString(QString str)
      {
         m_validString = str;
      };

      QString validationString()
      {
         return(m_validString);
      };

      bool hasChanges()
      {
         return(m_form->hasChanges());
      }

      void  setDatabase();
      void  setTableFilter(QString filter);
      bool  validate();
      int   rowCount() const;

   signals:
      void actionActivated(Qcj::Action);
      void clearForm();
      void deleteRecord();
      void insertRecord();
      void nextRow();
      void updateRecord();
      void findRows();
      void prevRow();
      void frameState(Qcj::FrameState);

   public slots:
      void setState(Qcj::FrameState newState);

   protected slots:
      virtual void haveActivatedAction(bool = false);
      virtual void haveCancelAction(bool = false);
      virtual void haveClearAction(bool = false);
      virtual void haveDelAction(bool = false);
      virtual void haveDownAction(bool = false);
      virtual void haveEditAction(bool = false);
      virtual void haveNewAction(bool = false);
      virtual void haveSaveAction(bool = false);
      virtual void haveSearchAction(bool = false);
      virtual void haveUpAction(bool = false);

      void haveRowSelected(QSqlRecord*);
      void haveRowActivated(QSqlRecord *rec);
      void haveUpdated();
      void setFormVisible(bool hide);

   protected:
      bool                    m_enabled[Qcj::Actions];
      Qcj::FrameState         m_state;
      QcjLib::AutoDataForm    *m_form;
      QcjDataTable            *m_table;

   private:
      QAbstractButton*        m_buttons[Qcj::Actions];
      QAction*                m_cancelAction;
      QAction*                m_activatedAction;
      QAction*                m_clearAction;
      QAction*                m_delAction;
      QAction*                m_editAction;
      QAction*                m_newAction;
      QAction*                m_downAction;
      QAction*                m_saveAction;
      QAction*                m_searchAction;
      QAction*                m_upAction;

      QString                 m_validString;   /* validation string */

   };
}

#endif
