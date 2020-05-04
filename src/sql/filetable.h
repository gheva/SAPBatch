#ifndef SQL_FILETABLE_H__
#define SQL_FILETABLE_H__

#include "table.h"

namespace sap
{

class FileRecord : public Record
{
public:
  FileRecord(const std::string table_name);
  virtual ~FileRecord();
protected:
private:
};

typedef Table<FileRecord> FileTable;
} // namespace

#endif // SQL_FILETABLE_H__

/* vim: set cindent sw=2 expandtab : */

