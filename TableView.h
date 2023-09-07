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
#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include "LogBuilder.h"

#include <QDebug>
#include <QHeaderView>
#include <QKeyEvent>
#include <QSqlTableModel>
#include <QTableView>
#include <QModelIndex>
#include <QWidget>

namespace QcjLib
{
   /**********************************************************************/
   /*   This  Object is a reimplimentatin of the standard QListView. It  */
   /*   changes the original behavior to emit the activated signal when  */
   /*   ever  a new row is highlighted such as through a mouse click OR  */
   /*   using  the  arrow  keys. The original behavior only emitted the  */
   /*   activated  signal on a mouse click or pressing enter once after  */
   /*   a row was highlighted using the arrow keys.                      */
   /**********************************************************************/
   class TableView : public QTableView 
   {
      Q_OBJECT
      Q_PROPERTY (QString xml_definition READ readXmlDef WRITE writeXmlDef)

   public:
      TableView(QWidget * parent = 0) :
         QTableView(parent),
         m_debug(false),
         m_haveDelegates(false)
      {
         setTabKeyNavigation(false);
         connect(horizontalHeader(), SIGNAL(sectionClicked(int)), 
                 this, SLOT(SlotSectionClicked(int)));
         setSortingEnabled(true);
         m_sortColumn = -1;
      }
      
      void writeXmlDef(QString s) { m_xmldef = s; };
      QString readXmlDef() const { return(m_xmldef); };
      void setFields(int row = 0);

      void SetDebug(bool debug)
      {
         m_debug = debug;
      }

   //signals:
   //   void 	activated(const QModelIndex &index);
      void SetSortColumn(int column)
      {
         QSqlTableModel *tbl_model = dynamic_cast<QSqlTableModel*>(model());
         if ( tbl_model != 0 ) 
         {
            if ( column == m_sortColumn ) 
            {
               m_sortOrder = (m_sortOrder == Qt::AscendingOrder ) ? Qt::DescendingOrder : Qt::AscendingOrder;
            }
            else 
            {
               m_sortOrder = Qt::AscendingOrder;
            }
            m_sortColumn = column; 
            tbl_model->sort(m_sortColumn, m_sortOrder);
            qDebug(*log(LOG, 1)) << __FUNCTION__ << "current index row: " << currentIndex().row() << ", col: " << currentIndex().column();
         }
      }

      static const QString LOG;

   protected:
      void currentChanged(const QModelIndex &current, const QModelIndex &previous)
      {
         qDebug(*log(LOG, 1)) << "TableView::currentChanged(): Enter...";
         QTableView::currentChanged(current, previous);
         emit clicked(current);
         qDebug(*log(LOG, 1)) << "TableView::currentChanged(): Exit";
      };

      virtual bool focusInEvent(QEvent *evt);
      virtual bool event(QEvent *evt);
      virtual void keyPressEvent(QKeyEvent *evt);

   protected slots:
      void SlotSectionClicked(int logicalSection)
      {
         qDebug(*log(LOG, 1)) << __FUNCTION__ << "have click on logical section " << logicalSection;
         SetSortColumn(logicalSection);
      }

   private:
      bool           m_debug;
      bool           m_haveDelegates;
      int            m_sortColumn;
      Qt::SortOrder  m_sortOrder;
      QString        m_xmldef;
   };
}

# endif 
