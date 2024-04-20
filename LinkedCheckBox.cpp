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
#include "LinkedCheckBox.h"
#include <QDebug>

QcjLib::LinkedCheckBox::LinkedCheckBox(const QString &text, QcjLib::LinkedCheckBox *prev, QWidget *parent) :
   QCheckBox(text, parent),
   m_prev(prev),
   m_next(nullptr)
{
   qDebug() << "New check box: " << text;
   connect(this, SIGNAL(clicked(bool)), this, SLOT(haveClicked()), Qt::UniqueConnection);
   show();
}

void QcjLib::LinkedCheckBox::focusOutEvent(QFocusEvent *evt)
{
   qDebug() << "Have event: " << evt->type();
   if (true && evt->type() == QEvent::FocusOut)
   {
      if (false && m_next == nullptr)
      {
         qDebug() << "Asking to loose focus!";
         emit lastButton();
      }
   }
   qDebug() << __FUNCTION__ << "Have event: " << evt->type();
}

void QcjLib::LinkedCheckBox::haveClicked()
{
   qDebug() << QString("button %1 clicked").arg(text());
}
