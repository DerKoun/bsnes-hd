#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>

// Workaround since we don't support file mappings on Horizon, but this is currently not functional.
// To make it work, the file could be read when the file_map is opened
#define PROT_READ 0b001
#define PROT_WRITE 0b010
#define PROT_EXEC 0b100
#define MAP_PRIVATE 2
#define MAP_FIXED 0x10
#define MAP_ANONYMOUS 0x20
#define MAP_FAILED ((void *)-1)
#define MAP_SHARED 0

static inline void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
  return malloc(len);
}

static inline int mprotect(void *addr, size_t len, int prot)
{
  return 0;
}

static inline int munmap(void *addr, size_t len)
{
  free(addr);
  return 0;
}

#ifdef __cplusplus
}
#endif
