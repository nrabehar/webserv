#include "core/memory/memory.hpp"

IPrintable::~IPrintable() {}

std::ostream & operator<<(std::ostream & ostream, const IPrintable & object)
{
  object.print(ostream);
  return (ostream);
}

IMemoryPool::~IMemoryPool() {}

bool isFromPool(const void * ptr)
{
  for (size_t i = 0; i < poolRegistry().size(); ++i)
  {
    if (poolRegistry()[i]->ownsBlock(ptr))
    {
      return (true);
    }
  }
  return (false);
}

bool ft::_failure = false;

bool ft::failure()
{
  return (ft::_failure);
}

void ft::failure(bool state)
{
  ft::_failure = state;
}

std::vector<IMemoryPool *> & poolRegistry()
{
  static std::vector<IMemoryPool *> _pools;
  return (_pools);
}

MemoryPoolRegistar::~MemoryPoolRegistar()
{
  if (!_pool)
  {
    return;
  }
  std::vector<IMemoryPool *> & pools = poolRegistry();
  pools.erase(std::remove(pools.begin(), pools.end(), _pool), pools.end());
}

MemoryPoolRegistar::MemoryPoolRegistar()
{
}

MemoryPoolRegistar::MemoryPoolRegistar(const MemoryPoolRegistar & src)
{
  (*this) = src;
}

MemoryPoolRegistar & MemoryPoolRegistar::operator=(const MemoryPoolRegistar & rhs)
{
  if (this != &rhs)
  {
  }
  return (*this);
}

MemoryPoolRegistar::MemoryPoolRegistar(IMemoryPool * pool) : _pool(pool)
{
  if (pool)
  {
    poolRegistry().push_back(pool);
  }
}
