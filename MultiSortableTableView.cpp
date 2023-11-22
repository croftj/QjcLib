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
