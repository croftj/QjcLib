#ifndef YESNOCOMBOBOX_H
#define YESNOCOMBOBOX_H

# include <QComboBox>

namespace QcjLib
{
   class YesNoComboBox : public QComboBox
   {
      Q_OBJECT

   public:
      YesNoComboBox(QWidget *parent);
   };
}

# endif
