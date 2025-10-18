#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <iostream>
#include <vector>
#include <algorithm>

class IPrintable
{
  public:
  virtual ~IPrintable();
  virtual void print(std::ostream & ostream) const = 0;
};

std::ostream & operator<<(std::ostream & ostream, const IPrintable & object);

template <bool>
struct __assert;

template <>
struct __assert<true>
{
};

#undef __static_assert
#define __static_assert(expr) (void)sizeof(__assert<(expr)>)

#undef __max
#define __max(a, b) ((a) > (b) ? (a) : (b))

#undef __min
#define __min(a, b) ((a) < (b) ? (a) : (b))

#undef __x32
#define __x32(x) (((x) + 31) / 32 * 32)

#undef __x64
#define __x64(x) (((x) + 63) / 64 * 64)

#undef __align
#define __align(x) ((sizeof(void *) == 4) ? (__x32(x)) : (__x64(x)))

#undef __max_kb
#define __max_kb 2048

struct memory_block_header
{
  void (*del)(void*);
  void * (*i)();
};

template <typename T>
void __typedDeleter(void * ptr);

template <typename T>
void * __typedInstance();

bool isFromPool(const void * ptr);

template <typename T>
struct _nilof
{
  static T * instance();
};

template <typename T>
struct _type
{
  enum
  {
    is_custom = false,
    nb = 2048,
    kb = __min((nb * sizeof(T)), __max_kb),
  };
};

#undef __register_nil_true
#define __register_nil_true(TYPE) \
template <> \
struct _nilof<TYPE> \
{ \
  static TYPE * instance() \
  { \
    return (TYPE::instance()); \
  } \
};

#undef __register_nil_false
#define __register_nil_false(TYPE) \
template <> \
struct _nilof<TYPE> \
{ \
  static TYPE * instance() \
  { \
    return (0); \
  } \
};

#undef __register_type
#define __register_type(TYPE, COUNT, ISCUSTOM) \
template <> \
struct _type<TYPE> \
{ \
  enum \
  { \
    is_custom = ISCUSTOM, \
    nb = (COUNT), \
    kb = __min((nb * sizeof(TYPE)), __max_kb), \
  }; \
}; \
__register_nil_##ISCUSTOM(TYPE)

class IMemoryPool
{
  public:
  virtual ~IMemoryPool();
  virtual bool ownsBlock(const void * ptr) const = 0;
  virtual void * giveBlock() = 0;
  virtual void takeBlock(void * ptr) = 0;
};

std::vector<IMemoryPool *> & poolRegistry();

class MemoryPoolRegistar
{
  private:
  IMemoryPool * _pool;
  public:
  virtual ~MemoryPoolRegistar();
  MemoryPoolRegistar(IMemoryPool * pool);
  private:
  MemoryPoolRegistar();
  MemoryPoolRegistar(const MemoryPoolRegistar & src);
  MemoryPoolRegistar & operator=(const MemoryPoolRegistar & rhs);
};

template <typename T>
class MemoryPool : public virtual IPrintable, public virtual IMemoryPool
{
  private:
  struct _block { _block * next; };
  enum
  {
    _header_size = sizeof(memory_block_header),
    _object_size = sizeof(T),
    _combined_size = _header_size + _object_size,
    _block_size = __max(sizeof(_block), _combined_size),
    _aligned_size = __align(_block_size),
    _storage_size = (_aligned_size * _type<T>::nb),
  };
  union _storage
  {
    char raw[_storage_size];
    double align_d;
    long double align_ld;
    void* align_ptr;
  };
  _storage _stock;
  _block * _free_block;
  size_t _used_block;
  MemoryPoolRegistar _registar;
  public:
  virtual ~MemoryPool();
  MemoryPool();
  virtual bool ownsBlock(const void * ptr) const;
  virtual void * giveBlock();
  virtual void takeBlock(void * ptr);
  virtual void print(std::ostream & stream) const;
  private:
  MemoryPool(const MemoryPool & src);
  MemoryPool & operator=(const MemoryPool & rhs);
};

template <typename T>
class MemoryManager : public virtual IPrintable
{
  private:
  MemoryPool<T> _pool;
  T * _singleton;
  public:
  virtual ~MemoryManager();
  static MemoryManager & instance();
  T * placeObject();
  template <typename A1>
    T * placeObject(A1 a1);
  template <typename A1, typename A2>
    T * placeObject(A1 a1, A2 a2);
  template <typename A1, typename A2, typename A3>
    T * placeObject(A1 a1, A2 a2, A3 a3);
  void removeObject(T * object);
  void returnToPool(void * block);
  T * getSingleton() const;
  virtual void print(std::ostream & stream) const;
  private:
  MemoryManager();
  MemoryManager(const MemoryManager & src);
  MemoryManager & operator=(const MemoryManager & rhs);
};

struct ft
{
  private:
  static bool _failure;
  public:
  static bool failure();
  static void failure(bool state);
  template <typename T>
    static T * alloc();
  template <typename T, typename A1>
    static T * alloc(A1 a1);
  template <typename T, typename A1, typename A2>
    static T * alloc(A1 a1, A2 a2);
  template <typename T, typename A1, typename A2, typename A3>
    static T * alloc(A1 a1, A2 a2, A3 a3);
  template <typename T>
    static void free(T *& object);
};

#include "core/memory/memory.ipp"

#endif
