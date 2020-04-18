#include "sql/table.h"
#include <iostream>

using namespace sap;
using namespace std;

class milisecond_record : public Record
{
public:
  milisecond_record(const std::string& table) : Record(table)
  {
    add_column("file", TYPE_STRING);
    add_column("index_in_file", TYPE_INT);
    add_column("pitch", TYPE_DOUBLE);
  }
};

int main(int argc, char** argv)
{
  if (argc < 4)
  {
    cerr << "Usage " << argv[0] << " username password dbname" << endl;
    exit(EXIT_FAILURE);
  }
  MySQL connection(argv[1], argv[2], argv[3]);
  Table<milisecond_record> milisecond_table("millis");
  if (!milisecond_table.create(connection))
  {
    cerr << connection.error() << endl;
    exit(EXIT_FAILURE);
  }
  Record* r = milisecond_table.new_record();
  r->set("file", "a file named sue");
  r->set("index_in_file", 1);
  r->set("pitch", 1.0);
  if (!r->insert(connection))
  {
    cerr << connection.error() << endl;
  }
}

/* vim: set cindent sw=2 expandtab : */

