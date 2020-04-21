#ifndef SAP_SYNQUEUE_H__
#define SAP_SYNQUEUE_H__

#include <condition_variable>
#include <mutex>
#include <queue>
#include "sql/table.h"

namespace sap
{

class SynQueue
{
public:
  SynQueue();
  virtual ~SynQueue();
  void enqueue(Record* r);
  Record* dequeue();
  void done();
protected:
private:
  std::queue<Record*> queue_;
  std::mutex mutex_;
  std::condition_variable condition_;
  bool done_;
};

} // namespace

#endif // SAP_SYNQUEUE_H__

/* vim: set cindent sw=2 expandtab : */

