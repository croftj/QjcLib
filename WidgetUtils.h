#ifndef WIDGETUTILS_H
#define WIDGETUTILS_H

#include <QSqlField>
#include <QString>
#include <QWidget>
#include <QVariant>

namespace WidgetUtils
{
   QWidget *findChildByFieldName(QWidget *form, QString field_name);
   QVariant::Type widgetType(QWidget *wdt);
   QVariant getWidgetValue(QWidget *wdt);
   bool setWidgetValue(QWidget* wdt, QVariant val);
   QSqlField createField(QWidget *wdt, QString field_name);
   bool isA(QObject *obj, const char *type);
   bool isAObject(QObject *obj, const char *type);
};

#endif
