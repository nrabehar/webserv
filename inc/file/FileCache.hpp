#ifndef FILECACHE_HPP
#define FILECACHE_HPP

#include "../webserv.hpp"

class FileCache: public ICacheStrategy
{

    private:

        struct CacheEntry {
            std::string     content;
            time_t          m_time;
        };
        typedef std::map<std::string, CacheEntry> CacheMap;

        CacheMap	_cache;

    public:

        FileCache();
        virtual ~FileCache();

        virtual void put(const std::string &, const std::string &);
        virtual const std::string & get(const std::string &);
        virtual bool exists(const std::string &);
        virtual void update(std::string const &);

    private:

        FileCache(const FileCache &);
        FileCache & operator=(const FileCache &);

};

#endif
