/******************************************************************************/
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
#include "SqlTableModel.h"

using namespace QcjLib;

const QString SqlTableModel::LOG("QcjLib_table_model");
static LogBuilder mylog(SqlTableModel::LOG, 1, "QcjLib Table Model");

#if 0
QSqlRecord SqlTableModel::insertBlankRecord()
{
   QSqlRecord rv = record();
   QString index_field = pFormDef->getIndexField();
   for (fld_count = 0; fld_count < rv.count(); fld_count++)
   {
      QSqlField = field(fld_count);
      QString default_value = field.defaultValue();

      if (field.name() == index_field)
      {
         defau
         def_value = QString("nextval('") + model.tableName() + "_"
                     + rec.fieldName(idx) + "_seq'::regclass)";


}
#endif
