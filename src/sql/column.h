#ifndef SAP_COLUMN_H__
#define SAP_COLUMN_H__
#include <string>

namespace sap
{

class Column
{
public:
  Column(const std::string& name);
  virtual ~Column();
  virtual void set(int i);
  virtual void set(long long i);
  virtual void set(double d);
  virtual void set(const std::string& s);
  virtual std::string column_sql();
  virtual std::string value();
  std::string name();
protected:
  std::string name_;
};

class StringColumn : public Column
{
public:
  StringColumn(const std::string& name);
  virtual void set(const std::string& s);
  virtual std::string column_sql();
  virtual std::string value();
private:
  std::string value_;
};

class IntColumn : public Column
{
public:
  IntColumn(const std::string& name);
  virtual void set(int i);
  virtual void set(long long i);
  virtual std::string column_sql();
  virtual std::string value();
  int int_value() { return value_; }
private:
  long long value_;
};

class DoubleColumn : public Column
{
public:
  DoubleColumn(const std::string& name);
  virtual void set(double d);
  virtual std::string column_sql();
  virtual std::string value();
  double double_value() { return value_; }
private:
  double value_;
};


} // namespace

#endif // SAP_COLUMN_H__

/* vim: set cindent sw=2 expandtab : */

