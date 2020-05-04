#include "directoryiterator.h"

namespace sap
{

DirectoryIterator::DirectoryIterator(const std::string& path) : root_(path), index_(0)
{
#ifdef POSIX
  directory_ = opendir(path.c_str());
#elif defined(WIN32)
  std::string root = path + "\\*";
  directory_ = FindFirstFile(root.c_str(), &FindFileData);;
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

DirectoryIterator::iterator* DirectoryIterator::next_file()
{
  const std::lock_guard<std::mutex> lock(mutex_);
  return _next_file();
}

DirectoryIterator::iterator* DirectoryIterator::_next_file()
{
#ifdef POSIX
  struct dirent* entry = readdir(directory_);
  if (entry == nullptr)
  {
    return nullptr;
  }
  while (entry->d_name[0] == '.')
  {
    entry = readdir(directory_);
    if (entry == nullptr)
    {
      return nullptr;
    }
  }
  iterator* ret = new iterator;
  ret->file_name = entry->d_name;
  ret->file_path = root_ + "/" + entry->d_name;
  ret->file_index = ++index_;

  return ret;
#elif defined(WIN32)
  while (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
  {
    if (FindNextFile(directory_, &FindFileData) == 0)
    {
      return nullptr;
    }
  }
  iterator* ret = new iterator;
  ret->file_name = FindFileData.cFileName;
  ret->file_path = root_ + "/" + FindFileData.cFileName;
  ret->file_index = ++index_;
  if (FindNextFile(directory_, &FindFileData) == 0)
  {
    return nullptr;
  }

  return ret;
#endif
}

#ifdef POSIX
#elif defined(WIN32)
#endif
} // namespace

/* vim: set cindent sw=2 expandtab : */

