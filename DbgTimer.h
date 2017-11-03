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
