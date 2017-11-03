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
