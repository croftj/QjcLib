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
/***************************************************************************//**
* @file	 Version.h
* 
* Class declaration for Version. This class parses the svn HeadURL string and
* creates appropriate version strings to be displayed to the user
*
* @note    None
*
* @warning None
*******************************************************************************/

#ifndef VERSION_H
#define VERSION_H

#include "LogBuilder.h"

#include <QString>

namespace QcjLib
{
   /***************************************************************************//**
   * Version class for parsing and storing applicatioon Version Information
   *
   *******************************************************************************/
   class Version
   {
   public:
      /***************************************************************************//**
      * Version constructor
      * 
      * @return       none
      * 
      * @note          none
      * 
      * @warning       none
      *******************************************************************************/ 
      Version()
      {
   #if defined (QT_DEBUG)
         m_Verbose = true;
   #endif
      }

      virtual ~Version(void) {}

      //***************************************************************************//**
      //  void Version::SetPathRegExpression()
      //
      // @param[in]  re, The Regular expression to parse the version string with
      // @return     n/a
      //
      // Ex: .*/Projects/wwb/wwb6/(.+)/wwb6/versioninfo\.xml\s+\$
      // The captured portion of the string is the trunk/branch/tag portion of the path
      //*******************************************************************************/ 
      static void SetPathRegExpression(QString re)
      {
         m_PathREStr = re;
      }

      //***************************************************************************//**
      //  void Version::SetRevisionRegExpression()
      //
      // @param[in]  re, The Regular expression to parse the revision string with
      // @return     n/a
      //
      // Ex: :\s+([0-9]+)\s+\$
      // The captured portion of the string is the revision number string
      //*******************************************************************************/ 
      static void SetRevisionRegExpression(QString re)
      {
         m_RevREStr = re;
      }

      static void SetCMPathString(QString path)
      {
         m_CmPath = path;
      }

      static void SetCMRevisionString(QString rev)
      {
         m_Revision = rev;
      }

      static QString GetVersionNumber();
      static QString GetShortVersionNumber();
      static QString GetFullVersionString(bool verbose);
      static void ParseVersionControlStrings();
      static void SetVerbose();

      /***************************************************************************//**
      *  void Version::Major()
      *
      *  @return     QString with the Major Number
      *
      *******************************************************************************/
      static QString Major()
      {
         ParseVersionControlStrings();
         return(m_Major);
      }

      /***************************************************************************//**
      *  void Version::Minor()
      *
      *  @return     QString with the Minor Number
      *
      *******************************************************************************/
      static QString Minor()
      {
         ParseVersionControlStrings();
         return(m_Minor);
      }

      /***************************************************************************//**
      *  void Version::Patch()
      *
      *  @return     QString with the Patch Number
      *
      *******************************************************************************/
      static QString Patch()
      {
         ParseVersionControlStrings();
         return(m_Patch);
      }

      /***************************************************************************//**
      *  void Version::Build()
      *
      *  @return     QString with the Build Number
      *
      *******************************************************************************/
      static QString Build()
      {
         ParseVersionControlStrings();
         return(m_Build);
      }

      /***************************************************************************//**
      *  void Version::ExtraInfo()
      *
      *  @return     QString with any extra information defined
      *
      *******************************************************************************/
      static QString ExtraInfo()
      {
         ParseVersionControlStrings();
         return(m_ExtraInfo);
      }

      /***************************************************************************//**
      *  void Version::Type()
      *
      *  @return     QString with the Build Type
      *
      *******************************************************************************/
      static QString Type()
      {
         ParseVersionControlStrings();
         return(m_Type);
      }

      /***************************************************************************//**
      *  void Version::Name()
      *
      *  @return     QString with the Build Name
      *
      *******************************************************************************/
      static QString Name()
      {
         ParseVersionControlStrings();
         return(m_Name);
      }

      /***************************************************************************//**
      *  void Version::CmPath()
      *
      *  @return     QString with the Full Path in Version Control
      *
      *******************************************************************************/
      static QString CmPath()
      {
         return(m_CmPath);
      }

      /***************************************************************************//**
      *  void Version::Revision()
      *
      *  @return     QString with the Build Revision
      *
      *******************************************************************************/
      static QString Revision()
      {
         return(m_Revision);
      }

   private:
      enum
      {
         VERSION_MAJOR_OFFSET = 1,
         VERSION_MINOR_OFFSET,
         VERSION_PATCH_OFFSET,
         VERSION_BUILD_OFFSET
      };

                     Version(const Version &);         // degenerated
      Version & operator =(const Version &);           // degenerated

      static QString DebugVersionString();
      static void    Initialize(QString version);

      static int           m_Iteration;   // For debug purposes
      static QString       m_Build;       // This holds the Build number of the version
      static QString       m_CmPath;      // is a string holding the Version Control keywards to be parsed
      static QString       m_ExtraInfo;   // This holds the name of the branch or tag of the codeline
      static QString       m_Major;       // This holds the Major number of the version
      static QString       m_Minor;       // This holds the Minor number of the version
      static QString       m_Name;        // This holds name of the branch or version
      static bool          m_Parsed;      // If false, the parser needs to be run
      static QString       m_Patch;       // This holds the Patch number of the version
      static QString       m_PathREStr;   // This holds the regular expression string to parse the path with
      static QString       m_Revision;    // This holds the revision number of the code
      static QString       m_RevREStr;    // This holds the regular expression string to parse the revision with
      static QString       m_Type;        // This holds the type of the build Development, Branch or Tag (aka Version)
      static bool          m_Verbose;     // If true, show a verbose form of the version info

      static const QString TRIAL_TAG_STR; // The trial string that is in the tag at build time
   };
}

#endif // VERSION_H
