#include "MultiSortableTableView.h"

#include "SqlSortableTableModel.h"

#include <QDebug>
#include <QHeaderView>
#include <QSqlTableModel>
#include <QTableView>

using namespace QcjLib;

const QString MultiSortableTableView::LOG("QcjLib_multisortview");
static LogBuilder mylog(MultiSortableTableView::LOG, 1, "QcjLib Multisortable Table View");

MultiSortableTableView::MultiSortableTableView(QWidget *parent) :
   QTableView(parent)
{
   connect((const QObject*)(horizontalHeader()), SIGNAL(sectionClicked(int)), this, SLOT(SortBy(int)));
   setSortingEnabled(true);
   m_sortColumn = -1;
}

void MultiSortableTableView::SortBy(int section)
{
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "section = " << section;
   SqlSortableTableModel *sort_model = dynamic_cast<SqlSortableTableModel*>(model());
   if ( sort_model != 0 ) 
   {
      qDebug(*log(LOG, 1)) << __FUNCTION__ << "Setting sort order for section";
      m_sortOrder = sort_model->SetOrder(section);
      m_sortColumn = section;
      sort_model->select();
   }
   else 
   {
      QSqlTableModel *tbl_model = dynamic_cast<QSqlTableModel*>(model());
      if ( tbl_model != NULL ) 
      {
         qDebug(*log(LOG, 1)) << __FUNCTION__ << "Have table model";
         if ( section == m_sortColumn ) 
         {
            m_sortOrder = (m_sortOrder == Qt::AscendingOrder ) ? Qt::DescendingOrder : Qt::AscendingOrder;
            qDebug(*log(LOG, 1)) << __FUNCTION__ << "Flipped order for section";

         }
         else 
         {
            m_sortOrder = Qt::AscendingOrder;
            qDebug(*log(LOG, 1)) << __FUNCTION__ << "Using Ascending order for section";
         }

         m_sortColumn = section;
         qDebug(*log(LOG, 1)) << __FUNCTION__ << "Sorting model";
         tbl_model->sort(m_sortColumn, m_sortOrder);
      }
   }
   qDebug(*log(LOG, 1)) << __FUNCTION__ << "Setting sort Indicator";
   horizontalHeader()->setSortIndicator(m_sortColumn, m_sortOrder);
}
