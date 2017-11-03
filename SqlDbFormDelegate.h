#ifndef SQLRELATIONDELEGATE_H
#define SQLRELATIONDELEGATE_H

# include "LogBuilder.h"

# include <QItemDelegate>

namespace QcjLib
{
   class SqlDbFormDelegate : public QItemDelegate
   {
      Q_OBJECT

   public:
      SqlDbFormDelegate(QObject *parent = 0);

      virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
      virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                        const QModelIndex &index) const;

      static const QString LOG;
   };
}

#endif
