#include "table.h"
#include <sstream>
#include <iostream>

namespace sap
{

Record::Record(const std::string& table) : table_(table)
{
}

void Record::add_column(std::string name, types type)
{
  switch (type)
  {
  case TYPE_INT:
    columns_.emplace(name, new IntColumn(name));
    break;
  case TYPE_DOUBLE:
    columns_.emplace(name, new DoubleColumn(name));
    break;
  case TYPE_STRING:
    columns_.emplace(name, new StringColumn(name));
    break;
  }
  ordered_columns_.push_back(columns_[name]);
}

void Record::set(const std::string& name, int i)
{
  columns_[name]->set(i);
}

void Record::set(const std::string& name, double d)
{
  columns_[name]->set(d);
}

void Record::set(const std::string& name, const std::string& s)
{
  columns_[name]->set(s);
}

std::string Record::create_table()
{
  std::string ret = "CREATE TABLE IF NOT EXISTS " + table_;
  ret += "(";
  std::string delim = "";
  for (int i = 0; i < ordered_columns_.size(); ++i)
  {
    ret += delim + ordered_columns_[i]->column_sql();
    delim = ",";
  }
  ret += ")";
  return ret;
}

bool Record::insert(MySQL& connection)
{
  std::stringstream ss;

  ss << "INSERT INTO " << table_ << "(";
  std::string delim = "";
  for (int i = 0; i < ordered_columns_.size(); ++i)
  {
    ss << delim << ordered_columns_[i]->name();
    delim = ",";
  }
  ss << ") VALUES (";
  delim = "";
  for (int i = 0; i < ordered_columns_.size(); ++i)
  {
    ss << delim << ordered_columns_[i]->value();
    delim = ",";
  }
  ss << ")";
  std::cerr << ss.str() << std::endl;
  return connection.run(ss.str());
}

} // namespace

/* vim: set cindent sw=2 expandtab : */

