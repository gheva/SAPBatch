#include "column.h"
#include <sstream>
#include <cmath>

namespace sap
{

Column::Column(const std::string& name) : name_(name)
{
}

Column::~Column()
{
}

void Column::set(int i)
{
}

void Column::set(const std::string& s)
{
}

void Column::set(double d)
{
}

std::string Column::column_sql()
{
  return "";
}

std::string Column::name()
{
  return name_;
}

std::string Column::value()
{
  return "";
}

StringColumn::StringColumn(const std::string& name) : Column(name)
{
}

std::string StringColumn::value()
{
  std::stringstream ss;
  ss << "'" << value_ << "'";
  return ss.str();
}

void StringColumn::set(const std::string& s)
{
  value_ = s;
}

std::string StringColumn::column_sql()
{
  return name_ + " TEXT";
}

IntColumn::IntColumn(const std::string& name) : Column(name)
{
}

std::string IntColumn::value()
{
  std::stringstream ss;
  ss << value_;
  return ss.str();
}

void IntColumn::set(int i)
{
  value_ = i;
}

std::string IntColumn::column_sql()
{
  return name_ + " INT";
}

DoubleColumn::DoubleColumn(const std::string& name) : Column(name)
{
}

std::string DoubleColumn::value()
{
  std::stringstream ss;
  ss << value_;
  return ss.str();
}

void DoubleColumn::set(double d)
{
  if (std::isnan(d))
  {
    value_ = 0;
  }
  else
  {
    value_ = d;
  }
}

std::string DoubleColumn::column_sql()
{
  return name_ + " DOUBLE";
}

} // namespace

/* vim: set cindent sw=2 expandtab : */

