#ifndef CACHE_HPP
#define CACHE_HPP

#include "../../webserv.hpp"

enum CacheType {
    CAT_FILE,
};

class ICacheStrategy
{
    public:

        virtual ~ICacheStrategy() {}
        virtual void put(const std::string &, const std::string &) = 0;
        virtual std::string get(const std::string &) = 0;
        virtual bool exists(const std::string &) = 0;

};

class CacheManager
{
    private:

        std::map<CacheType, ICacheStrategy *> _strategy;
        ICacheStrategy *              _curr_st;

    public:

        ~CacheManager();
        static CacheManager * getInstance();

        bool exists(const std::string &);
        std::string get(const std::string &);
        void put(const std::string &, const std::string &);
        void use(CacheType);
    
    private:

        CacheManager();
        CacheManager(const CacheManager &);
        CacheManager & operator=(const CacheManager &);

};

#endif // CACHE_HPP