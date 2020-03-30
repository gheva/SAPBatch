#include "directoryiterator.h"

namespace sap
{

DirectoryIterator::DirectoryIterator(const std::string& path) : root_(path)
{
#ifdef POSIX
  directory_ = opendir(path.c_str());
#elif defined(WIN32)
  directory_ = FindFirstFile(path, &FindFileData);;
#endif
}

DirectoryIterator::~DirectoryIterator()
{
#ifdef POSIX
  closedir(directory_);
#elif defined(WIN32)
  FindClose(directory_);
#endif
}

std::string DirectoryIterator::next_file()
{
  const std::lock_guard<std::mutex> lock(mutex_);
  return _next_file();
}

std::string DirectoryIterator::_next_file()
{
#ifdef POSIX
  struct dirent* entry = readdir(directory_);
  if (entry == nullptr)
  {
    return "";
  }
  while (entry->d_name[0] == '.')
  {
    entry = readdir(directory_);
    if (entry == nullptr)
    {
      return "";
    }
  }

  return root_ + "/" + entry->d_name;
#elif defined(WIN32)
  while (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
  {
    if (FindNextFile(directory_, &FindFileData) == 0)
    {
      return "";
    }
  }
  // TODO this is not going to work and needs to be tested
  std::sting ret = FindFileData.cFileName;
  FindNextFile(directory_, &FindFileData);
  return ret;
#endif
}

#ifdef POSIX
#elif defined(WIN32)
#endif
} // namespace

/* vim: set cindent sw=2 expandtab : */

