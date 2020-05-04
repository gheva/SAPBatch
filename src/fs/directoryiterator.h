#ifndef SAP_DIRECTORYITERATOR_H__
#define SAP_DIRECTORYITERATOR_H__

#include <string>
#include <mutex>
#ifdef POSIX
#include <dirent.h>
#elif defined(WIN32)
#include <windows.h>
#endif

namespace sap
{

class DirectoryIterator
{
public:
  struct iterator
  {
    std::string file_name;
    std::string file_path;
    int file_index;
  };
  DirectoryIterator(const std::string& path);
  virtual ~DirectoryIterator();
  iterator* next_file();
protected:
private:
  iterator* _next_file();
  std::mutex mutex_;
  std::string root_;
  unsigned int index_;
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

