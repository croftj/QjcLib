#include "Exceptions.h"
#include "QcjData/QcjDataStatics.h"


QcjLib::SqlException::SqlException(const QString msg) : std::exception()
{
   m_msg = msg;
}

const char *QcjLib::SqlException::what() const noexcept
{
   return(qPrintable(m_msg));
}

