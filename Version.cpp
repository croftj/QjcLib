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
/***************************************************************************//**
* @file   Version.cpp
* 
* Class definition for About Dialog.
*
* @note    None
*
* @warning None
*******************************************************************************/

/******************************************************************************/
/* include block */
#include "Version.h"

#include <QDebug>
#include <QFile>
#include <QRegExp>
#include <QStringList>

using namespace QcjLib;

#define LOG "QcjLib_version"

static LogBuilder mylog(LOG, 1, "QcjLib Version");

QString        Version::m_Build;
QString        Version::m_CmPath;
QString        Version::m_ExtraInfo;
QString        Version::m_Major;
QString        Version::m_Minor;
QString        Version::m_Name;
QString        Version::m_Patch;
QString        Version::m_PathREStr;
QString        Version::m_Revision;
QString        Version::m_RevREStr;
QString        Version::m_Type;
bool           Version::m_Verbose;
bool           Version::m_Parsed = false;

const QString  Version::TRIAL_TAG_STR = "trial-";

/***************************************************************************//**
* Get the version string to show the user
*
*  @param[in]    verbose, If  the parameter verbose is true, the full version 
*                 string will be returned, otherwise, the m_Verbose will be 
*                 used to determine if the full version string or the shortened 
*                 version string will be returned.
*
* @return        QString to show user
* @note          none
* @warning       none
*******************************************************************************/
QString Version::GetFullVersionString(bool verbose)
{
   QString v_str;
   
   ParseVersionControlStrings();
   if ( verbose == false ) 
   {
      verbose = m_Verbose;
   }
   /***********************************************/
   /*   Double  your  pleasure, double your fun!  */
   /*   If  the  caller  wants  more text shown,  */
   /*   show the SVN Rev number and Qt, Version   */
   /***********************************************/
   if(verbose)
   {
      v_str += m_Type + " " + m_Name + "rev: " + m_Revision;
   }

   /***********************************************/
   /*   Otherwise,  just  show  the  version  or  */
   /*   branch                                    */
   /***********************************************/
   else
   {
      v_str += m_Type + " " + m_Name;
   }
   return(v_str);
}

/***************************************************************************//**
*  void Version::GetVersionNumber()
*
*  @return     QString with the Full Numeric Build Version
*              <Major>.<Minor>.<Patch>.<Build>
*
*******************************************************************************/
QString Version::GetVersionNumber()
{
   ParseVersionControlStrings();
   return("Vers: " + m_Major + "." + m_Minor + "." + m_Patch + "." + m_Build);
}


/***************************************************************************//**
*  void Version::GetShortVersionNumber()
*
*  @return     QString with the Shortened Numeric Build Version
*              <Major>.<Minor>.<Patch>
*
*******************************************************************************/
QString Version::GetShortVersionNumber()
{
   ParseVersionControlStrings();
   return("Vers: " + m_Major + "." + m_Minor + "." + m_Patch + ".");
}


/***************************************************************************//**
*  void Version::ParseVersionControlStrings()
*
*  Parses the version control keywords and creates the approriate sctrings to 
*  present at the versions
*
*  @return       none
*
*  @note    If REGEXP_TEST is defined, this function will iterate through
*           the various predefined versions of the svn url so the regexp 
*           definitions can be more readily tested.
******************************************************************************/
void Version::ParseVersionControlStrings()
{
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "Enter- m_Parsed = " << m_Parsed;
   if ( ! m_Parsed ) 
   {
      m_Name.clear();

      /***********************************************/
      /*   This  section parses out the Path string  */
      /*   from version control and determines what  */
      /*   sort  of code we were built from (tabgs,  */
      /*   branches, trunk) and version information  */
      /*   if available (tags)                       */
      /***********************************************/
      if ( ! m_CmPath.isEmpty() && ! m_PathREStr.isEmpty() ) 
      {
         QString rev_str = m_CmPath;

         qDebug(*log(LOG, 1)) << __FUNCTION__ << "m_PathREStr = " << m_PathREStr;
         qDebug(*log(LOG, 1)) << __FUNCTION__ << "rev_str     = " << rev_str;

         QRegExp re1(m_PathREStr);
         re1.setMinimal(true);

         if ( re1.indexIn(rev_str) >= 0 )
         {
            QString temp1 = re1.cap(1);
            qDebug(*log(LOG, 1)) << __FUNCTION__ << "MATCH!!!!  temp1: " << temp1 << ", rev_str: " << rev_str;

            /***********************************************/
            /*   Next, if it came from either branches or  */
            /*   tags,  get the name of the branch or the  */
            /*   tag.                                      */
            /***********************************************/
            QStringList rs = temp1.split("/");
            if ( rs.count() == 2 ) 
            {
               QString type = rs[0];
               m_Name = rs[1];
               qDebug(*log(LOG, 1)) << __FUNCTION__ << "MATCH!!!!  type: " << type << ", m_Name: " << m_Name;
               if ( type.toLower() == "branches") 
               {
                  m_Type = "Branch";
                  m_Verbose = true;
               }
               if ( type.toLower() == "tags") 
               {
                  m_Type = "Version";

                  // get rid of the trial string if it's in the tag
                  m_Name.remove(TRIAL_TAG_STR);

                  /***********************************************/
                  /*   If this came from a tag, lets try to get  */
                  /*   the   Major,   Minor,  Patch  and  Build  */
                  /*   numbers from it.                          */
                  /***********************************************/

                  rs = m_Name.split("_");
                  if ( rs.count() > VERSION_BUILD_OFFSET ) 
                  {
                     m_Build = rs[VERSION_BUILD_OFFSET];
                  }
                  if ( rs.count() > VERSION_PATCH_OFFSET ) 
                  {
                     m_Patch = rs[VERSION_PATCH_OFFSET];
                  }
                  if ( rs.count() > VERSION_MINOR_OFFSET ) 
                  {
                     m_Minor = rs[VERSION_MINOR_OFFSET];
                  }
                  if ( rs.count() > VERSION_MAJOR_OFFSET ) 
                  {
                     m_Major = rs[VERSION_MAJOR_OFFSET];
                  }
               }
            }
            /***********************************************/
            /*   Otherwise, lets hope it came from trunk   */
            /***********************************************/
            else if ( temp1.toLower() == "trunk" )
            {
               m_Type = "Development";
               m_Verbose = true;
            }
         }

         /***********************************************/
         /*   If we can't determine where it came fim,  */
         /*   life is not good!                         */
         /***********************************************/
         else 
         {
            qDebug(*log(LOG, 1)) << __FUNCTION__ << "NO match!!!!  rev_str: " << rev_str;
         }   
      }
      if ( ! m_Revision.isEmpty() && ! m_RevREStr.isEmpty() ) 
      {
         QRegExp re1(m_RevREStr);
         re1.setMinimal(true);

         if ( re1.indexIn(m_Revision) >= 0 )
         {
            m_Revision = re1.cap(1);
            qDebug(*log(LOG, 1)) << __FUNCTION__ << "MATCH!!!! m_Revision: " << m_Revision;
         }
         else 
         {
            qDebug(*log(LOG, 1)) << __FUNCTION__ << "NO MATCH!!!! m_RevREStr: " << m_RevREStr;
            qDebug(*log(LOG, 1)) << __FUNCTION__ << "             m_Revision: " << m_Revision;
         }
      }
      m_Parsed = true;
   }
}

