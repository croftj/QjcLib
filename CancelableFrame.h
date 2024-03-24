/*********************************************************************************
**
**   $Id$
**   Copyright (c) 2004 Joe Croft <joe@croftj.net>
**   
**   This file is part of hl4.
**
**   Hl4 is free software; you can redistribute it and/or modify
**   it under the terms of the GNU General Public License as published by
**   the Free Software Foundation; either version 2 of the License, or
**   (at your option) any later version.
**
**   Hl4 is distributed in the hope that it will be useful,
**   but WITHOUT ANY WARRANTY; without even the implied warranty of
**   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**   GNU General Public License for more details.
**
**   You should have received a copy of the GNU General Public License
**   along with Hl4; if not, write to the Free Software
**   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**
*********************************************************************************/
#ifndef QCJLIB_CANCELABLE_FRAME_H
#define QCJLIB_CANCELABLE_FRAME_H

#include <QFrame>

namespace QcjLib
{
   class CancelableFrame : public QFrame
   {
      Q_OBJECT

   public:
      CancelableFrame(QWidget *parent = nullptr) : QFrame(parent) {};
      virtual bool cancel() = 0;
      static CancelableFrame *findFrame(QWidget* parent);
   };
}

#endif
