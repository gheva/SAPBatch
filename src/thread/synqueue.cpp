#include "synqueue.h"

namespace sap
{

SynQueue::SynQueue() : done_(false)
{
}

SynQueue::~SynQueue()
{
}

void SynQueue::enqueue(Record* r)
{
  std::lock_guard<std::mutex> lock(mutex_);
  queue_.push(r);
  condition_.notify_one();
}

void SynQueue::done()
{
  std::unique_lock<std::mutex> lock(mutex_);
  done_ = true;
}

Record* SynQueue::dequeue()
{
  std::unique_lock<std::mutex> lock(mutex_);
  while(queue_.empty())
  {
    if (done_ && queue_.empty())
    {
      return nullptr;
    }
    condition_.wait(lock);
  }
  Record* record = queue_.front();
  queue_.pop();
  return record;
}

} // namespace

/* vim: set cindent sw=2 expandtab : */

