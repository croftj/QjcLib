# include "YesNoComboBox.h"

# include <QVariant>

using namespace QcjLib;

YesNoComboBox::YesNoComboBox(QWidget *parent) : QComboBox(parent)
{
   addItem("Yes", QVariant(1));
   addItem("No", QVariant(0));
}

