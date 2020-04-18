#include "mysql.h"

namespace sap
{

MySQL::MySQL(std::string user, std::string password, std::string dbname) : valid_(false),
  username_(user), password_(password), dbname_(dbname)
{
  connection_ = mysql_init(NULL);
  if (connection_ != NULL)
  {
    if (mysql_real_connect(connection_, "localhost", username_.c_str(),
        password_.c_str(), dbname_.c_str(), 0, NULL, 0) != NULL) 
    {
      valid_ = true;
    }
  }
}

MySQL::MySQL(std::string user, std::string password) : valid_(false),
  username_(user), password_(password)
{
  connection_ = mysql_init(NULL);
  if (connection_ != NULL)
  {
    if (mysql_real_connect(connection_, "localhost", username_.c_str(),
        password_.c_str(), NULL, 0, NULL, 0) != NULL) 
    {
      valid_ = true;
    }
  }
}

MySQL::~MySQL()
{
  mysql_close(connection_);
}

bool MySQL::run(const std::string& query)
{
  if (mysql_query(connection_, query.c_str()) != 0)
  {
    return false;
  }
  return true;
}

std::string MySQL::error()
{
  return mysql_error(connection_);
}

} // namespace

/* vim: set cindent sw=2 expandtab : */

