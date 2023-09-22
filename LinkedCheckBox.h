#ifndef LINKED_CHECK_BOX
#define LINKED_CHECK_BOX

#include <QCheckBox>
#include <QFocusEvent>
#include <QString>
#include <QWidget>

namespace QcjLib
{
   class LinkedCheckBox : public QCheckBox
   {
      Q_OBJECT

   public:
      LinkedCheckBox(const QString &text, LinkedCheckBox *previous, QWidget *parent);

   signals:
      void lastButton();

   protected:
      virtual void focusOutEvent(QFocusEvent *evt);

   private:
      LinkedCheckBox *m_prev;
      LinkedCheckBox *m_next;
   };
}

#endif
