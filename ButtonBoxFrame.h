#ifndef BUTTON_BOX_FRAME
#define BUTTON_BOX_FRAME

#include <QFrame>
#include <QString>
#include <QStringList>

namespace QcjLib
{
   class ButtonBoxFrame : public QFrame
   {
      Q_OBJECT               
      Q_PROPERTY (QString xml_definition READ readXmlDef WRITE writeXmlDef)
      
   public:
      ButtonBoxFrame(QWidget *parent = nullptr);
      ~ButtonBoxFrame() {};

      void setDatabase(const QString &item);
      QStringList checkedButtons();
      QStringList buttonList();

      void writeXmlDef(QString xmldef)
      {
         m_xmldef = xmldef;
      }

      QString readXmlDef()
      {
         return(m_xmldef);
      }

   protected:
      QString  m_xmldef;
   };
}
#endif
