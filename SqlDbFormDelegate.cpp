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
# include "SqlDbFormDelegate.h"

# include <QComboBox>
# include <QDebug>

using namespace QcjLib;

const QString SqlDbFormDelegate::LOG("QcjLib_db_form_delegate");
static LogBuilder mylog(SqlDbFormDelegate::LOG, 1, "QcjLib Database Form Delegate");

SqlDbFormDelegate::SqlDbFormDelegate(QObject *parent) : QItemDelegate(parent)
{
}

void SqlDbFormDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
   QComboBox *comboBox = dynamic_cast<QComboBox*>(editor);
   if ( comboBox != NULL && ! comboBox->property("sql_field_name").toString().isEmpty() ) 
   {
      QString name = comboBox->property("sql_field_name").toString();
      int value = index.model()->data(index, Qt::EditRole).toInt();
      int offset = comboBox->findData(value);
      qDebug(*log(LOG, 1)) << __FUNCTION__ << "field name: " << name << ", value = " << value << ", offset = " << offset;
      comboBox->setCurrentIndex(offset);
   }
   else 
   {
      QItemDelegate::setEditorData(editor, index);
   }
}

void SqlDbFormDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
   QComboBox *comboBox = dynamic_cast<QComboBox*>(editor);
   if ( comboBox != NULL && ! comboBox->property("sql_field_name").toString().isEmpty() ) 
   {
      QString name = comboBox->property("sql_field_name").toString();
      int offset = comboBox->currentIndex();
      QVariant value = comboBox->itemData(offset);
      qDebug(*log(LOG, 1)) << __FUNCTION__ << "field name: " << name << "index = " << index << ", value = " << value << ", value(int) " << value.toInt() << ", offset = " << offset;
      model->setData(index, value, Qt::EditRole);
   }
   else 
   {
      QItemDelegate::setModelData(editor, model, index);
   }
   
}
