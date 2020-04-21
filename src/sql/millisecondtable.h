#ifndef SAP_MILLISECONDTABLE_H__
#define SAP_MILLISECONDTABLE_H__

#include "table.h"

namespace sap
{

class MillisecondRecord : public Record
{
public:
  MillisecondRecord(const std::string table_name);
  virtual ~MillisecondRecord();
protected:
private:
};

typedef Table<MillisecondRecord> MillisecondTable;

} // namespace

#endif // SAP_MILLISECONDTABLE_H__

/* vim: set cindent sw=2 expandtab : */

