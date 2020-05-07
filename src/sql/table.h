#ifndef SAP_TABLE_H__
#define SAP_TABLE_H__
#include <string>
#include <unordered_map>
#include <vector>
#include "column.h"
#include "sql/mysql.h"

namespace sap
{

template <typename R>
class Table
{
public:
  Table(const std::string& name);
  R* new_record();
  bool create(MySQL& connection);
protected:
  const std::string name_;
};

class Record
{
public:
  enum types
  {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_STRING,
  };
  Record(const std::string& table);
  virtual ~Record();

  void add_column(std::string name, types type);

  void set(const std::string& name, int i);
  void set(const std::string& name, double d);
  void set(const std::string& name, const std::string& s);

  std::string create_table();

  bool insert(MySQL& connection);
  Column* get(const std::string& name) { return columns_[name]; }

protected:
  std::unordered_map<std::string, Column*> columns_;
  std::vector<Column*> ordered_columns_;
  std::string table_;
};

// {{{ Implementation

template <typename R>
Table<R>::Table(const std::string& name) : name_(name)
{
}

template <typename R>
R* Table<R>::new_record()
{
  return new R(name_);
}

template <typename R>
bool Table<R>::create(MySQL& connection)
{
  R record(name_);
  std::string query = record.create_table();
  return connection.run(query);
}

// }}}

} // namespace

#endif // SAP_TABLE_H__

/* vim: set cindent sw=2 expandtab foldmethod=marker : */

