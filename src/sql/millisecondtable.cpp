#include "millisecondtable.h"

namespace sap
{

MillisecondRecord::MillisecondRecord(const std::string table_name) : Record(table_name)
{
  add_column("file_index", TYPE_INT);
  add_column("index_in_file", TYPE_INT);
  add_column("pitch", TYPE_DOUBLE);
}

MillisecondRecord::~MillisecondRecord()
{
}

} // namespace

/* vim: set cindent sw=2 expandtab : */

