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

      virtual QVariant GetLastInsertId(QString table_name) = 0;
      virtual QString GetTableName(QString name) = 0;
      virtual QString GetIdName(QString name) = 0;
      virtual QString AdjustSqlNames(QString sql) = 0;
      virtual QSqlDatabase Database() = 0;
      virtual bool IsAdvancedUser() = 0;
   };
};

#endif
