#ifndef DBINTERFACE_H
#define DBINTERFACE_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QString>
#include <QVariant>

namespace QcjLib
{
   class DbInterface
   {
   public:
      class SqlError 
      {
      public:
         SqlError(QString err = QString(), QString statement = QString(), QSqlError dbe = QSqlError()) : 
            Message(err),
            Error(dbe),
            Statement(statement)
         {}

         QString     Message;
         QSqlError   Error;
         QString     Statement;
      };

      virtual QVariant GetLastInsertId(const QString table_name) = 0;
      virtual QString GetTableName(const QString name) = 0;
      virtual QString GetIndexName(const QString name) = 0;
      virtual QSqlDatabase database() = 0;
      virtual QSqlQuery NewQuery() = 0;
   };
};

#endif
