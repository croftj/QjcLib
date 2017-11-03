#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include "LogBuilder.h"

# include <QDebug>
# include <QHeaderView>
# include <QSqlTableModel>
# include <QTableView>
# include <QModelIndex>
# include <QWidget>

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

   public:
      TableView(QWidget * parent = 0) :
         m_debug(false),
         QTableView(parent)
      {
         connect(horizontalHeader(), SIGNAL(sectionClicked(int)), 
                 this, SLOT(SlotSectionClicked(int)));
         setSortingEnabled(true);
         m_sortColumn = -1;
      }

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

   protected slots:
      void SlotSectionClicked(int logicalSection)
      {
         qDebug(*log(LOG, 1)) << __FUNCTION__ << "have click on logical section " << logicalSection;
         SetSortColumn(logicalSection);
      }

   private:
      bool           m_debug;
      int            m_sortColumn;
      Qt::SortOrder  m_sortOrder;
   };
}

# endif 
