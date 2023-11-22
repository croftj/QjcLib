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
#ifndef DBGTIMER_H
#define DBGTIMER_H

#include "LogBuilder.h"

#include <QDebug>
#include <QElapsedTimer>
#include <QString>

namespace QcjLib
{
   class DbgTimer : public QElapsedTimer
   {
   public:
      DbgTimer(QString funct, int line, int dbg = 1, int dbgThreshhold = 0)
      {
         static int  m_nextId = 0;
         m_dbg = dbg;
         m_dbgThreshhold = dbgThreshhold;
         if ( m_dbg > m_dbgThreshhold ) 
         {
            m_timerId = m_nextId++;
            m_funct = funct;
            m_line = line;
            qDebug(*QcjLib::log(LOG, 1)) << "ST:0    Timer [" << m_timerId << ":" << m_funct << "@" << m_line << "] started!";
            start();
         }
      }

      ~DbgTimer()
      {
         if ( m_dbg > m_dbgThreshhold ) 
         {
            qDebug(*QcjLib::log(LOG, 1)) << "ET:" << elapsed() <<"\tTimer [" << m_timerId << ":" << m_funct << "@" << m_line << "]";
         }
      }

      void snapShot()
      {
         if ( m_dbg > m_dbgThreshhold ) 
         {
            qDebug(*QcjLib::log(LOG, 1)) << "SS:" << elapsed() <<"\tTimer [" << m_timerId << ":" << m_funct << "@" << m_line << "]";
         }
      }

      static const QString LOG;

   private:
      int         m_timerId;
      QString     m_funct;
      int         m_line;
      int         m_dbg;
      int         m_dbgThreshhold;
   };
}

#endif
