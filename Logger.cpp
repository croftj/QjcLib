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
# include "Logger.h"
# include "LogRegistery.h"

# include  <QFile>

# include  <stdlib.h>
# include  <stdlib.h>

using namespace QcjLib;

QcjLib::Logger* QcjLib::Logger::m_instance = NULL;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
   QString str(context.category);
//   std::cout << __FUNCTION__ <<  " str: |" << qPrintable(str) << "|" << std::endl;
   QStringList sl = str.split(":");
   QString category = sl[0];
   unsigned int level = 1;

   QString msg_buf;
   QTextStream buf(&msg_buf);

   if ( sl.count() > 1 ) 
   {
      level = sl[1].toInt();
   }
//   std::cout << __FUNCTION__ <<  " context->category: " << qPrintable(category) << ", level: " << level << ", logging level: " << QcjLib::LogRegistery::instance()->LogLevel(category) << std::endl;

   if ( QcjLib::LogRegistery::instance()->LogLevel(category) >= level ) 
   {
      buf << category << ":" << level << "::" ;
      switch (type)
      {
         case QtDebugMsg:
            buf << "Debug: ";
            break;

         case QtWarningMsg:
            buf << "Warning: ";
            break;

         case QtCriticalMsg:
            buf << "Critical: ";
            break;

         case QtFatalMsg:
            buf << "FATAL: " << msg << endl;
            QcjLib::Logger::instance()->LogMessage(msg_buf);
            abort();
      }
      buf << context.function << ": " << msg << endl;
      QcjLib::Logger::instance()->LogMessage(msg_buf);
   }
}

QcjLib::Logger::Logger(QObject *parent) : QObject(parent)
{
   QFile *out = new QFile(); 
   out->open(stdout, QIODevice::WriteOnly);
   m_consoleStream.setDevice(out);
   qInstallMessageHandler(myMessageOutput);
}

QcjLib::Logger::~Logger()
{
}


