#ifndef SAP_MYSQL_H__
#define SAP_MYSQL_H__
#include <mysql.h>
#include <string>

namespace sap
{

class MySQL
{
public:
  MySQL(std::string user, std::string password, std::string dbname);
  MySQL(std::string user, std::string password);
  virtual ~MySQL();
  bool run(const std::string& query);
  std::string error();
  bool is_valid() { return valid_; }
private:
  bool valid_;
  std::string username_;
  std::string password_;
  std::string dbname_;
  MYSQL* connection_;
};

} // namespace

#endif // SAP_MYSQL_H__

/* vim: set cindent sw=2 expandtab : */

