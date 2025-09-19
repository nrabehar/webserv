#include "webserv.hpp"

FileCache::FileCache() {}
FileCache::~FileCache() {}

void FileCache::put(const std::string & path, const std::string & content)
{
    struct stat st;
    CacheEntry entry;

    entry.content = content;
    if (stat(path.c_str(), &st) != 0)
        entry.m_time = std::time(NULL);
    else
        entry.m_time = st.st_mtime;
    _cache[path] = entry;
}

std::string FileCache::get(const std::string & path)
{
    CacheMap::iterator it = _cache.find(path);
    if (it == _cache.end())
        return std::string(); // or throw an exception
    return it->second.content;
}

bool FileCache::exists(const std::string & path)
{
    CacheMap::iterator it = _cache.find(path);
    if (it == _cache.end())
        return false;
    struct stat st;
    if (stat(path.c_str(), &st) != 0)
        return false;
    if (st.st_mtime > it->second.m_time)
    {
        _cache.erase(it);
        return false;
    }
    return true;
}

void FileCache::update(std::string const & path)
{
    CacheMap::iterator it = _cache.find(path);
    if (it != _cache.end())
        _cache.erase(it);
}
