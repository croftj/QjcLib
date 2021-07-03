#ifndef GENERICTABLELMODEL_H
#define GENERICTABLELMODEL_H

# include "LogBuilder.h"

#include <QMutex>
# include <QStandardItemModel>

namespace QcjLib
{
   class GenericTableModel : public QStandardItemModel 
   {
      Q_OBJECT

   public:
      GenericTableModel(QObject *parent = NULL) : QStandardItemModel(parent) {};
      
      typedef QMap<QString, QString> ModelRow_t;

      QStringList Headers();

      int         FindColumn(QString col_name);
      int         FindRow(QString col_name, QString value);
      int         FindRow(int col, QString value);
      int         AddColumn(QString col_name);
      int         AddColumn(int row, QString col_name, QString text);
      bool        RemoveColumn(QString col_name);
      void        SetValue(int row, QString col_name, QString text);
      void        SetValue(int row, int col, QString text);
      ModelRow_t  GetRow(int row);
      QString     Value(int row, QString col_name);
      QString     Value(int row, int col);

      static const QString LOG;

   protected:
   private:
      mutable QMutex m_lock;
   };
};

#endif
