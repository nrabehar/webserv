#ifndef MEMORY_TPP
#define MEMORY_TPP

template <typename T>
T * _nilof<T>::instance()
{
  return (0);
}

template <typename T>
void __typedDeleter(void * ptr)
{
  char * obj_ptr;
  char * block_ptr;
  void * block;
  MemoryManager<T> & manager = MemoryManager<T>::instance();
  if (!ptr)
  {
    return ;
  }
  T * object = static_cast<T *>(ptr);
  if (object == manager.getSingleton())
  {
    return ;
  }
  object->~T();
  obj_ptr = reinterpret_cast<char *>(object);
  block_ptr = obj_ptr - sizeof(memory_block_header);
  block = static_cast<void *>(block_ptr);
  manager.returnToPool(block);
}

template <typename T>
void * __typedInstance()
{
  return (static_cast<void*>(_nilof<T>::instance()));
}

template <typename T>
MemoryPool<T>::~MemoryPool()
{
}

template <typename T>
MemoryPool<T>::MemoryPool() : _free_block(0), _used_block(0), _registar(this)
{
  __static_assert
  (
    (_type<T>::nb > 0) &&
    (_storage_size <= __align((_type<T>::kb * 1024)))
  );
  _free_block = 0;
  _used_block = 0;
  for (size_t i = 0; i < _type<T>::nb; ++i)
  {
    _block * b = reinterpret_cast<_block *>(_stock.raw + (i * _aligned_size));
    b->next = _free_block;
    _free_block = b;
  }
}

template <typename T>
MemoryPool<T>::MemoryPool(const MemoryPool<T> & src)
{
  (*this) = src;
}

template <typename T>
MemoryPool<T> & MemoryPool<T>::operator=(const MemoryPool<T> & rhs)
{
  if (this != &rhs)
  {
  }
  return (*this);
}

template <typename T>
bool MemoryPool<T>::ownsBlock(const void * ptr) const
{
  if (!ptr)
  {
    return false;
  }
  const char * obj_ptr = static_cast<const char *>(ptr);
  const char * pool_start = _stock.raw;
  const char * pool_end = _stock.raw + _storage_size;
  return (obj_ptr >= pool_start && obj_ptr < pool_end);
}

template <typename T>
void * MemoryPool<T>::giveBlock()
{
  _block * block;
  if (!_free_block || (_used_block >= _type<T>::nb))
  {
    return (0);
  }
  block = _free_block;
  _free_block = _free_block->next;
  ++_used_block;
  return (static_cast<void *>(block));
}

template <typename T>
void MemoryPool<T>::takeBlock(void * ptr)
{
  _block * block;
  if (!_used_block || !ptr || !ownsBlock(ptr))
  {
    return ;
  }
  block = static_cast<_block *>(ptr);
  block->next = _free_block;
  _free_block = block;
  --_used_block;
}

template <typename T>
void MemoryPool<T>::print(std::ostream & stream) const
{
  stream << "[ " << _used_block << " / " << _type<T>::nb << " ]";
}

template <typename T>
MemoryManager<T>::~MemoryManager()
{
}

template <typename T>
MemoryManager<T>::MemoryManager() : _pool(),
_singleton(_nilof<T>::instance())
{
}

template <typename T>
MemoryManager<T>::MemoryManager(const MemoryManager<T> & src) : _pool(),
_singleton(_nilof<T>::instance())
{
  (*this) = src;
}

template <typename T>
MemoryManager<T> & MemoryManager<T>::operator=(const MemoryManager<T> & rhs)
{
  if (this != &rhs)
  {
  }
  return (*this);
}

template <typename T>
MemoryManager<T> & MemoryManager<T>::instance()
{
  static MemoryManager<T> _i;
  return (_i);
}

template <typename T>
T * MemoryManager<T>::placeObject()
{
  void * obj_ptr;
  char * blok_ptr;
  void * block;
  memory_block_header * header;
  if (ft::failure())
  {
    return (_singleton);
  }
  block = _pool.giveBlock();
  if (!block)
  {
    return (_singleton);
  }
  header = static_cast<memory_block_header *>(block);
  header->del = &__typedDeleter<T>;
  header->i = &__typedInstance<T>;
  blok_ptr = static_cast<char *>(block);
  obj_ptr = blok_ptr + sizeof(memory_block_header);
  return (new (obj_ptr) T());
}

template <typename T>
template <typename A1>
T * MemoryManager<T>::placeObject(A1 a1)
{
  void * obj_ptr;
  char * blok_ptr;
  void * block;
  memory_block_header * header;
  if (ft::failure())
  {
    return (_singleton);
  }
  block = _pool.giveBlock();
  if (!block)
  {
    return (_singleton);
  }
  header = static_cast<memory_block_header *>(block);
  header->del = &__typedDeleter<T>;
  header->i = &__typedInstance<T>;
  blok_ptr = static_cast<char *>(block);
  obj_ptr = blok_ptr + sizeof(memory_block_header);
  return (new (obj_ptr) T(a1));
}

template <typename T>
template <typename A1, typename A2>
T * MemoryManager<T>::placeObject(A1 a1, A2 a2)
{
  void * obj_ptr;
  char * blok_ptr;
  void * block;
  memory_block_header * header;
  if (ft::failure())
  {
    return (_singleton);
  }
  block = _pool.giveBlock();
  if (!block)
  {
    return (_singleton);
  }
  header = static_cast<memory_block_header *>(block);
  header->del = &__typedDeleter<T>;
  header->i = &__typedInstance<T>;
  blok_ptr = static_cast<char *>(block);
  obj_ptr = blok_ptr + sizeof(memory_block_header);
  return (new (obj_ptr) T(a1, a2));
}

template <typename T>
template <typename A1, typename A2, typename A3>
T * MemoryManager<T>::placeObject(A1 a1, A2 a2, A3 a3)
{
  void * obj_ptr;
  char * blok_ptr;
  void * block;
  memory_block_header * header;
  if (ft::failure())
  {
    return (_singleton);
  }
  block = _pool.giveBlock();
  if (!block)
  {
    return (_singleton);
  }
  header = static_cast<memory_block_header *>(block);
  header->del = &__typedDeleter<T>;
  header->i = &__typedInstance<T>;
  blok_ptr = static_cast<char *>(block);
  obj_ptr = blok_ptr + sizeof(memory_block_header);
  return (new (obj_ptr) T(a1, a2, a3));
}

template <typename T>
void MemoryManager<T>::removeObject(T * object)
{
  char * obj_ptr;
  char * block_ptr;
  void * block;
  if (!object || object == _singleton)
  {
    return ;
  }
  object->~T();
  obj_ptr = reinterpret_cast<char *>(object);
  block_ptr = obj_ptr - sizeof(memory_block_header);
  block = static_cast<void *>(block_ptr);
  _pool.takeBlock(block);
}

template <typename T>
void MemoryManager<T>::returnToPool(void * block)
{
  _pool.takeBlock(block);
}

template <typename T>
T * MemoryManager<T>::getSingleton() const
{
  return (_singleton);
}

template <typename T>
void MemoryManager<T>::print(std::ostream & stream) const
{
  _pool.print(stream);
}

template <typename T>
T * ft::alloc()
{
  return (MemoryManager<T>::instance().placeObject());
}

template <typename T, typename A1>
T * ft::alloc(A1 a1)
{
  return (MemoryManager<T>::instance().placeObject(a1));
}

template <typename T, typename A1, typename A2>
T * ft::alloc(A1 a1, A2 a2)
{
  return (MemoryManager<T>::instance().placeObject(a1, a2));
}

template <typename T, typename A1, typename A2, typename A3>
T * ft::alloc(A1 a1, A2 a2, A3 a3)
{
  return (MemoryManager<T>::instance().placeObject(a1, a2, a3));
}

template <typename T>
void ft::free(T *& object)
{
  char * obj_ptr;
  char * block_ptr;
  void * block;
  memory_block_header * header;
  T * singleton;
  if (!object)
  {
    return ;
  }
  obj_ptr = reinterpret_cast<char *>(object);
  block_ptr = obj_ptr - sizeof(memory_block_header);
  block = static_cast<void *>(block_ptr);
  if (!isFromPool(block))
  {
    return ;
  }
  header = reinterpret_cast<memory_block_header *>(block_ptr);
  singleton = static_cast<T*>(header->i());
  if (object == singleton || !header->del)
  {
    return ;
  }
  header->del(object);
  object = singleton;
}

#endif
