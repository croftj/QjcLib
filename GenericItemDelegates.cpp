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
# include "GenericItemDelegates.h"

# include <QComboBox>
# include <QDebug>

using namespace QcjLib;

const QString GenericIntDelegate::LOG("QcjLib_generic_delegate");
static LogBuilder mylog(GenericIntDelegate::LOG, 1, "QcjLib Geniric Item Delegate");

GenericIntDelegate::GenericIntDelegate(QcjDataFields &fieldData, QObject *parent) : QStyledItemDelegate(parent)
{
   m_fieldData = fieldData;
}

QWidget *GenericIntDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
   QSpinBox *templ = dynamic_cast<QSpinBox*>(m_fieldData.widget);
   QSpinBox *editor = new QSpinBox(parent);
   qDebug(*log(LOG, 1)) << "editor =" << (unsigned long)editor;
   editor->setMinimum(templ->minimum());
   editor->setMaximum(templ->maximum());
   editor->setSingleStep(templ->singleStep());
   editor->setPrefix(templ->prefix());
   editor->setSuffix(templ->suffix());
   connect(editor, SIGNAL(editingFinished()), this, SLOT(commitCloseEditor()));
   return(editor);
}

void GenericIntDelegate::closeCommitEditor()
{
   QSpinBox *editor = qobject_cast<QSpinBox*>(sender());
   emit commitData(editor);
   emit closeEditor(editor);
}

void GenericIntDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
   dynamic_cast<QSpinBox*>(editor)->setValue(index.model()->data(index, Qt::EditRole).toInt());
}

void GenericIntDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
   QVariant value(dynamic_cast<QSpinBox*>(editor)->value());
   model->setData(index, value, Qt::EditRole);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

const QString GenericDoubleDelegate::LOG("QcjLib_generic_delegate");

GenericDoubleDelegate::GenericDoubleDelegate(QcjDataFields &fieldData, QObject *parent) : QStyledItemDelegate(parent)
{
   m_fieldData = fieldData;
}

QWidget *GenericDoubleDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
   QDoubleSpinBox *templ = dynamic_cast<QDoubleSpinBox*>(m_fieldData.widget);
   QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
   qDebug(*log(LOG, 1)) << "editor =" << (unsigned long)editor;
   editor->setDecimals(templ->decimals());
   editor->setMinimum(templ->minimum());
   editor->setMaximum(templ->maximum());
   editor->setPrefix(templ->prefix());
   editor->setSingleStep(templ->singleStep());
   editor->setSuffix(templ->suffix());
   connect(editor, SIGNAL(editingFinished()), this, SLOT(closeCommitEditor()));
   return(editor);
}

void GenericDoubleDelegate::closeCommitEditor()
{
   QDoubleSpinBox *editor = qobject_cast<QDoubleSpinBox*>(sender());
   emit commitData(editor);
   emit closeEditor(editor);
}

void GenericDoubleDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
   dynamic_cast<QDoubleSpinBox*>(editor)->setValue(index.model()->data(index, Qt::EditRole).toDouble());
}

void GenericDoubleDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
   QVariant value(dynamic_cast<QDoubleSpinBox*>(editor)->value());
   model->setData(index, value, Qt::EditRole);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

GenericMoneyDelegate::GenericMoneyDelegate(QcjDataFields &fieldData, QObject *parent) : QStyledItemDelegate(parent)
{
   m_fieldData = fieldData;
}

QWidget *GenericMoneyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
   QcjMoneyEdit *editor = new QcjMoneyEdit(parent);
   connect(editor, SIGNAL(textEdited(QString&)), this, SLOT(commitCloseEditor()));
   return(editor);
}

void GenericMoneyDelegate::closeCommitEditor()
{
   QcjMoneyEdit *editor = qobject_cast<QcjMoneyEdit*>(sender());
   emit commitData(editor);
   emit closeEditor(editor);
}

void GenericMoneyDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
   static_cast<QcjMoneyEdit*>(editor)->setText(index.model()->data(index, Qt::EditRole).toString());
}

void GenericMoneyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
   QcjMoneyEdit *me = static_cast<QcjMoneyEdit*>(editor);
   QVariant value(me->formatCurrency(me->text()));
   model->setData(index, value, Qt::EditRole);
   model->setData(index,  QVariant(Qt::AlignRight), Qt::TextAlignmentRole);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

GenericStringSelectDelegate::GenericStringSelectDelegate(QcjDataFields &fieldData, QObject *parent) : QStyledItemDelegate(parent)
{
   m_fieldData = fieldData;
}

QWidget *GenericStringSelectDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
   QcjStringSelect *templ = dynamic_cast<QcjStringSelect*>(m_fieldData.widget);
   QcjStringSelect *editor = new QcjStringSelect(parent);
   int itmCount = templ->count();
   for (int idx = 0; idx < itmCount; idx++)
   {
      editor->addItem(templ->itemText(idx), templ->itemData(idx));
   }
   editor->setEditable(templ->isEditable());
   connect(editor, SIGNAL(currentIndexChanged(int)), this, SLOT(commitCloseEditor()));
   return(editor);
}

void GenericStringSelectDelegate::closeCommitEditor()
{
   QcjStringSelect *editor = qobject_cast<QcjStringSelect*>(sender());
   emit commitData(editor);
   emit closeEditor(editor);
}

void GenericStringSelectDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
   dynamic_cast<QcjStringSelect*>(editor)->setText(index.model()->data(index, Qt::EditRole).toString());
}

void GenericStringSelectDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
   QVariant value(dynamic_cast<QcjStringSelect*>(editor)->text());
   model->setData(index, value, Qt::EditRole);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

GenericYesNoDelegate::GenericYesNoDelegate(QcjDataFields &fieldData, QObject *parent) : QStyledItemDelegate(parent)
{
   m_fieldData = fieldData;
}

QWidget *GenericYesNoDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
   qDebug() << "Creating edit widget: value = " << index.model()->data(index, Qt::EditRole).toString();
   QcjYesNoSelect *templ = dynamic_cast<QcjYesNoSelect*>(m_fieldData.widget);
   QcjYesNoSelect *editor = new QcjYesNoSelect(parent);
   editor->setText(index.model()->data(index, Qt::EditRole).toString());
   int itmCount = templ->count();
   editor->setEditable(templ->isEditable());
   connect(editor, SIGNAL(currentIndexChanged(int)), this, SLOT(commitCloseEditor()));
   return(editor);
}

void GenericYesNoDelegate::closeCommitEditor()
{
   QcjYesNoSelect *editor = qobject_cast<QcjYesNoSelect*>(sender());
   emit commitData(editor);
   emit closeEditor(editor);
}

void GenericYesNoDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
   dynamic_cast<QcjYesNoSelect*>(editor)->setText(index.model()->data(index, Qt::EditRole).toString());
}

void GenericYesNoDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
   QVariant value(dynamic_cast<QcjYesNoSelect*>(editor)->text());
   model->setData(index, value, Qt::EditRole);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

GenericPhoneDelegate::GenericPhoneDelegate(QcjDataFields &fieldData, QObject *parent) : QStyledItemDelegate(parent)
{
   m_fieldData = fieldData;
}

QWidget *GenericPhoneDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
   QcjPhoneEdit *templ = dynamic_cast<QcjPhoneEdit*>(m_fieldData.widget);
   QcjPhoneEdit *editor = new QcjPhoneEdit(parent);
   connect(editor, SIGNAL(currentIndexChanged(int)), this, SLOT(commitCloseEditor()));
   return(editor);
}

void GenericPhoneDelegate::closeCommitEditor()
{
   QcjPhoneEdit *editor = qobject_cast<QcjPhoneEdit*>(sender());
   emit commitData(editor);
   emit closeEditor(editor);
}

void GenericPhoneDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
   dynamic_cast<QcjPhoneEdit*>(editor)->setText(index.model()->data(index, Qt::EditRole).toString());
}

void GenericPhoneDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
   QVariant value(dynamic_cast<QcjPhoneEdit*>(editor)->text());
   model->setData(index, value, Qt::EditRole);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/


