#include <mysql.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// {{{ class MySQL
class MySQL
{
public:
  MySQL(string user, string password, string dbname) : valid_(false),
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

  MySQL(string user, string password) : valid_(false),
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

  ~MySQL()
  {
    mysql_close(connection_);
  }

  bool run(const string& query)
  {
    if (mysql_query(connection_, query.c_str()) != 0)
    {
      return false;
    }
    return true;
  }

  string error() { return mysql_error(connection_); }

  bool is_valid() { return valid_; }
private:
  bool valid_;
  string username_;
  string password_;
  string dbname_;
  MYSQL* connection_;
};
// }}}

// {{{ create database
void create_database(const string& user, const string password, const string dbname)
{
  MYSQL* connection = mysql_init(NULL);
  if (connection == NULL) 
  {
    cerr << mysql_error(connection) << endl;
    exit(EXIT_FAILURE);
  }

  if (mysql_real_connect(connection, "localhost", user.c_str(), password.c_str(), 
          NULL, 0, NULL, 0) == NULL) 
  {
    cerr << mysql_error(connection) << endl;
    mysql_close(connection);
    exit(EXIT_FAILURE);
  }

  string query = "CREATE DATABASE " + dbname;
  if (mysql_query(connection, query.c_str()) != 0)
  {
    cerr << mysql_error(connection) << endl;
    mysql_close(connection);
    exit(EXIT_FAILURE);
  }

  mysql_close(connection);
}
// }}}

class column
{
public:
  column(const string& name) : name_(name) {}
  virtual ~column() {}
  virtual void set(int i) {}
  virtual void set(double d) {}
  virtual void set(string s) {}
  virtual string column_sql() { return ""; }
protected:
  string name_;
};

class string_column : public column
{
public:
  string_column(const string& name) : column(name) {}
  virtual void set(const string& s) { value_ = s; }
  virtual string column_sql() { return name_ + " TEXT"; }
private:
  string value_;
};

class int_column : public column
{
public:
  int_column(const string& name) : column(name) {}
  virtual void set(int i) { value_ = i; }
  virtual string column_sql() { return name_ + " INT"; }
private:
  int value_;
};

class double_column : public column
{
public:
  double_column(const string& name) : column(name) {}
  virtual void set(double d) { value_ = d; }
  virtual string column_sql() { return name_ + " DOUBLE"; }
private:
  double value_;
};

class Record
{
public:
  Record(const string& table) : table_(table) {}
  enum types
  {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_STRING,
  };
  void add_column(string name, types type)
  {
    switch (type)
    {
    case TYPE_INT:
      columns_.emplace(name, new int_column(name));
      break;
    case TYPE_DOUBLE:
      columns_.emplace(name, new double_column(name));
      break;
    case TYPE_STRING:
      columns_.emplace(name, new string_column(name));
      break;
    }
    ordered_columns_.push_back(columns_[name]);
  }
  void set(const string& name, int i)
  {
    columns_[name]->set(i);
  }
  void set(const string& name, double d)
  {
    columns_[name]->set(d);
  }
  void set(const string& name, const string& s)
  {
    columns_[name]->set(s);
  }
  string create_table()
  {
    string ret = "CREATE TABLE IF NOT EXISTS " + table_;
    ret += "(";
    string delim = "";
    for (int i = 0; i < ordered_columns_.size(); ++i)
    {
      ret += delim + ordered_columns_[i]->column_sql();
      delim = ",";
    }
    ret += ")";
    return ret;
  }

protected:
  unordered_map<string, column*> columns_;
  vector<column*> ordered_columns_;
  string table_;
};

class milisecond_record : public Record
{
public:
  milisecond_record(const string& table) : Record(table)
  {
    add_column("file", TYPE_STRING);
    add_column("index_in_file", TYPE_INT);
    add_column("pitch", TYPE_DOUBLE);
  }
};


template <typename T>
class table
{
public:
  table(const string& name) : name_(name) {}
  T* new_record() { return new T(name_); }

  bool create(MySQL& connection)
  {
    T record(name_);
    string query = record.create_table();
    return connection.run(query);
  }

protected:
  string name_;
};

int main(int argc, char** argv)
{
  if (argc < 4)
  {
    cerr << "Usage " << argv[0] << " username password dbname" << endl;
    exit(EXIT_FAILURE);
  }
  //create_database(argv[1], argv[2], argv[3]);
  MySQL connection(argv[1], argv[2], argv[3]);
  if (!connection.is_valid())
  {
    cerr << connection.error() << endl;
    exit(EXIT_FAILURE);
  }
  table<milisecond_record> my_table("miliseconds");
  if (!my_table.create(connection))
  {
    cerr << connection.error() << endl;
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}

/* vim: set cindent sw=2 expandtab foldmethod=marker : */

