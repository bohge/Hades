#ifndef LUAJIT_MEMORYPOOL
#define LUAJIT_MEMORYPOOL

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif
EXTERNC void pool_init(size_t size);
EXTERNC void* pool_alloc_memory(size_t size);
EXTERNC int pool_free_memory(void *ptr, size_t size);

#endif