#include "filetable.h"

namespace sap
{

FileRecord::FileRecord(const std::string table_name): Record(table_name)
{
  add_column("bird_ID", TYPE_STRING);
  add_column("file_index", TYPE_INT);
  add_column("file_name", TYPE_STRING);
  add_column("file_age", TYPE_DOUBLE);
  add_column("bird_age", TYPE_DOUBLE);
}

FileRecord::~FileRecord()
{
}

} // namespace

/* vim: set cindent sw=2 expandtab : */

