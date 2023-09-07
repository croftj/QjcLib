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
#ifndef GENERICTABLELMODEL_H
#define GENERICTABLELMODEL_H

# include "LogBuilder.h"
# include "Types.h"

#include <QMutex>
# include <QStandardItemModel>

namespace QcjLib
{
   class GenericTableModel : public QStandardItemModel 
   {
      Q_OBJECT

   public:
      GenericTableModel(QObject *parent = NULL);

      GenericTableModel(const QcjLib::GenericTableModel& other) : 
         QStandardItemModel(nullptr)
      {
         qDebug() << "Enter...";
         for (int row = 0; row < other.rowCount(); row++)
         {
            for (int col = 0; col < other.columnCount(); col++)
            {
               const QString val(other.Value(row, col));
               SetValue(row, col, val);
//               QVariant header_val;
//               header_val = other.headerData(col, Qt::Horizontal);
//               setHeaderData(col, Qt::Horizontal, header_val);
            }
         }
      };
      
      typedef QHash<QString, QString> ModelRow_t;

      QStringList Headers() const;

      int         FindColumn(QString col_name) const;
      int         FindRow(QString col_name, QString value) const;
      int         FindRow(int col, QString value) const;
      int         AddColumn(QString col_name, QString data_name = QString());
      int         AddColumn(int row, QString col_name, QString text, QString data_name = QString());
      bool        RemoveColumn(QString col_name);
      void        SetValue(int row, QString col_name, QString text);
      void        SetValue(int row, int col, QString text);
      ModelRow_t  GetRow(int row) const;
      VariantHash GetVariantRow(int row) const;
      QString     Value(int row, QString col_name) const;
      QString     Value(int row, int col) const;
      QString     ColumnName(int col) const;
      QString     ColumnDataName(int col) const;
      QString     ColumnDataName(QString &name) const;
      int         appendBlankRow();
      void        appendRow(const QcjLib::VariantHash &data);
      static const QString LOG;

   protected:
   private:
      mutable QRecursiveMutex m_lock;
   };
};

#endif
