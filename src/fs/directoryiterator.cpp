#include "directoryiterator.h"

namespace sap
{

DirectoryIterator::DirectoryIterator(const std::string& path) : root_(path)
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
  std::string ret = root_ + "\\" + FindFileData.cFileName;
  if (FindNextFile(directory_, &FindFileData) == 0)
  {
    return "";
  }
  return ret;
#endif
}

#ifdef POSIX
#elif defined(WIN32)
#endif
} // namespace

/* vim: set cindent sw=2 expandtab : */

