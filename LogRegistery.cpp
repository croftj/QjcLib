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
# include "LogRegistery.h"

# include "LogBuilder.h"

using namespace QcjLib;

static LogBuilder mylog("default", 1, "Catches the undefined debug messages");

LogRegistery* QcjLib::LogRegistery::m_instance = NULL;

/********************************************************************//*
**   LogRegistery::LogRegistery(QObject *parent) constructor
**   
**   Function  to  build the full name of the log which is comprised
**   of its name and level
**   
**   Parameters
**   
**   parent pointer to the parent QObject
**   
**   Returns N/A
***********************************************************************/
LogRegistery::LogRegistery(QObject *parent) : QObject(parent), m_defaultCategory(new QLoggingCategory("default"))
{
}

/********************************************************************//*
**   void  LogRegistery::RegisterLog(QString  log_name, unsigned int levels, QString description)
**   
**   Function to define the actual log categories to be used for the
**   requested log
**   
**   Parameters
**   
**   log_name     Name of the log
**   
**   levels       Maximum number of levels to define for this log
**   
**   description  String describint the log.
**   
**   Returns N/A
***********************************************************************/
void LogRegistery::RegisterLog(QString log_name, unsigned int levels, QString description)
{
//   std::cout << __FUNCTION__ <<  " log_name: " << qPrintable(log_name) << std::endl;
//   std::cout << __FUNCTION__ <<  " log_name: " << std::endl;
//   m_logLevels.insert(log_name, levels);
   if ( ! m_logLevels.contains(log_name) ) 
   {
      m_logLevels.insert(log_name, 1);
   }

   m_logDescriptions.insert(log_name, description);

   m_logMaxLevels.insert(log_name, levels);
   for (unsigned int level = 1; level <= levels; level++) 
   {
      QString full_name = BuildName(log_name, level);
      char *name_buf = new char[strlen(qPrintable(full_name)) + 2];
      strncpy(name_buf, qPrintable(full_name), strlen(qPrintable(full_name)) + 1);
//      std::cout << __FUNCTION__ << " full_name: " << name_buf << std::endl;

//      std::cout << __FUNCTION__ << " full_name: " << std::endl;
      QLoggingCategory *category = new QLoggingCategory(name_buf);
//      std::cout << __FUNCTION__ <<  " cat->name(" << (unsigned long)category << "): " << category->categoryName() << " (" << (unsigned long long)(category->categoryName()) << ")" << std::endl;
      m_logMap.insert(full_name, category);
//      foreach (QString name, m_logMap.keys())
//      {
//         QLoggingCategory *cat = m_logMap.value(name);
//         std::cout << __FUNCTION__ << " cat(" << (unsigned long)cat << "): " << qPrintable(name) << ", cat->name: " << (unsigned long long)(cat->categoryName()) << std::endl;
//      }
   }
//         emit LogRegistered();
}

/********************************************************************//*
**   Function  saves the settings for each of the individual logs to
**   QSettings
**   
**   Returns N/A
***********************************************************************/
void QcjLib::LogRegistery::SaveLogSettings()
{
   QSettings settings;

   QString log_settings;
   foreach (QString log_name, m_logLevels.keys())
   {
      QString descr = m_logDescriptions.value(log_name);
      QString level = QString::number(m_logLevels.value(log_name));
      log_settings += (log_settings.size() > 0) ? "," : "";
      log_settings += log_name + ":" + descr + ":" + level;
   }
   settings.setValue(LOG_STATUS, log_settings);
}

QLoggingCategory *QcjLib::log(QString log_name, unsigned int level)
{
   return(QcjLib::LogRegistery::instance()->category(log_name, level));
}



