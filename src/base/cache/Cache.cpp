#include "webserv.hpp"

CacheManager::CacheManager(): _curr_st(NULL) {}

CacheManager::~CacheManager()
{
	std::map<CacheType, ICacheStrategy *>::iterator it;

	for (it = _strategy.begin(); it != _strategy.end(); ++it)
		delete it->second;
	_strategy.clear();
}

CacheManager* CacheManager::getInstance()
{
	static CacheManager instance;   
	return (&instance);
}


bool CacheManager::exists(const std::string & key)
{
	if (!_curr_st)
		return (false);
	return (_curr_st->exists(key));
}

const std::string & CacheManager::get(const std::string & key)
{
	static std::string empty;
	if (!_curr_st)
		return (empty);
	return (_curr_st->get(key));
}

void CacheManager::put(const std::string & key, const std::string & value)
{
	if (!_curr_st)
		return ;
	_curr_st->put(key, value);
}

void CacheManager::use(CacheType type)
{

	ICacheStrategy * st = NULL;
	if (_strategy.count(type))
	{
		_curr_st = _strategy[type];
		return ;
	}

	switch (type)
	{
		case CAT_FILE:
			st = new FileCache();
			break;
		default: return ;
	}
	if (st)
	{
		_strategy[type] = st;
		_curr_st = st;
	}
}

