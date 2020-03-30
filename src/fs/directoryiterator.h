#ifndef SAP_DIRECTORYITERATOR_H__
#define SAP_DIRECTORYITERATOR_H__

#include <string>
#include <mutex>
#ifdef POSIX
#include <dirent.h>
#elif defined(WIN32)
#endif

namespace sap
{

class DirectoryIterator
{
public:
  DirectoryIterator(const std::string& path);
  virtual ~DirectoryIterator();
  std::string next_file();
protected:
private:
  std::string _next_file();
  std::mutex mutex_;
  std::string root_;
#ifdef POSIX
  DIR* directory_;
#elif defined(WIN32)
  WIN32_FIND_DATA FindFileData;
  HANDLE directory_;
#endif
};

} // namespace

#endif // SAP_DIRECTORYITERATOR_H__

/* vim: set cindent sw=2 expandtab : */

