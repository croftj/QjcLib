#include "LinkedCheckBox.h"
#include <QDebug>

QcjLib::LinkedCheckBox::LinkedCheckBox(const QString &text, QcjLib::LinkedCheckBox *prev, QWidget *parent) :
   QCheckBox(text, parent),
   m_prev(prev),
   m_next(nullptr)
{
   qDebug() << "New check box: " << text;
   show();
}

void QcjLib::LinkedCheckBox::focusOutEvent(QFocusEvent *evt)
{
   qDebug() << "Have event: " << evt->type();
   if (true && evt->type() == QEvent::FocusOut)
   {
      if (false && m_next == nullptr)
      {
         qDebug() << "Asking to loose focus!";
         emit lastButton();
      }
   }
   qDebug() << __FUNCTION__ << "Have event: " << evt->type();
}

