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
# include "LogDialog.h"

# include <QFile>
# include <QFileDialog>
# include <QMessageBox>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

# include "Logger.h"

using namespace QcjLib;

void LogDialog::accept()
{
   bool valid = true;
   if ( m_ui.fileCheckBox->isChecked() ) 
   {
      QFile log(m_ui.fileNameEdit->text());
      if ( ! Logger::instance()->SetLogFile(m_ui.fileNameEdit->text()) ) 
      {
         QMessageBox::critical(NULL, "Error Opening Log File", "File '" 
                                   + m_ui.fileNameEdit->text()
                                   + " could not be opened");
         valid = false;
      }
   }
   else 
   {
      Logger::instance()->SetLogFile("");
   }

   if ( valid ) 
   {
      Logger::instance()->EnableConsole(m_ui.consoleCheckBox->isChecked());

      QSettings settings;
      settings.setValue(QcjLib::LOG_CONSOLE_ENABLE, m_ui.consoleCheckBox->isChecked());
      settings.setValue(QcjLib::LOG_VIEW_ENABLE,    m_ui.viewCheckBox->isChecked());
      settings.setValue(QcjLib::LOG_FILE_ENABLE,    m_ui.fileCheckBox->isChecked());
      settings.setValue(QcjLib::LOG_FILE_NAME,      m_ui.fileNameEdit->text());

      qDebug() << __FUNCTION__ << "Saving settings";
      m_ui.logTableWidget->ApplySettings();
      LogRegistery::instance()->SaveLogSettings();

      QDialog::accept();
   }
}

void LogDialog::SlotSetFileEnable(int)
{
   m_ui.fileNameEdit->setVisible(m_ui.fileCheckBox->isChecked());
   m_ui.browseBtn->setVisible(m_ui.fileCheckBox->isChecked());
}


void LogDialog::SlotBrowseFiles()
{
   m_ui.fileNameEdit->setText(QFileDialog::getOpenFileName(this, "Log File", m_ui.fileNameEdit->text()));
}

void LogDialog::InitializeLogger()
{
   QSettings settings;
   if ( settings.value(QcjLib::LOG_FILE_ENABLE, true).toBool() )
   {
      Logger::instance()->SetLogFile(settings.value(QcjLib::LOG_FILE_NAME, "").toString());
   }

   bool console_enable = settings.value(QcjLib::LOG_CONSOLE_ENABLE, true).toBool();
   std::cout << "console_enable: " << (int)console_enable << std::endl;
   Logger::instance()->EnableConsole(console_enable);
}

