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
#ifndef GENERIC_ITEM_DELEGATE_H
#define GENERIC_ITEM_DELEGATE_H

#include "LogBuilder.h"

# include <QDebug>
# include <QModelIndex>
# include <QStyledItemDelegate>
# include <QWidget>

#include "../QcjData/QcjDataStatics.h"
#include "../QcjData/QcjDataHelpers.h"

/****************************************************************************/
/* This is a collection of StyledItemDelegates. They align with the various */
/* form element types defined in thr QcjData forms and table definitions.   */
/****************************************************************************/
namespace QcjLib
{
   class GenericIntDelegate : public QStyledItemDelegate
   {
      Q_OBJECT

   public:
      GenericIntDelegate(QcjDataFields &fieldData, QObject *parent = nullptr);

      virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
      virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
      virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                        const QModelIndex &index) const;

      static const QString LOG;

   private slots:
      void closeCommitEditor();

   private:
      QcjDataFields  m_fieldData;
      QSpinBox*      m_spinBox;
   };

   class GenericDoubleDelegate : public QStyledItemDelegate
   {
      Q_OBJECT

   public:
      GenericDoubleDelegate(QcjDataFields &fieldData, QObject *parent = nullptr);

      virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
      virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
      virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                        const QModelIndex &index) const;
      void setMinMaxIncrement(double minimum, double maximum, double increment);

      static const QString LOG;

   private slots:
      void closeCommitEditor();

   private:
      QcjDataFields  m_fieldData;
   };

#if 0
   class GenericImageDelegate : public QStyledItemDelegate
   {
      Q_OBJECT

   public:
      GenericImageDelegate(QcjDataFields &fieldData, QObject *parent = nullptr);

      virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
      virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
      virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                        const QModelIndex &index) const;
      void setMinMaxIncrement(double minimum, double maximum, double increment);
      static const QString LOG;

   private slots:
      void closeCommitEditor();

   private:
      QcjDataFields  m_fieldData;
   };
#endif

   class GenericMoneyDelegate : public QStyledItemDelegate
   {
      Q_OBJECT

   public:
      GenericMoneyDelegate(QcjDataFields &fieldData, QObject *parent = nullptr);

      virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
      virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
      virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                        const QModelIndex &index) const;
      void setMinMaxIncrement(double minimum, double maximum, double increment);
      static const QString LOG;

   private slots:
      void closeCommitEditor();

   private:
      QcjDataFields  m_fieldData;
   };

   class GenericStringSelectDelegate : public QStyledItemDelegate
   {
      Q_OBJECT

   public:
      GenericStringSelectDelegate(QcjDataFields &fieldData, QObject *parent = nullptr);

      virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
      virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
      virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                        const QModelIndex &index) const;

      static const QString LOG;

   private slots:
      void closeCommitEditor();

   private:
      QcjDataFields  m_fieldData;
   };

   class GenericYesNoDelegate : public QStyledItemDelegate
   {
      Q_OBJECT

   public:
      GenericYesNoDelegate(QcjDataFields &fieldData, QObject *parent = nullptr);

      virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
      virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
      virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                        const QModelIndex &index) const;

      static const QString LOG;

   private slots:
      void closeCommitEditor();

   private:
      QcjDataFields  m_fieldData;
   };

   class GenericPhoneDelegate : public QStyledItemDelegate
   {
      Q_OBJECT

   public:
      GenericPhoneDelegate(QcjDataFields &fieldData, QObject *parent = nullptr);

      virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
      virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
      virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                        const QModelIndex &index) const;

      static const QString LOG;

   private slots:
      void closeCommitEditor();

   private:
      QcjDataFields  m_fieldData;
   };
#if 0
   class GenericIntDelegate : public QStyledItemDelegate
   {
      Q_OBJECT

   public:
      GenericIntDelegate(QcjDataFields &fieldData, QObject *parent = nullptr);

      virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
      virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
      virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                        const QModelIndex &index) const;

      static const QString LOG;

   private:
      QcjDataFields  m_fieldData;
   };

   class GenericIntDelegate : public QStyledItemDelegate
   {
      Q_OBJECT

   public:
      GenericIntDelegate(QcjDataFields &fieldData, QObject *parent = nullptr);

      virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
      virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
      virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                        const QModelIndex &index) const;

      static const QString LOG;

   private:
      QcjDataFields  m_fieldData;
   };

   class GenericIntDelegate : public QStyledItemDelegate
   {
      Q_OBJECT

   public:
      GenericIntDelegate(QcjDataFields &fieldData, QObject *parent = nullptr);

      virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
      virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
      virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                        const QModelIndex &index) const;

      static const QString LOG;

   private:
      QcjDataFields  m_fieldData;
   };

   class GenericIntDelegate : public QStyledItemDelegate
   {
      Q_OBJECT

   public:
      GenericIntDelegate(QcjDataFields &fieldData, QObject *parent = nullptr);

      virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
      virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
      virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                        const QModelIndex &index) const;

      static const QString LOG;

   private:
      QcjDataFields  m_fieldData;
   };

   class GenericIntDelegate : public QStyledItemDelegate
   {
      Q_OBJECT

   public:
      GenericIntDelegate(QcjDataFields &fieldData, QcjDataFields &fieldData, QObject *parent = nullptr);

      virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
      virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
      virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                        const QModelIndex &index) const;

      static const QString LOG;

   private:
      QcjDataFields  m_fieldData;
   };
#endif
}

#endif
