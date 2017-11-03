#ifndef LISTVIEW_H
#define LISTVIEW_H

# include <QDebug>
# include <QListView>
# include <QModelIndex>
# include <QItemSelectionModel>
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
   class ListView : public QListView 
   {
      Q_OBJECT

   public:
      ListView(QWidget * parent = 0) :
         QListView(parent)
      {
      };

   signals:
      void 	activated(const QModelIndex &index);

   public slots:
      void setSelectedIndex(const QModelIndex &index)
      {
         QItemSelectionModel *sm = new QItemSelectionModel(model());
         sm->setCurrentIndex(index, QItemSelectionModel::Select);
         selectionModel()->deleteLater();
         setSelectionModel(sm);
      }

   protected:
      void currentChanged(const QModelIndex &current, const QModelIndex &previous)
      {
         QListView::currentChanged(current, previous);
   //      if ( current.isValid() ) 
   //      {
            emit activated(current);
   //      }
      };

   private:
   };
}
#endif
