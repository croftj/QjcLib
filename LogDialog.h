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
#ifndef LOGDIALOG_H
#define LOGDIALOG_H

# include <QDialog>
# include <QSettings>

# include "LogRegistery.h"
# include "LogLevelWidget.h"

# include "ui_logdialog.h"
#include <iostream>

namespace QcjLib
{
   class LogDialog : public QDialog
   {
      Q_OBJECT

   public:
      LogDialog(QWidget *parent = NULL) : 
         QDialog(parent, Qt::Dialog | Qt::WindowMinMaxButtonsHint)
      {
         m_ui.setupUi(this);
         m_ui.logTableWidget->Initialize();

         connect(m_ui.fileCheckBox, SIGNAL(stateChanged(int)), this, SLOT(SlotSetFileEnable(int)), Qt::UniqueConnection);
         connect(m_ui.browseBtn,    SIGNAL(clicked()),         this, SLOT(SlotBrowseFiles()), Qt::UniqueConnection);

         QSettings settings;
         m_ui.consoleCheckBox->setChecked(settings.value(LOG_CONSOLE_ENABLE, true).toBool());
         m_ui.viewCheckBox->setChecked(settings.value(LOG_VIEW_ENABLE, true).toBool());
         m_ui.fileCheckBox->setChecked(settings.value(LOG_FILE_ENABLE, true).toBool());
         m_ui.fileNameEdit->setText(settings.value(LOG_FILE_NAME, "./app.log").toString());
         std::cout << "here" << std::endl;
      }

      static bool isViewEnabled()
      {
         QSettings settings;
         return(settings.value(LOG_VIEW_ENABLE, true).toBool());
      }

      static void InitializeLogger();

   public slots:
      void accept();
      void SlotSetFileEnable(int check_state);
      void SlotBrowseFiles();

   protected:
   private:
      Ui::LogDialog  m_ui;
   };
}

#endif
