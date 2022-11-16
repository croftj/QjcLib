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
#ifndef LOGGER_H
#define LOGGER_H

# include <QDebug>
# include <QFile>
# include <QFileInfo>
# include <QIODevice>
# include <QMessageLogContext>
# include <QObject>
# include <QString>
# include <QTextStream>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

namespace QcjLib
{
   class Logger : QObject 
   {
      Q_OBJECT

   public:
      Logger(QObject *parent = NULL);
      ~Logger();

      static Logger *instance()
      {
         if ( m_instance == NULL ) 
         {
            m_instance = new Logger();
         }
         return(m_instance);
      }

      bool SetLogFile(QString filename)
      {
         bool rv = true;

         if ( ! filename.isEmpty() ) 
         {
            QString rename = filename + ".1";
            if ( QFileInfo::exists(filename) ) 
            {
               std::cout << __FUNCTION__ << " Renaming existing log file" << std::endl;
               QFile::rename(filename, rename);
            }

            m_logFile.setFileName(filename);
            std::cout << __FUNCTION__ << " Opeing log file" << std::endl;
            if ( (rv = m_logFile.open(QIODevice::WriteOnly)) ) 
            {
               if ( m_deviceStream.device() != NULL &&
                    m_deviceStream.device()->isOpen() ) 
               {
                  m_deviceStream.device()->close();
               }
               m_deviceStream.setDevice(&m_logFile);
            }
         }
         else if ( m_logFile.isOpen() ) 
         {
            m_logFile.close();
         }
         return(rv);
      }

      void LogMessage(QString msg)
      {
         if ( m_consoleEnable ) 
         {
            m_consoleStream << msg;
            m_consoleStream.flush();
         }
         if ( m_deviceStream.device() != NULL &&
              m_deviceStream.device()->isOpen() ) 
         {
            m_deviceStream << msg;
            m_deviceStream.flush();
         }
         emit LogEntry(msg);
      }

      void EnableConsole(bool enable)
      {
         m_consoleEnable = enable;
      }

   signals:
      void  LogEntry(QString msg);

   protected:
   private:
      void LogOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
      
      bool           m_consoleEnable;
      QTextStream    m_consoleStream;
      QTextStream    m_deviceStream;
      QFile          m_logFile;
      static Logger  *m_instance;
   };
};

#endif
