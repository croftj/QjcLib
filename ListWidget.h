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
# include <QListWidget>
# include <QModelIndex>
# include <QWidget>

namespace QcjLib
{
   /**********************************************************************/
   /*   This Object is a reimplimentatin of the standard QListWidget. It */
   /*   changes the original behavior to emit the activated signal when  */
   /*   ever  a new row is highlighted such as through a mouse click OR  */
   /*   using  the  arrow  keys. The original behavior only emitted the  */
   /*   activated  signal on a mouse click or pressing enter once after  */
   /*   a row was highlighted using the arrow keys.                      */
   /**********************************************************************/
   class ListWidget : public QListWidget 
   {
      Q_OBJECT

   public:
      ListWidget(QWidget * parent = 0) :
         QListWidget(parent)
      {
      };

//   signals:
//      void 	activated(const QModelIndex &index);

   protected:
      void currentChanged(const QModelIndex &current, const QModelIndex &previous)
      {
         QListWidget::currentChanged(current, previous);
   //      if ( current.isValid() ) 
   //      {
            emit activated(current);
   //      }
      };

   private:
   };
}
