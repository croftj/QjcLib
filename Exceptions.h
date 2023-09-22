#ifndef QCJLIB_EXCEPTION
#define QCJLIB_EXCEPTION

#include <exception>
#include <QString>

namespace QcjLib
{
   class SqlException : public std::exception
   {
      public:
         SqlException(const QString msg);
         virtual const char *what() const noexcept;
      private:
         QString  m_msg;
   };
}

#endif
