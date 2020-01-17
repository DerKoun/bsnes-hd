#pragma once

//service model template built on top of shared-memory

#include <nall/shared-memory.hpp>

#if defined(API_POSIX) && !defined(PLATFORM_HORIZON)
  #include <nall/posix/service.hpp>
#endif

// For Horizon we re-use the Windows stub
#if defined(API_WINDOWS) || defined(PLATFORM_HORIZON)
  #include <nall/windows/service.hpp>
#endif
