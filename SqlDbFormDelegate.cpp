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
