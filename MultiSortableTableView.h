#ifndef MULTISORTABLETABLEVIEW
#define MULTISORTABLETABLEVIEW

#include "LogBuilder.h"

#include <QDebug>
#include <QModelIndex>
#include <QTableView>
#include <QWidget>

namespace QcjLib
{
   class MultiSortableTableView : public QTableView
   {
      Q_OBJECT

   public:
      MultiSortableTableView(QWidget *parent = NULL);

      static const QString LOG;

   protected:
      void currentChanged(const QModelIndex &current, const QModelIndex &previous)
      {
         qDebug(*log(LOG, 1)) << __FUNCTION__ << "Current index changed: " << current;
         QTableView::currentChanged(current, previous);
         emit activated(current);
      };

   protected slots:
      void SortBy(int column);

   private:
      int            m_sortColumn;
      Qt::SortOrder  m_sortOrder;
   };
}

#endif
