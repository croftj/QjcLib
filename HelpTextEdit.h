#ifndef HELPTEXTEDIT_H
#define HELPTEXTEDIT_H

#include "LogBuilder.h"

#include <QDebug>
#include <QDomDocument>
#include <QDomNode>
#include <QFile>
#include <QMap>
#include <QMessageBox>
#include <QString>
#include <QTextEdit>

namespace QcjLib
{
   class HelpTextEdit : public QTextEdit
   {
      Q_OBJECT
   public:
      typedef QMap<QString, QDomDocument *> HelpMap_t;

      HelpTextEdit(QWidget *parent) : 
         QTextEdit(parent)
      {
         setReadOnly(true);
      }

      void initialize()
      {
         qDebug(*QcjLib::log(LOG, 1)) << __FUNCTION__ << "have properties: " << dynamicPropertyNames();
         const QDomNode& help_node = QDomNode();
         QVariant help_property = property("help_resource");
         QString help_block = help_property.toString();
         QStringList sl = help_property.toString().split("::");
         QString resource_name = sl[0];
         if ( sl.count() > 1 ) 
         {
            help_block = sl[1];
         }

         qDebug(*QcjLib::log(LOG, 1)) << __FUNCTION__ << "help_property = " << help_property;
         qDebug(*QcjLib::log(LOG, 1)) << __FUNCTION__ << "resource_name = " << resource_name;
         qDebug(*QcjLib::log(LOG, 1)) << __FUNCTION__ << "help_block = " << help_block;

         if ( ! m_helpMap.contains(resource_name) ) 
         {
            QDomDocument *doc = new QDomDocument();
            QFile configFile(resource_name);
            if ( configFile.exists() ) 
            {
               if ( configFile.open(QIODevice::ReadOnly) ) 
               {
                  QString errString;
                  int errLine;
                  int errColumn;

                  if ( ! doc->setContent(&configFile, &errString, &errLine, &errColumn) ) 
                  {
                     QMessageBox::critical(0, "Error Parsing configuration file", "Could not parse the file: " + 
                                             resource_name + ". Error: " + errString +
                                             " on line " + QString::number(errLine) +
                                             ", column " + QString::number(errColumn)
                                             );
                     exit(0);
                  }
                  qDebug(*QcjLib::log(LOG, 1)) << __FUNCTION__ << __FUNCTION__ << "help file parsed!";
               }
               else 
               {
                  QMessageBox::critical(0, "Error opening help file", "Could not open the file: " + 
                                          resource_name + ". Error: " + configFile.errorString());
                  exit(0);
               }
            }
            else 
            {
               qDebug(*QcjLib::log(LOG, 1)) << __FUNCTION__ << "No Config file found!! name: " << resource_name;
            }
            m_helpMap.insert(resource_name, doc);
         }

         QDomElement docElem = m_helpMap[resource_name]->documentElement();
         QDomNode n1 = docElem.firstChild();

         while ( !n1.isNull() ) 
         {
            QDomElement e1 = n1.toElement();
            qDebug(*QcjLib::log(LOG, 1)) << __FUNCTION__ << "Found tagName: " << e1.tagName();
            
            if ( ! e1.isNull() && e1.tagName() == "help" )
            {
               if ( e1.hasAttribute("name") )
               {
                  qDebug(*QcjLib::log(LOG, 1)) << __FUNCTION__ << "Found name: " << e1.attribute("name");
                  QString name = e1.attribute("name");
                  if ( name == help_block ) 
                  {
                     qDebug(*QcjLib::log(LOG, 1)) << __FUNCTION__ << "Found help block!";
                     qDebug(*QcjLib::log(LOG, 1)) << __FUNCTION__ << "node type = " << n1.nodeType();

                     QDomNode textNode = n1.firstChild();
                     if ( textNode.isText() ) 
                     {
                        qDebug(*QcjLib::log(LOG, 1)) << __FUNCTION__ << "Found text node: " << n1.nodeValue();
                        setText(textNode.nodeValue());
                     }
                  }
               }
            }
            n1 = n1.nextSibling();
         }
         if ( help_node.isText() ) 
         {
            setText(help_node.nodeValue());
         }
      }

      static const QString LOG;

   private:
      static HelpMap_t  m_helpMap;
   };
}

#endif

